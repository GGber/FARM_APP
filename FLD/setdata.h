#ifndef SETDATA_H
#define SETDATA_H

#include <QWidget>
#include <QDebug>


namespace Ui {
class SetData;
}

class SetData : public QWidget
{
    Q_OBJECT

public:
    explicit SetData(QWidget *parent = nullptr);
    ~SetData();

    QByteArray TCP_SendData;//定义TCP发送的数组

    int Farm_Number=0;//定义田编号

public slots:

    void Change_Farm_Number();

    void Data_sta_clear();

private slots:

    void on_pushButton_Sure_clicked();

    void on_pushButton_Cancel_clicked();


    void on_checkBox_Contrl_1_stateChanged(int arg1);

    void on_checkBox_Contrl_2_stateChanged(int arg1);

    void on_checkBox_Contrl_3_stateChanged(int arg1);

private:
    Ui::SetData *ui;

    quint16 Relay_Send=0xf000;  //定义继电器控制位
    char Auto_Send=0;        //定义模式设置
    quint16 State_Send=0;       //定义采集点状态设置


signals:
    void Data_set_OK();
};

#endif // SETDATA_H
