#ifndef MYSERIALPORT_H
#define MYSERIALPORT_H

#include <QObject>

class mySerialPort : public QObject
{
    Q_OBJECT
public:
    explicit mySerialPort(QObject *parent = 0);
    ~mySerialPort();

    void serialSend(const QByteArray &byteArray); // arguments by a reference, and constant
    void initSerialPort();
private:
signals:
    void packageAvailable(QByteArray );
public slots:
    void serialReceived();

};

#endif // MYSERIALPORT_H
