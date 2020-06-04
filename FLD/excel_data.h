#ifndef EXCEL_DATA_H
#define EXCEL_DATA_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>
#include <QTableView>
#include <QScrollBar>

struct Farm_Data{
    int num;
    QString date;
    QString Data_1;
    QString Data_2;
    QString Data_3;
    QString Data_4;
    QString Data_5;
    QString Data_6;
    QString con_1;
    QString con_2;
    QString con_3;
    QString sta;
};

namespace Ui {
class Excel_Data;
}

class Excel_Data : public QWidget
{
    Q_OBJECT

public:
    explicit Excel_Data(QWidget *parent = nullptr);
    ~Excel_Data();

     Ui::Excel_Data *ui;

    QSqlTableModel *DB_Model;

    int DB_sta=0;

public slots:

    void ADD_Data_DB(Farm_Data*);

private:

    QSqlDatabase Farmdb;
    QTableView *My_tableview;
    int Query_count;    //数据库行数

private slots:

    void Creatconnection();
    void on_pushButton_update_clicked();
};


#endif // EXCEL_DATA_H
