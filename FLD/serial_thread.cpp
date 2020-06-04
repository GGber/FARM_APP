#include "serial_thread.h"
#define Str_number 1002   //数据长度

Serial_thread::Serial_thread()
{
}

void Serial_thread::socket_Read_Data()
{
     /*******************处理接收到的数据**********************/
    if(Myserial->bytesAvailable()>=Str_number)
    {
        Serial_thread_Readydata=1;
    }

}

void Serial_thread::Serial_thread_stop()
{
    Serial_thread_sta=0;
}

void Serial_thread::Serial_thread_start()
{
    Serial_thread_sta=1;
}

int Serial_thread::Serial_Init(QString name,qint32 Rate,QSerialPort::DataBits Databit,QSerialPort::Parity parity,QSerialPort::StopBits stopbit)
{
    Myserial->setPortName(name);
    Myserial->setBaudRate(Rate);
    Myserial->setDataBits(Databit);
    Myserial->setParity(parity);
    Myserial->setStopBits(stopbit);
    //打开串口
    if(!Myserial->open(QIODevice::ReadWrite))
    {
        QMessageBox::about(NULL, "提示", "无法打开串口！");
        return 0;
    }else {
        return 1;
}
}
void Serial_thread::run()
{
    Serial_thread_sta=1;

    Myserial = new QSerialPort();
    connect(Myserial,&QSerialPort::readyRead,this,&Serial_thread::socket_Read_Data);//来自串口新数据接收到槽函数

    while(Serial_thread_sta)
    {
        if(Serial_thread_Readydata)
        {
            buffer=Myserial->readAll();
            emit Recive_Data(1,buffer);
            Serial_thread_Readydata=0;
        }
        msleep(500);
    }
}
