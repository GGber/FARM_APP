#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtNetwork/QTcpServer>
#include <QtNetwork/QTcpSocket>
#include <QtNetwork/QHostInfo>
#include <QLabel>
#include <QDebug>
#include <QTimer>
#include <QDateTime>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QMessageBox>
#include "myfarmer.h"
#include "excel_data.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    Excel_Data *My_db_data;

private:
    Ui::MainWindow *ui;

private:

    QTcpSocket *Mysocket;

    QTimer *Mytimer;
    QTimer *TCP_timer;//检测时间
    QTimer *Data_timer;//数据更新时间
    QLabel *Label_time;

    Myfarmer *My_Farmer;
    SetData *My_set;
    QByteArray TCP_well_buf;//水井分配帧
    QByteArray Send_str;
    QByteArray Get_str;
    QByteArray buffer;//接收到的buf

    QString IP;//定义需要连接服务器的ip；
    quint16 port;//定义需要连接服务器的端口
    quint8 Start_send=0;//数据发送启动标识

    quint8 TCP_sta=0;//TCP心跳
    quint8 Send_var=1;//发送的类型，1表示读，2表示写
    quint8 serial_exist_queue[20];//存在的串口

    QSerialPort *Myserial;


private slots:

    void socket_Read_Data();

    void socket_Disconnected();//连接断开函数

    void Timer_update();//显示时间函数

    void on_toolButton_connect_clicked();//连接以太网

    void on_toolButton_user_clicked();

    void on_toolButton_Manage_clicked();

    void Send_data_to_ARM();

    void Rearch_TCP_Sta();

    void Time_send_ARM();

    void on_toolButton_set_clicked();
    void on_toolButton_Analysis_clicked();
    void on_pushButton_set_sure_clicked();
    void on_checkBox1_1_stateChanged(int arg1);
    void on_checkBox1_2_stateChanged(int arg1);
    void on_checkBox1_3_stateChanged(int arg1);
    void on_checkBox1_4_stateChanged(int arg1);
    void on_checkBox1_5_stateChanged(int arg1);
    void on_checkBox2_1_stateChanged(int arg1);
    void on_checkBox2_2_stateChanged(int arg1);
    void on_checkBox2_3_stateChanged(int arg1);
    void on_checkBox2_4_stateChanged(int arg1);
    void on_checkBox2_5_stateChanged(int arg1);
    void on_checkBox3_1_stateChanged(int arg1);
    void on_checkBox3_2_stateChanged(int arg1);
    void on_checkBox3_3_stateChanged(int arg1);
    void on_checkBox3_4_stateChanged(int arg1);
    void on_checkBox3_5_stateChanged(int arg1);
    void on_checkBox4_1_stateChanged(int arg1);
    void on_checkBox4_2_stateChanged(int arg1);
    void on_checkBox4_3_stateChanged(int arg1);
    void on_checkBox4_4_stateChanged(int arg1);
    void on_checkBox4_5_stateChanged(int arg1);
    void on_checkBox5_1_stateChanged(int arg1);
    void on_checkBox5_2_stateChanged(int arg1);
    void on_checkBox5_3_stateChanged(int arg1);
    void on_checkBox5_4_stateChanged(int arg1);
    void on_checkBox5_5_stateChanged(int arg1);

    void Serial_search_function();

};

#endif // MAINWINDOW_H
