#ifndef MAIN_SAM_TEST_H
#define MAIN_SAM_TEST_H

#include <QMainWindow>

namespace Ui {
class Main_SAM_test;
}

class Main_SAM_test : public QMainWindow
{
    Q_OBJECT

public:
    explicit Main_SAM_test(QWidget *parent = 0);
    ~Main_SAM_test();
    void setSamPos12(quint8 ID,quint16 Pos);
    void getSamPos12(quint8 ID);

    void setSamPos8(quint8 ID,quint8 Pos,quint8 Mode);
    void getSamPos8(quint8 ID);
    void setPassive(quint8 ID);

    void getPID(quint8 ID);
    void setPID(quint8 ID,quint8 Pvalue,quint8 Ivalue,quint8 Dvalue);


private:
    Ui::Main_SAM_test *ui;

public slots:
    void connectButton_clicked();
    void testButton_clicked();
    void dialPos_valueChanged();
    void dialpos8bitValuechanged();
    void pushButSAMGetPos_clicked();
    void dataReceive(QByteArray);
    void taskUpdate();
private slots:
    void on_pushButton_SAMSetPassive_clicked();
    void on_pushButton_SAMSetPos_clicked();
    void on_pushButton_SAMSetPos8_clicked();
    void on_pushButton_SAMGetPos8_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_SAMGetPID_clicked();
    void on_pushButton_SAMSetPID_clicked();
};

#endif // MAIN_SAM_TEST_H
