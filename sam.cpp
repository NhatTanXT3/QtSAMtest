#include "sam.h"

SAM::SAM(QObject *parent) : QObject(parent)
{

}


void SAM::serialSend(const QByteArray &byteArray)
{
    serial->write(byteArray);
}

void SAM::initConnection()
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


void SAM::setSamPos12(quint8 ID, quint16 Pos)
{
    quint8 Mode=8;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = (((Mode&0x03)<<5)+(Pos>>7))&0x7F;
    ba[3] = Pos&0x7F;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    this->serialSend(ba);
}

void SAM::getSamPos12(quint8 ID)
{
    quint8 Mode=7;
    quint16 Pos=0;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = (((Mode&0x03)<<5)+(Pos>>7))&0x7F;
    ba[3] = Pos&0x7F;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    this->serialSend(ba);
}

void SAM::setSamAverageTorq(quint8 ID, quint16 ATorq)
{
    quint8 Mode=9;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = (((Mode&0x03)<<5)+(ATorq>>7))&0x7F;
    ba[3] = ATorq&0x7F;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    this->serialSend(ba);
}

void SAM::getSamAverageTorq(quint8 ID)
{
    quint8 Mode=10;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = ((Mode&0x03)<<5);
    ba[3] = 0;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    this->serialSend(ba);
}

void SAM::setSamPos8(quint8 ID, quint8 Pos,quint8 Mode)
{
    if (Mode>4)
        Mode=4;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = (((Mode&0x03)<<5)+((Pos>>7)&0x01))&0x7F;
    ba[3] = Pos&0x7F;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    this->serialSend(ba);
}

void SAM::getSamPos8(quint8 ID)
{

    quint8  Mode=5;
    quint8 Pos=0;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = (((Mode&0x03)<<5)+((Pos>>7)&0x01))&0x7F;
    ba[3] = Pos&0x7F;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    this->serialSend(ba);
}

void SAM::setPassive(quint8 ID)
{
    quint8 Mode=6;
    quint16 Pos=0;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = (((Mode&0x03)<<5)+(Pos>>7))&0x7F;
    ba[3] = Pos&0x7F;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    this->serialSend(ba);
}

void SAM::getPID(quint8 ID)
{
    QByteArray ba;
    ba.resize(4);
    ba[0] = 0xff;
    ba[1] = 0x95;
    ba[2] = ID&0x1F;
    ba[3] =0xfe;
    this->serialSend(ba);
}

void SAM::setPID(quint8 ID, quint8 Pvalue, quint8 Ivalue, quint8 Dvalue)
{
    QByteArray ba;
    ba.resize(9);
    ba[0] = 0xff;
    ba[1]=0xaa;
    ba[2] = ID&0x1F;
    ba[3] = ((Pvalue&0x80)>>5)+((Ivalue&0x80)>>6)+((Dvalue&0x80)>>7);
    ba[4] = Pvalue&0x7F;
    ba[5] = Ivalue&0x7F;
    ba[6] = Dvalue&0x7F;
    ba[7] = (ba.at(2)^ba.at(3)^ba.at(4)^ba.at(5)^ba.at(6))&0x7F;
    ba[8] =0xfe;
    this->serialSend(ba);
}

void SAM::setPDQuick(quint8 ID, quint8 Pvalue, quint8 Dvalue)
{
    QByteArray ba;
    ba.resize(7);
    ba[0] = 0xff;
    ba[1]=0xbb;
    ba[2] = (ID&0x1F)+((Pvalue&0x80)>>1)+((Dvalue&0x80)>>2);
    ba[3] = Pvalue&0x7F;
    ba[4] = Dvalue&0x7F;
    ba[5] = (ba.at(2)^ba.at(3)^ba.at(4))&0x7F;
    ba[6] =0xfe;
    this->serialSend(ba);
}

void SAM::getAllPos12()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xcc;
    ba[2] =0xfe;
    this->serialSend(ba);
}

void SAM::getAllPos12Full()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0x99;
    ba[2] =0xfe;
    this->serialSend(ba);
}

void SAM::getAllPos8Torq8()
{

    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xec;
    ba[2] =0xfe;
    this->serialSend(ba);
}

void SAM::setAllPassive()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0x88;
    ba[2] =0xfe;
    this->serialSend(ba);
}

void SAM::SAM_Power_enable(quint8 state)
{
    if(state)
        state=1;
    else
        state=0;
    QByteArray ba;
    ba.resize(4);
    ba[0] = 0xff;
    ba[1] = 0x81;
    ba[2] = state;
    ba[3] =0xfe;
    this->serialSend(ba);
}

void SAM::setAllPos12(quint16 *Pos, quint8 numOfSam)
{
    QByteArray ba;
    ba.resize(numOfSam*4+3);
    ba[0] = 0xff;
    ba[1] = 0xf0;

    quint8 refIndex=2;
    for(quint8 i=0; i<numOfSam;i++)
    {
        if((*(Pos+i)>400)&&(*(Pos+i)<3701))
        {
            ba[refIndex++]=i;//id
            ba[refIndex++]=(*(Pos+i)>>7)&0x7F;
            ba[refIndex++]=*(Pos+i)&0x7F;
            ba[refIndex++]=(ba.at(refIndex-3)^ba.at(refIndex-2)^ba.at(refIndex-1))&0x7F;
        }
    }
    ba[refIndex] =0xfe;
    this->serialSend(ba);

}

void SAM::setAllAverageTorque(quint16 *Atorq, quint8 numOfSam)
{
    QByteArray ba;
    ba.resize(numOfSam+3);
    ba[0] = 0xff;
    ba[1] = 0xbd;

    quint8 refIndex=2;
    for(quint8 i=0; i<numOfSam;i++)
    {
        if(*(Atorq+i)<4001)
        {
            ba[refIndex++]=i;//id
            ba[refIndex++]=(*(Atorq+i)>>7)&0x7F;
            ba[refIndex++]=*(Atorq+i)&0x7F;
            ba[refIndex++]=(ba.at(refIndex-3)^ba.at(refIndex-2)^ba.at(refIndex-1))&0x7F;
        }
    }
    ba[refIndex] =0xfe;
    this->serialSend(ba);
}

void SAM::getAllAverageTorque()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xbf;
    ba[2] =0xfe;
    this->serialSend(ba);
}

void SAM::setAllPDQuick(quint8 *Pvalue, quint8 *Dvalue, quint8 numOfSam)
{
    QByteArray ba;
    ba.resize(numOfSam+3);
    ba[0] = 0xff;
    ba[1] = 0xc1;

    quint8 refIndex=2;
    for(quint8 i=0; i<numOfSam;i++)
    {

        ba[refIndex++]=(i&0x1F)+(((*(Pvalue+i))&0x80)>>1)+(((*(Dvalue+i))&0x80)>>2);//id
        ba[refIndex++]=(*(Pvalue+i))&0x7F;
        ba[refIndex++]=(*(Dvalue+i))&0x7F;
        ba[refIndex++]=(ba.at(refIndex-3)^ba.at(refIndex-2)^ba.at(refIndex-1))&0x7F;

    }
    ba[refIndex] =0xfe;
    this->serialSend(ba);
}

void SAM::getAllPDQuick()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xc3;
    ba[2] =0xfe;
    this->serialSend(ba);
}

void SAM::serialReceived()
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
