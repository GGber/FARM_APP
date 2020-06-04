#include "mainwindow.h"
#include "load_window.h"

#include <QApplication>



int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainWindow w;
//    Load_window w;
    w.show();

    return a.exec();
}
