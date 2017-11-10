#ifndef SAM_H
#define SAM_H

#include <QObject>
#include <QtSerialPort/QtSerialPort>
class SAM : public QObject
{
    Q_OBJECT
    QSerialPort *serial;
public:
    explicit SAM(QObject *parent = 0);


    void serialSend(const QByteArray &byteArray);
    void initConnection();
    void setSamPos12(quint8 ID,quint16 Pos);
    void getSamPos12(quint8 ID);

    void setSamAverageTorq(quint8 ID,quint16 ATorq);
    void getSamAverageTorq(quint8 ID);

    void setSamPos8(quint8 ID,quint8 Pos,quint8 Mode);
    void getSamPos8(quint8 ID);
    void setPassive(quint8 ID);

    void getPID(quint8 ID);
    void setPID(quint8 ID,quint8 Pvalue,quint8 Ivalue,quint8 Dvalue);

    void setPDQuick(quint8 ID,quint8 Pvalue,quint8 Dvalue);

    void getAllPos12();
    void getAllPos12Full();
    void getAllPos8Torq8();
    void setAllPassive();
    void SAM_Power_enable(quint8 state);
    void setAllPos12(quint16 *Pos,quint8 numOfSam);
    void setAllAverageTorque(quint16 *Atorq,quint8 numOfSam);
    void getAllAverageTorque();

    void setAllPDQuick(quint8 *Pvalue,quint8 *Dvalue,quint8 numOfSam);
    void getAllPDQuick();
signals:
void packageAvailable(QByteArray );

public slots:
    void serialReceived();
};

#endif // SAM_H
