#ifndef SERIAL_THREAD_H
#define SERIAL_THREAD_H
#include <QThread>
#include <QSerialPort>        //提供访问串口的功能
#include <QSerialPortInfo>    //提供系统中存在的串口的信息
#include <QTimer>
#include <QMessageBox>

class Serial_thread : public QThread
{
    Q_OBJECT
public:
    Serial_thread();
    QByteArray buffer;
    QSerialPort *Myserial;

protected:
    void run() Q_DECL_OVERRIDE;

private:
   int Serial_thread_sta;
   int Serial_thread_Readydata;

private slots:
    void socket_Read_Data();

public slots:
    void Serial_thread_stop();
    void Serial_thread_start();
    int Serial_Init(QString name,qint32 Rate,QSerialPort::DataBits Databit,QSerialPort::Parity parity,QSerialPort::StopBits stopbit);

signals:
    void Recive_Data(int sta, QByteArray buffer);
};

#endif // SERIAL_THREAD_H
