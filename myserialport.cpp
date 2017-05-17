#include <QDebug>
#include <QtSerialPort/QtSerialPort>
#include "myserialport.h"
QSerialPort *serial;
mySerialPort::mySerialPort(QObject *parent) : QObject(parent)
{
    initSerialPort();
}

mySerialPort::~mySerialPort()
{
    serial->close();
}

void mySerialPort::serialSend(const QByteArray &byteArray)
{
    serial->write(byteArray);
}

void mySerialPort::initSerialPort()
{
    serial=new QSerialPort(this);
    serial->setPortName("ttyUSB0");
//    serial->setPortName("ttyACM0");
//    serial->setBaudRate(QSerialPort::Baud115200);
    serial->setBaudRate(1500000);
    serial->setDataBits(QSerialPort::Data8);
    serial->setParity(QSerialPort::NoParity);
    serial->setStopBits(QSerialPort::OneStop);
    serial->setFlowControl(QSerialPort::NoFlowControl);
    serial->open(QIODevice::ReadWrite);
    serial->write("ok");
    connect(serial,SIGNAL(readyRead()),this,SLOT(serialReceived()));
}

void mySerialPort::serialReceived()
{
    QByteArray myData;
    myData=serial->readAll();
    qDebug()<<"size: "<<myData.size()<<" Data: "<<myData;
    myData.remove(0,myData.indexOf(0xff));
     qDebug()<<myData;
    if((myData.at(0)==(char)0xff)&&(myData.at(myData.size()-1)==(char)0xfe))
    {
        emit packageAvailable(myData);
//
    }
    else{
        qDebug()<<"error receive package: "<<myData.at(0)<<" "<<myData.at(myData.size()-1);
    }
//    QString mystring;
//    mystring=serial->readAll();
//    qDebug()<<mystring << " "<<mystring.indexOf("\r\n") <<" "<<mystring.count();
}
