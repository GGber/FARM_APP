#include "load_window.h"
#include "ui_load_window.h"

Load_window::Load_window(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Load_window)
{
    ui->setupUi(this);
    this->setWindowTitle("欢迎登录！");
    My_window = new MainWindow();
    ui->pushButton_load->setShortcut(Qt::Key_Enter);
    ui->lineEdit_user_name->setFocus();

}

Load_window::~Load_window()
{
    delete ui;
}

void Load_window::on_pushButton_load_clicked()
{
    if(ui->lineEdit_user_name->text().isEmpty())
    {
        QMessageBox::warning(NULL,"警告","输入账号为空！");
        return;
    }

    if(ui->lineEdit_user_name->text()==user_name)
    {
        if(ui->lineEdit_user_password->text().isEmpty())
        {
            QMessageBox::warning(NULL,"警告","输入密码为空！");
            return;
        }

        if(ui->lineEdit_user_password->text()==user_password)
        {
            My_window->show();
            this->close();
        }
        else {
            QMessageBox::warning(NULL,"警告","输入密码错误！");
            ui->lineEdit_user_password->clear();
            return;
        }
    }
    else {
         QMessageBox::warning(NULL,"警告","输入账号错误！");
         ui->lineEdit_user_name->clear();
         return;

    }


}
