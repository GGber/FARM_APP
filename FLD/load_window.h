#ifndef LOAD_WINDOW_H
#define LOAD_WINDOW_H

#include <QWidget>
#include "mainwindow.h"
#include <QMessageBox>

namespace Ui {
class Load_window;
}

class Load_window : public QWidget
{
    Q_OBJECT

public:
    explicit Load_window(QWidget *parent = nullptr);
    ~Load_window();

private slots:
    void on_pushButton_load_clicked();

private:
    Ui::Load_window *ui;

    MainWindow *My_window;

    QString user_name="admin";
    QString user_password="123";

};

#endif // LOAD_WINDOW_H
