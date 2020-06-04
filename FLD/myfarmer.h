#ifndef MYFARMER_H
#define MYFARMER_H

#include <QWidget>
#include <QTimer>
#include <QDebug>
#include "setdata.h"
#include "excel_data.h"
#include "ui_excel_data.h"

namespace Ui {
class Myfarmer;
}

class Myfarmer : public QWidget
{
    Q_OBJECT

public:
    explicit Myfarmer(QWidget *parent = nullptr);
    ~Myfarmer();

    SetData *My_set;

    Excel_Data *My_db_data;

    QString Data_string;//显示日期

public slots:

    void Time_Dispose_data(QByteArray buffer);

    void on_toolButton_farmone_clicked();

    void on_toolButton_farmtwo_clicked();

    void on_toolButton_farmthree_clicked();

    void on_toolButton_farmfour_clicked();

    void on_toolButton_farmfive_clicked();


private slots:

    void on_toolButton_Setdata_clicked(); //下发数据设置按钮

private:

    Ui::Myfarmer *ui;
    QTimer *Dispose_data;
    QTimer *Send_data;
    Farm_Data m_Farm_Data;


    int Relay_1_state;//接收到的继电器的状态值
    int Relay_2_state;
    int Relay_3_state;
    int Relay_4_state;
    int Relay_5_state;

    int Auto_1_state;//接收到的Auto的状态值
    int Auto_2_state;
    int Auto_3_state;
    int Auto_4_state;
    int Auto_5_state;



signals:
    void Start_set();
};

#endif // MYFARMER_H
