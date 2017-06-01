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

void Main_SAM_test::setSamAverageTorq(quint8 ID, quint16 ATorq)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::getSamAverageTorq(quint8 ID)
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

void Main_SAM_test::setPDQuick(quint8 ID, quint8 Pvalue, quint8 Dvalue)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::getAllPos12()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xcc;
    ba[2] =0xfe;
    subCon->serialSend(ba);
}

void Main_SAM_test::getAllPos8Torq8()
{

    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xec;
    ba[2] =0xfe;
    subCon->serialSend(ba);
}

void Main_SAM_test::setAllPassive()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0x88;
    ba[2] =0xfe;
    subCon->serialSend(ba);
}

void Main_SAM_test::SAM_Power_enable(quint8 state)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::setAllPos12(quint16 *Pos, quint8 numOfSam)
{
    QByteArray ba;
    ba.resize(numOfSam+3);
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
    subCon->serialSend(ba);

}

void Main_SAM_test::setAllAverageTorque(quint16 *Atorq, quint8 numOfSam)
{
    QByteArray ba;
    ba.resize(numOfSam+3);
    ba[0] = 0xff;
    ba[1] = 0xbd;

    quint8 refIndex=2;
    for(quint8 i=0; i<numOfSam;i++)
    {
        if(*(Atorq+i)<4000)
        {
            ba[refIndex++]=i;//id
            ba[refIndex++]=(*(Atorq+i)>>7)&0x7F;
            ba[refIndex++]=*(Atorq+i)&0x7F;
            ba[refIndex++]=(ba.at(refIndex-3)^ba.at(refIndex-2)^ba.at(refIndex-1))&0x7F;
        }
    }
    ba[refIndex] =0xfe;
    subCon->serialSend(ba);
}

void Main_SAM_test::getAllAverageTorque()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xbf;
    ba[2] =0xfe;
    subCon->serialSend(ba);
}

void Main_SAM_test::setAllPDQuick(quint8 *Pvalue, quint8 *Dvalue, quint8 numOfSam)
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
    subCon->serialSend(ba);
}

void Main_SAM_test::getAllPDQuick()
{
    QByteArray ba;
    ba.resize(3);
    ba[0] = 0xff;
    ba[1] = 0xc3;
    ba[2] =0xfe;
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
        else if((quint8)myData.at(1)==0xcc)
        {
            quint16 samPos12[24];
            quint8 samAvail[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

            quint8 NumofSam=(myData.size()-3)/4;
            qDebug()<<"Num of Sam"<<NumofSam;
            for (unsigned char i=0;i<NumofSam;i++)
            {
                //                samPos12[bitand(bytes[(i-1)*4+3],31)+1]=bitand[bytes((i-1)*4+5),127)+bitshift(bitand(bytes((i-1)*4+4),31),7);
                if(myData.at(i*4+5)==((myData.at(i*4+2)^myData.at(i*4+3)^myData.at(i*4+4))&0x7F))
                {
                    samPos12[myData.at(i*4+2)&0x1F]=(myData.at(i*4+4)&0x7F)+((myData.at(i*4+3)&0x1F)<<7);
                    samAvail[myData.at(i*4+2)&0x1F]=1;
                }
                else
                    qDebug()<<"error checksum 1";
            }
            QString a;

            if(samAvail[0])
                ui->label_A_ID0_P12->setText(a.setNum(samPos12[0]));
            else ui->label_A_ID0_P12->setText(a.setNum(0));

            if(samAvail[1])
                ui->label_A_ID1_P12->setText(a.setNum(samPos12[1]));
            else ui->label_A_ID1_P12->setText(a.setNum(1));

            if(samAvail[2])
                ui->label_A_ID2_P12->setText(a.setNum(samPos12[2]));
            else ui->label_A_ID2_P12->setText(a.setNum(2));

            if(samAvail[3])
                ui->label_A_ID3_P12->setText(a.setNum(samPos12[3]));
            else ui->label_A_ID3_P12->setText(a.setNum(3));

            if(samAvail[4])
                ui->label_A_ID4_P12->setText(a.setNum(samPos12[4]));
            else ui->label_A_ID4_P12->setText(a.setNum(4));

            if(samAvail[5])
                ui->label_A_ID5_P12->setText(a.setNum(samPos12[5]));
            else ui->label_A_ID5_P12->setText(a.setNum(5));

            if(samAvail[6])
                ui->label_A_ID6_P12->setText(a.setNum(samPos12[6]));
            else ui->label_A_ID6_P12->setText(a.setNum(6));

            if(samAvail[7])
                ui->label_A_ID7_P12->setText(a.setNum(samPos12[7]));
            else ui->label_A_ID7_P12->setText(a.setNum(7));

            if(samAvail[8])
                ui->label_A_ID8_P12->setText(a.setNum(samPos12[8]));
            else ui->label_A_ID8_P12->setText(a.setNum(8));

            if(samAvail[9])
                ui->label_A_ID9_P12->setText(a.setNum(samPos12[9]));
            else ui->label_A_ID9_P12->setText(a.setNum(9));

            if(samAvail[10])
                ui->label_A_ID10_P12->setText(a.setNum(samPos12[10]));
            else ui->label_A_ID10_P12->setText(a.setNum(10));

            if(samAvail[11])
                ui->label_A_ID11_P12->setText(a.setNum(samPos12[11]));
            else ui->label_A_ID11_P12->setText(a.setNum(11));

            //            qDebug()<<"all Pos12"<<samPos12[0]<<samPos12[1]<<samPos12[2]<<samPos12[3]<<samPos12[4]
            //                   <<samPos12[5]<<samPos12[6]<<samPos12[7]<<samPos12[8]<<samPos12[9]<<samPos12[10]<<samPos12[11]<<samPos12[22];
        }
        else if((quint8)myData.at(1)==0xec)
        {
            quint8 samPos8[24];
            quint8 samLoad8[24];
            quint8 samAvail[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};
            QString a;
            quint8 NumofSam=(myData.size()-3)/4;
            qDebug()<<"all Pos12"<<NumofSam;
            for (unsigned char i=0;i<NumofSam;i++)
            {
                //                samPos12[bitand(bytes[(i-1)*4+3],31)+1]=bitand[bytes((i-1)*4+5),127)+bitshift(bitand(bytes((i-1)*4+4),31),7);
                if(myData.at(i*4+5)==((myData.at(i*4+2)^myData.at(i*4+3)^myData.at(i*4+4))&0x7F))
                {
                    samLoad8[myData.at(i*4+2)&0x1F]=(myData.at(i*4+3)&0x7F)+((myData.at(i*4+2)<<1)&0x80);
                    samPos8[myData.at(i*4+2)&0x1F]=(myData.at(i*4+4)&0x7F)+((myData.at(i*4+2)<<2)&0x80);
                    samAvail[myData.at(i*4+2)&0x1F]=1;
                }
                else
                    qDebug()<<"error checksum 2";
            }

            quint8 id=0;
            if(samAvail[id])
            {
                ui->label_A_ID0_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID0_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID0_P8->setText(a.setNum(0));
                ui->label_A_ID0_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID1_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID1_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID1_P8->setText(a.setNum(0));
                ui->label_A_ID1_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID2_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID2_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID2_P8->setText(a.setNum(0));
                ui->label_A_ID2_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID3_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID3_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID3_P8->setText(a.setNum(0));
                ui->label_A_ID3_L8->setText(a.setNum(0));
            }
            id++;
            if(samAvail[id])
            {
                ui->label_A_ID4_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID4_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID4_P8->setText(a.setNum(0));
                ui->label_A_ID4_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID5_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID5_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID5_P8->setText(a.setNum(0));
                ui->label_A_ID5_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID6_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID6_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID6_P8->setText(a.setNum(0));
                ui->label_A_ID6_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID7_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID7_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID7_P8->setText(a.setNum(0));
                ui->label_A_ID7_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID8_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID8_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID8_P8->setText(a.setNum(0));
                ui->label_A_ID8_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID9_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID9_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID9_P8->setText(a.setNum(0));
                ui->label_A_ID9_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID10_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID10_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID10_P8->setText(a.setNum(0));
                ui->label_A_ID10_L8->setText(a.setNum(0));
            }
            id++;

            if(samAvail[id])
            {
                ui->label_A_ID11_P8->setText(a.setNum(samPos8[id]));
                ui->label_A_ID11_L8->setText(a.setNum(samLoad8[id]));
            }
            else {
                ui->label_A_ID11_P8->setText(a.setNum(0));
                ui->label_A_ID11_L8->setText(a.setNum(0));
            }
            id++;



            qDebug()<<"all Pos8"<<samPos8[0]<<samPos8[1]<<samPos8[2]<<samPos8[3]<<samPos8[4]
                   <<samPos8[5]<<samPos8[6]<<samPos8[7]<<samPos8[8]<<samPos8[9];
            qDebug()<<"all Load8"<<samLoad8[0]<<samLoad8[1]<<samLoad8[2]<<samLoad8[3]<<samLoad8[4]
                   <<samLoad8[5]<<samLoad8[6]<<samLoad8[7]<<samLoad8[8]<<samLoad8[9];
        }
        else if((quint8)myData.at(1)==0xbf)
        {
            quint16 samAverTorq[24];
            quint8 samAvail[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

            quint8 NumofSam=(myData.size()-3)/4;
            qDebug()<<"Num of Sam"<<NumofSam;
            for (unsigned char i=0;i<NumofSam;i++)
            {
                //                samPos12[bitand(bytes[(i-1)*4+3],31)+1]=bitand[bytes((i-1)*4+5),127)+bitshift(bitand(bytes((i-1)*4+4),31),7);
                if(myData.at(i*4+5)==((myData.at(i*4+2)^myData.at(i*4+3)^myData.at(i*4+4))&0x7F))
                {
                    samAverTorq[myData.at(i*4+2)&0x1F]=(myData.at(i*4+4)&0x7F)+((myData.at(i*4+3)&0x1F)<<7);
                    samAvail[myData.at(i*4+2)&0x1F]=1;
                }
                else
                    qDebug()<<"error checksum 1";
            }
            QString a;

            if(samAvail[0])
                ui->label_A_ID0_P12_2->setText(a.setNum(samAverTorq[0]));
            else ui->label_A_ID0_P12_2->setText(a.setNum(0));

            if(samAvail[1])
                ui->label_A_ID1_P12_2->setText(a.setNum(samAverTorq[1]));
            else ui->label_A_ID1_P12_2->setText(a.setNum(1));

            if(samAvail[2])
                ui->label_A_ID2_P12_2->setText(a.setNum(samAverTorq[2]));
            else ui->label_A_ID2_P12_2->setText(a.setNum(2));

            if(samAvail[3])
                ui->label_A_ID3_P12_2->setText(a.setNum(samAverTorq[3]));
            else ui->label_A_ID3_P12_2->setText(a.setNum(3));

            if(samAvail[4])
                ui->label_A_ID4_P12_2->setText(a.setNum(samAverTorq[4]));
            else ui->label_A_ID4_P12_2->setText(a.setNum(4));

            if(samAvail[5])
                ui->label_A_ID5_P12_2->setText(a.setNum(samAverTorq[5]));
            else ui->label_A_ID5_P12_2->setText(a.setNum(5));

            if(samAvail[6])
                ui->label_A_ID6_P12_2->setText(a.setNum(samAverTorq[6]));
            else ui->label_A_ID6_P12_2->setText(a.setNum(6));

            if(samAvail[7])
                ui->label_A_ID7_P12_2->setText(a.setNum(samAverTorq[7]));
            else ui->label_A_ID7_P12_2->setText(a.setNum(7));

            if(samAvail[8])
                ui->label_A_ID8_P12_2->setText(a.setNum(samAverTorq[8]));
            else ui->label_A_ID8_P12_2->setText(a.setNum(8));

            if(samAvail[9])
                ui->label_A_ID9_P12_2->setText(a.setNum(samAverTorq[9]));
            else ui->label_A_ID9_P12_2->setText(a.setNum(9));

            if(samAvail[10])
                ui->label_A_ID10_P12_2->setText(a.setNum(samAverTorq[10]));
            else ui->label_A_ID10_P12_2->setText(a.setNum(10));

            if(samAvail[11])
                ui->label_A_ID11_P12_2->setText(a.setNum(samAverTorq[11]));
            else ui->label_A_ID11_P12_2->setText(a.setNum(11));

            //            qDebug()<<"all Pos12"<<samPos12[0]<<samPos12[1]<<samPos12[2]<<samPos12[3]<<samPos12[4]
            //                   <<samPos12[5]<<samPos12[6]<<samPos12[7]<<samPos12[8]<<samPos12[9]<<samPos12[10]<<samPos12[11]<<samPos12[22];
        }
        else if((quint8)myData.at(1)==0xc3)
        {
            quint8 samP[24];
            quint8 samD[24];
            quint8 samAvail[24]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

            quint8 NumofSam=(myData.size()-3)/4;
            qDebug()<<"Num of Sam"<<NumofSam;
            for (unsigned char i=0;i<NumofSam;i++)
            {
                //                samPos12[bitand(bytes[(i-1)*4+3],31)+1]=bitand[bytes((i-1)*4+5),127)+bitshift(bitand(bytes((i-1)*4+4),31),7);
                if(myData.at(i*4+5)==((myData.at(i*4+2)^myData.at(i*4+3)^myData.at(i*4+4))&0x7F))
                {
                    samP[myData.at(i*4+2)&0x1F]=(myData.at(i*4+3)&0x7F)+((myData.at(i*4+2)<<1)&0x80);
                    samD[myData.at(i*4+2)&0x1F]=(myData.at(i*4+4)&0x7F)+((myData.at(i*4+2)<<2)&0x80);
                    samAvail[myData.at(i*4+2)&0x1F]=1;
                }
                else
                    qDebug()<<"error checksum 1";
            }
            QString a;
            quint8 i=0;

            if(samAvail[i])
            {
                ui->label_A_ID0_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID0_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID0_P12_3->setText("x");
                ui->label_A_ID0_P12_4->setText("x");
            }
            i++;

            if(samAvail[i])
            {
                ui->label_A_ID1_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID1_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID1_P12_3->setText("x");
                ui->label_A_ID1_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID2_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID2_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID2_P12_3->setText("x");
                ui->label_A_ID2_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID3_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID3_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID3_P12_3->setText("x");
                ui->label_A_ID3_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID4_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID4_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID4_P12_3->setText("x");
                ui->label_A_ID4_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID5_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID5_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID5_P12_3->setText("x");
                ui->label_A_ID5_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID6_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID6_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID6_P12_3->setText("x");
                ui->label_A_ID6_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID7_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID7_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID7_P12_3->setText("x");
                ui->label_A_ID7_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID8_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID8_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID8_P12_3->setText("x");
                ui->label_A_ID8_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID9_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID9_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID9_P12_3->setText("x");
                ui->label_A_ID9_P12_4->setText("x");
            }
            i++;
            if(samAvail[i])
            {
                ui->label_A_ID10_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID10_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID10_P12_3->setText("x");
                ui->label_A_ID10_P12_4->setText("x");
            }
            i++;

            if(samAvail[i])
            {
                ui->label_A_ID11_P12_3->setText(a.setNum(samP[i]));
                ui->label_A_ID11_P12_4->setText(a.setNum(samD[i]));
            }
            else
            {
                ui->label_A_ID11_P12_3->setText("x");
                ui->label_A_ID11_P12_4->setText("x");
            }


            //            qDebug()<<"all Pos12"<<samPos12[0]<<samPos12[1]<<samPos12[2]<<samPos12[3]<<samPos12[4]
            //                   <<samPos12[5]<<samPos12[6]<<samPos12[7]<<samPos12[8]<<samPos12[9]<<samPos12[10]<<samPos12[11]<<samPos12[22];
        }
    }
    else{//standard function return
        if(myData.size()==6){
            quint8 id;
            quint8 mode;

            QString a;

            id=((quint8)myData.at(1))&0x1F;
            mode=((((quint8)myData.at(1))>>3)&0x0C)+((((quint8)myData.at(2))>>5)&0x03);
            if(mode==10){
                quint16 Atorq;
                Atorq=((((quint8)myData.at(2))&0x1F)<<7)+((quint8)myData.at(3));
                qDebug()<<"stardand function Atorq"<<id<<Atorq;
                ui->label_SAMGetAvergTorq->setText(a.setNum(Atorq));
            }
            else if(mode==0){
                quint16 pos;
                pos=((((quint8)myData.at(2))&0x1F)<<7)+((quint8)myData.at(3));
                qDebug()<<"stardand function Pos12"<<id<<pos;
                ui->label_SAMGetPos->setText(a.setNum(pos));
            }
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
    qDebug()<<"set pos12"<<ui->spinBox_SAMID->value()<<ui->spinBox_SAMSetPos->value();
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

void Main_SAM_test::on_pushButton_SAMAllGetPos8_clicked()
{
    getAllPos8Torq8();
}

void Main_SAM_test::on_pushButton_SAMGetAllPos12_clicked()
{
    getAllPos12();
}

void Main_SAM_test::on_pushButton_SAMGetPos_clicked()
{

}

void Main_SAM_test::on_pushButton_3_clicked()
{
    getAllPos12();
}

void Main_SAM_test::on_pushButton_5_clicked()
{
    getAllPos8Torq8();
}

void Main_SAM_test::on_pushButton_4_clicked()
{

}

void Main_SAM_test::on_pushButton_20_clicked()
{

}

void Main_SAM_test::on_pushButton_SetAllPassive_clicked()
{
    setAllPassive();
}

void Main_SAM_test::on_pushButton_PpwerSAMs_clicked()
{
    SAM_Power_enable(0);
}

void Main_SAM_test::on_pushButton_turnOffSams_clicked()
{
    SAM_Power_enable(0);
}

void Main_SAM_test::on_pushButton_turnOnSams_clicked()
{
    SAM_Power_enable(1);
}

void Main_SAM_test::on_pushButton_setSAMs12_clicked()
{
    quint16 Pos12[12];
    Pos12[0]=ui->spinBox_P_ID0_3->value();
    Pos12[1]=ui->spinBox_P_ID1_3->value();
    Pos12[2]=ui->spinBox_P_ID2_3->value();
    Pos12[3]=ui->spinBox_P_ID3_3->value();
    Pos12[4]=ui->spinBox_P_ID4_3->value();
    Pos12[5]=ui->spinBox_P_ID5_3->value();
    Pos12[6]=ui->spinBox_P_ID6_3->value();
    Pos12[7]=ui->spinBox_P_ID7_3->value();
    Pos12[8]=ui->spinBox_P_ID8_3->value();
    Pos12[9]=ui->spinBox_P_ID9_3->value();
    Pos12[10]=ui->spinBox_P_ID10_3->value();
    Pos12[11]=ui->spinBox_P_ID11_3->value();
    setAllPos12(Pos12,12);
}

void Main_SAM_test::on_pushButton_SAMSetAveragTorq_clicked()
{
    setSamAverageTorq(ui->spinBox_SAMID->value(),ui->spinBox_SAMSetAveraTorq->value());
}

void Main_SAM_test::on_pushButton_SAMGetAveragTorq_clicked()
{
    getSamAverageTorq(ui->spinBox_SAMID->value());
}

void Main_SAM_test::on_pushButton_SAMSetPD_quick_clicked()
{
    setPDQuick(ui->spinBox_SAMID->value(),ui->spinBox_SAMP->value(),ui->spinBox_SAMD->value());
}

void Main_SAM_test::on_pushButton_setAvergTorq_clicked()
{
    quint16 ATorq[12];
    ATorq[0]=ui->spinBox_P_ID0_4->value();
    ATorq[1]=ui->spinBox_P_ID1_4->value();
    ATorq[2]=ui->spinBox_P_ID2_4->value();
    ATorq[3]=ui->spinBox_P_ID3_4->value();
    ATorq[4]=ui->spinBox_P_ID4_4->value();
    ATorq[5]=ui->spinBox_P_ID5_4->value();
    ATorq[6]=ui->spinBox_P_ID6_4->value();
    ATorq[7]=ui->spinBox_P_ID7_4->value();
    ATorq[8]=ui->spinBox_P_ID8_4->value();
    ATorq[9]=ui->spinBox_P_ID9_4->value();
    ATorq[10]=ui->spinBox_P_ID10_4->value();
    ATorq[11]=ui->spinBox_P_ID11_4->value();
    setAllAverageTorque(ATorq,12);
}

void Main_SAM_test::on_pushButton_getAvergTorq_clicked()
{
    getAllAverageTorque();
}

void Main_SAM_test::on_pushButton_setPDquickAll_clicked()
{
    quint8 samP[12];
    quint8 samD[12];
    samP[0]=ui->spinBox_P_ID0_5->value();
    samP[1]=ui->spinBox_P_ID1_5->value();
    samP[2]=ui->spinBox_P_ID2_5->value();
    samP[3]=ui->spinBox_P_ID3_5->value();
    samP[4]=ui->spinBox_P_ID4_5->value();
    samP[5]=ui->spinBox_P_ID5_5->value();
    samP[6]=ui->spinBox_P_ID6_5->value();
    samP[7]=ui->spinBox_P_ID7_5->value();
    samP[8]=ui->spinBox_P_ID8_5->value();
    samP[9]=ui->spinBox_P_ID9_5->value();
    samP[10]=ui->spinBox_P_ID10_5->value();
    samP[11]=ui->spinBox_P_ID11_5->value();

    samD[0]=ui->spinBox_P_ID0_6->value();
    samD[1]=ui->spinBox_P_ID1_6->value();
    samD[2]=ui->spinBox_P_ID2_6->value();
    samD[3]=ui->spinBox_P_ID3_6->value();
    samD[4]=ui->spinBox_P_ID4_6->value();
    samD[5]=ui->spinBox_P_ID5_6->value();
    samD[6]=ui->spinBox_P_ID6_6->value();
    samD[7]=ui->spinBox_P_ID7_6->value();
    samD[8]=ui->spinBox_P_ID8_6->value();
    samD[9]=ui->spinBox_P_ID9_6->value();
    samD[10]=ui->spinBox_P_ID10_6->value();
    samD[11]=ui->spinBox_P_ID11_6->value();
    setAllPDQuick(samP,samD,12);
}

void Main_SAM_test::on_pushButton_getPDQuickAll_clicked()
{
    getAllPDQuick();
}
