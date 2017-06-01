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

    void setSamAverageTorq(quint8 ID,quint16 ATorq);
    void getSamAverageTorq(quint8 ID);

    void setSamPos8(quint8 ID,quint8 Pos,quint8 Mode);
    void getSamPos8(quint8 ID);
    void setPassive(quint8 ID);

    void getPID(quint8 ID);
    void setPID(quint8 ID,quint8 Pvalue,quint8 Ivalue,quint8 Dvalue);

    void setPDQuick(quint8 ID,quint8 Pvalue,quint8 Dvalue);

    void getAllPos12();
    void getAllPos8Torq8();
    void setAllPassive();
    void SAM_Power_enable(quint8 state);
    void setAllPos12(quint16 *Pos,quint8 numOfSam);
    void setAllAverageTorque(quint16 *Atorq,quint8 numOfSam);
    void getAllAverageTorque();

    void setAllPDQuick(quint8 *Pvalue,quint8 *Dvalue,quint8 numOfSam);
    void getAllPDQuick();

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
    void on_pushButton_SAMAllGetPos8_clicked();
    void on_pushButton_SAMGetAllPos12_clicked();
    void on_pushButton_SAMGetPos_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_5_clicked();
    void on_pushButton_4_clicked();
    void on_pushButton_20_clicked();
    void on_pushButton_SetAllPassive_clicked();
    void on_pushButton_PpwerSAMs_clicked();
    void on_pushButton_turnOffSams_clicked();
    void on_pushButton_turnOnSams_clicked();
    void on_pushButton_setSAMs12_clicked();
    void on_pushButton_SAMSetAveragTorq_clicked();
    void on_pushButton_SAMGetAveragTorq_clicked();
    void on_pushButton_SAMSetPD_quick_clicked();
    void on_pushButton_setAvergTorq_clicked();
    void on_pushButton_getAvergTorq_clicked();
    void on_pushButton_setPDquickAll_clicked();
    void on_pushButton_getPDQuickAll_clicked();
};

#endif // MAIN_SAM_TEST_H
