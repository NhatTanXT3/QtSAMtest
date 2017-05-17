#include <QTimer>
#include <iostream>
#include <QTextStream>
#include <QDebug>
#include "main_sam_test.h"
#include "ui_main_sam_test.h"
#include "myserialport.h"

mySerialPort *subCon;

Main_SAM_test::Main_SAM_test(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Main_SAM_test)
{
    ui->setupUi(this);
    connect(ui->Buttonconnect,SIGNAL(clicked(bool)),this,SLOT(connectButton_clicked()));
    connect(ui->buttonTest,SIGNAL(clicked(bool)),this,SLOT(testButton_clicked()));
    connect(ui->dialPos,SIGNAL(valueChanged(int)),this,SLOT(dialPos_valueChanged()));
    connect(ui->dialPos8bit,SIGNAL(valueChanged(int)),this,SLOT(dialpos8bitValuechanged()));
    connect(ui->pushButton_SAMGetPos,SIGNAL(clicked(bool)),this,SLOT(pushButSAMGetPos_clicked()));

    //    connect(ui->pushButton_SAMSetPassive,SIGNAL(clicked(bool)),this,SLOT(on_pushButton_SAMSetPassive_clicked()));
    subCon = new mySerialPort();
    connect(subCon,SIGNAL(packageAvailable(QByteArray)),this, SLOT(dataReceive(QByteArray)));

    //    QTimer *timer = new QTimer(this);
    //    timer->setTimerType(Qt::PreciseTimer);
    //    connect(timer, SIGNAL(timeout()), this, SLOT(taskUpdate()));
    //    timer->start(1);
}

Main_SAM_test::~Main_SAM_test()
{
    delete ui;
}

void Main_SAM_test::setSamPos12(quint8 ID, quint16 Pos)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::getSamPos12(quint8 ID)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::setSamPos8(quint8 ID, quint8 Pos,quint8 Mode)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::getSamPos8(quint8 ID)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::setPassive(quint8 ID)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::getPID(quint8 ID)
{
    QByteArray ba;
    ba.resize(4);
    ba[0] = 0xff;
    ba[1] = 0x95;
    ba[2] = ID&0x1F;
    ba[3] =0xfe;
    subCon->serialSend(ba);
}

void Main_SAM_test::setPID(quint8 ID, quint8 Pvalue, quint8 Ivalue, quint8 Dvalue)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::taskUpdate()
{
    //qDebug()<<"timer task";
    //  subCon->serialSend("helloworld");
}

void Main_SAM_test::connectButton_clicked()
{
    std::cout<<"connect_button cliked";
    qDebug()<<"connect_button cliked";
    //    subCon->initSerialPort();
}

void Main_SAM_test::testButton_clicked()
{
    //     char *str;
    QString str;
    str = "hellowold";
    std::cout<<"test_button cliked";
    qDebug()<<"test_button cliked";
    //     subCon->serialSend("helloworld");
    //     subCon->serialSend(str.toLatin1().data());

    quint8 ID=0;
    quint8 Mode=0;
    quint16 Pos=0x37F;
    QByteArray ba;
    ba.resize(6);
    ba[0] = 0xff;
    ba[1] = (((Mode&0x0C)<<3)+ID)&0x7F;
    ba[2] = (((Mode&0x03)<<5)+(Pos>>7))&0x7F;
    ba[3] = Pos&0x7F;
    ba[4] = (ba.at(1)^ba.at(2)^ba.at(3))&0x7F;
    ba[5] =0xfe;
    //    subCon->serialSend(ba);
}

void Main_SAM_test::dialPos_valueChanged()
{
    ui->spinBox_SAMSetPos->setValue(ui->dialPos->value());
    setSamPos12(ui->spinBox_SAMID->value(),ui->dialPos->value());
    qDebug()<<"dialPos_Changed: ID " << ui->spinBox_SAMID->value() <<
              "Pos" << ui->dialPos->value();
}

void Main_SAM_test::dialpos8bitValuechanged()
{
    ui->spinBox_SAMSetPos8->setValue(ui->dialPos8bit->value());
    setSamPos8(ui->spinBox_SAMID->value(),ui->dialPos8bit->value(),ui->spinBox_SAMTorque->value());
    qDebug()<<"dialPos_Changed: Mode " << ui->spinBox_SAMID->value() <<
              " Torque: " << ui->spinBox_SAMTorque->value()<<
              "Pos" << ui->dialPos8bit->value();
}

void Main_SAM_test::pushButSAMGetPos_clicked()
{
    getSamPos12(ui->spinBox_SAMID->value());
}

void Main_SAM_test::dataReceive(QByteArray myData)
{
    qDebug()<<"Data come";
    if((quint8)myData.at(1)&0x80){ //special function return
        qDebug()<<"special function";
        if((quint8)myData.at(1)==0x95)
        {

            quint8 id=myData.at(2)&0x1E;
            quint8 Pvalue=myData.at(4)+((myData.at(3)<<5)&0x80);
            quint8 Ivalue=myData.at(5)+((myData.at(3)<<6)&0x80);
            quint8 Dvalue=myData.at(6)+((myData.at(3)<<7)&0x80);
             qDebug()<<"PID get" << Pvalue <<Ivalue <<Dvalue;
             QString a;
             ui->label_SAMP->setText(a.setNum(Pvalue));
             ui->label_SAMI->setText(a.setNum(Ivalue));
             ui->label_SAMD->setText(a.setNum(Dvalue));

        }
    }
    else{//standard function return
        if(myData.size()==6){
            quint8 id;
            quint16 pos;
            QString a;

            id=(quint8)myData.at(1);
            pos=(((quint8)myData.at(2))<<7)+((quint8)myData.at(3));
            qDebug()<<"stardand function"<<id<<pos;
            ui->label_SAMGetPos->setText(a.setNum(pos));
        }
        else if(myData.size()==7)
        {
            quint8 id;
            quint8 mode;
            quint8 pos8;
            quint8 torq8;
            QString a;

            mode=myData.at(1)&0x7F;
            id=(quint8)myData.at(2)&0x1F;

            pos8=(((myData.at(2))<<2)&0x80)+myData.at(4);
            torq8=(((myData.at(2))<<1)&0x80)+myData.at(3);
            qDebug()<<"Pos 8"<<id<<pos8<<torq8 <<"mode"<<mode;
            ui->label_SAMGetPos8->setText(a.setNum(pos8));
            ui->label_SAMGetTorq8->setText(a.setNum(torq8));

        }
        else{
            qDebug()<<"error length st"<<myData;
        }

    }
}

void Main_SAM_test::on_pushButton_SAMSetPassive_clicked()
{
    setPassive(ui->spinBox_SAMID->value());
    qDebug()<<"set passive"<<ui->spinBox_SAMID->value();
}

void Main_SAM_test::on_pushButton_SAMSetPos_clicked()
{

    setSamPos12(ui->spinBox_SAMID->value(),ui->spinBox_SAMSetPos->value());
    qDebug()<<"set pos 12"<<ui->spinBox_SAMID->value()<<ui->spinBox_SAMSetPos->value();
}

void Main_SAM_test::on_pushButton_SAMSetPos8_clicked()
{
    setSamPos8(ui->spinBox_SAMID->value(),ui->spinBox_SAMSetPos8->value(),ui->spinBox_SAMTorque->value());
    qDebug()<<"set pos 8"<<ui->spinBox_SAMID->value()<<ui->spinBox_SAMSetPos8->value();
}

void Main_SAM_test::on_pushButton_SAMGetPos8_clicked()
{
    getSamPos8(ui->spinBox_SAMID->value());
    qDebug()<<"get pos 8";
}

void Main_SAM_test::on_pushButton_2_clicked()
{

}


void Main_SAM_test::on_pushButton_SAMGetPID_clicked()
{
    getPID(ui->spinBox_SAMID->value());
    qDebug()<<"get PID";
}

void Main_SAM_test::on_pushButton_SAMSetPID_clicked()
{
    setPID(ui->spinBox_SAMID->value(),ui->spinBox_SAMP->value(),ui->spinBox_SAMI->value(),ui->spinBox_SAMD->value());
    qDebug()<<"set PID"<<ui->spinBox_SAMP->value()<< ui->spinBox_SAMI->value()<< ui->spinBox_SAMD->value();
}
