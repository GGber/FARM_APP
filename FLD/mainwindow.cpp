#include "mainwindow.h"
#include "ui_mainwindow.h"

#define Str_number 1002   //数据长度
#define Update_time 1000   //更新频率 （ms）

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("主界面");
//    qDebug()<<"123:"+ QDir::currentPath()<<endl;

    Mytimer      = new QTimer();
    Label_time   = new QLabel();
    TCP_timer    = new QTimer();
    Data_timer   = new QTimer();
    Myserial     = new QSerialPort();

    My_Farmer    = ui->widget_Farmer;
    My_set       = My_Farmer->My_set;
    My_db_data   = ui->widget_2;
    My_Farmer->My_db_data = My_db_data;

    TCP_well_buf.resize(5);//定义水井分配帧大小为5
    TCP_well_buf.fill(0);

    ui->stackedWidget_main->setCurrentIndex(0);
    ui->tabWidget_set->setTabText(0,tr("水井分配"));
    ui->tabWidget_set->setTabText(1,tr("其他设置"));

    Send_str.resize(1002); //发送数组定义
    Send_str.fill(0);
    Send_str[0]='\xAA';
    Send_str[1001]='\xBB';

    //串口搜索数组标志初始化//date:200522
    for(uint8_t i =0;i<20;i++)
    {
        serial_exist_queue[i] =0;
    }

    connect(Mytimer,&QTimer::timeout,this,&MainWindow::Timer_update);           //时钟更新槽函数
    connect(Mytimer,&QTimer::timeout,this,&MainWindow::Serial_search_function); //serial编号更新槽函数
    connect(TCP_timer,&QTimer::timeout,this,&MainWindow::Rearch_TCP_Sta);       //检测TCP是否还在连接
    connect(Data_timer,&QTimer::timeout,this,&MainWindow::Time_send_ARM);       //定时发送给ARM值
    connect(My_set,&SetData::Data_set_OK,this,&MainWindow::Send_data_to_ARM);   //设置完成后调用该函数
    connect(Myserial,&QSerialPort::readyRead,this,&MainWindow::socket_Read_Data);//来自串口新数据接收到槽函数

    Mytimer->start(1000);//1s中断一次
    statusBar()->addPermanentWidget(Label_time);                    //时钟显示

}

MainWindow::~MainWindow()
{

    delete ui;
}

void MainWindow::Time_send_ARM()            //轮询发送数据到ARM
{
    if(Start_send)
    {
        switch (Send_var) {

            case 1 : Myserial->write(Send_str);break;   //数据读

            case 2 : Myserial->write(My_set->TCP_SendData); //数据写
                     Send_var=1;
                     My_set->Data_sta_clear();//清除状态位
        }
    }
}

void MainWindow::Send_data_to_ARM()//发送数据按钮
{
    if(Start_send)
    {
        Send_var=2;
    }

}

void MainWindow::on_pushButton_set_sure_clicked()//水井分配  设置完成按钮
{
    qDebug()<<TCP_well_buf<<endl;
    if(Start_send)
        /*******************发送数据**********************/
          Myserial->write(TCP_well_buf);
}



void MainWindow::Rearch_TCP_Sta()
{
    if(TCP_sta)
        TCP_sta=0;
    else
    {
        TCP_sta=0;
        socket_Disconnected();
        QMessageBox::warning(this,"超时警告",tr("连接超时，请检测设备通信"));
    }
}

void MainWindow::Timer_update()
{
    QDateTime Mytime_Now=QDateTime::currentDateTime();
    QString Time_str=Mytime_Now.toString("yyyy年MM月dd日  hh:mm:ss  dddd");
    Label_time->setText(Time_str);
    My_Farmer->Data_string=Time_str;
//    qDebug()<<tr("sta:")<<My_db_data.DB_sta<<endl

}

void MainWindow::socket_Disconnected()
{
    Myserial->close();
    ui->toolButton_connect->setText(tr("无信号"));
    ui->toolButton_connect->setIcon(QPixmap(":/source/pic/disconnet.png"));
    Start_send=0;//停止发送
    TCP_timer->stop();
    Data_timer->stop();

    qDebug() << "Disconnected!";
}

//Bug1:搜索的串口端字符串形如“COM8”，否则会存在问题，如：“com8","com1x5"等。字符串提取功能可进行优化
//Bug2:此函数只是搜索更细串口端容器的信息，需要做人性化处理：工作的串口锁定效果及释放功能，当串口进行插拔时，"选择接口"的对象发生变化！存在空白内容问题！
//Bug3:上位机确定后，请将输出函数qDebug()功能屏蔽
//Bug4:此函数搜索范围1~20
//BUg5:点击确认按钮时，"选择接口”相关内容会跳动！
#define COMNUM_MAX 20
#define COMBUG_ADJ 0
void MainWindow::Serial_search_function()
{
    //修：200522,Lmyy
    uint8_t exist_serial[COMNUM_MAX] ={0};
    QRegExp rx("\\d+");
    //遍历硬件上所有存在的串口信息
    foreach(const QSerialPortInfo &info,QSerialPortInfo::availablePorts())
    {
        //从字符串中提取数字
        QString curName =info.portName();
        int pos =0;
        int num =0;
#if  COMBUG_ADJ
        qDebug()<<"name:"<<curName;
#endif
        while((pos =rx.indexIn(curName,pos)) !=-1)
        {
            num =rx.cap(0).toInt();
            pos += rx.matchedLength();
#if  COMBUG_ADJ
            qDebug()<<"num:"<<num;
#endif
        }
        //对扫描存在的串口端进行标志！
        if((num>=0)&&(num<COMNUM_MAX))
        {
            exist_serial[num] =1;
        }
        else {
#if  COMBUG_ADJ
            qDebug()<<"serial number error!";
#endif
            return;
        }
        //判断容器中没有相应的串口端，并添加
        if((ui->comboBox_device->findText(info.portName())<0) && (serial_exist_queue[num] ==0))
        {
            ui->comboBox_device->addItem(info.portName());
            serial_exist_queue[num] =1;
        }
    }
    //遍历更新容器串口列表
    for(uint8_t i =0;i<COMNUM_MAX;i++)
    if((exist_serial[i] !=serial_exist_queue[i])&&(serial_exist_queue[i] ==1))
    {
        QString serialstr =QString("COM")+QString::number(i);
#if  COMBUG_ADJ
         qDebug()<<"serialstr:"<<serialstr;
#endif
        int index =ui->comboBox_device->findText(serialstr);
        if(index != -1)
        {
            ui->comboBox_device->removeItem(index);
            serial_exist_queue[i] =0;
#if  COMBUG_ADJ
            qDebug()<<"remove No."<<index;
#endif
        }
    }
}

void MainWindow::socket_Read_Data()
{
     /*******************处理接收到的数据**********************/
        buffer=Myserial->readAll();
        if(!buffer.isEmpty())
        {
            Get_str.append(buffer);
            if(buffer.contains('\xBB')&&(Get_str.size()>=Str_number))
            {
                My_Farmer->Time_Dispose_data(Get_str);
                TCP_sta=1;
//                qDebug()<<"大小为:"<<Get_str.size()<<Get_str.toHex()<<endl;
                Get_str.clear();
            }
        }
        buffer.clear();

}
void MainWindow::on_toolButton_connect_clicked()
{
    if(ui->toolButton_connect->text()==tr("无信号"))
        {

            if(ui->comboBox_device->currentText()==NULL)
                return;

            Myserial->setPortName(ui->comboBox_device->currentText());
            Myserial->setBaudRate(115200);
            Myserial->setDataBits(QSerialPort::Data8);
            Myserial->setParity(QSerialPort::NoParity);
            Myserial->setStopBits(QSerialPort::OneStop);
            //打开串口
            if(!Myserial->open(QIODevice::ReadWrite))
            {
                QMessageBox::about(NULL, "提示", "无法打开串口！");
                return;
            }

            ui->toolButton_connect->setText(tr("通信中！"));
            ui->toolButton_connect->setIcon(QPixmap(":/source/pic/connect.png"));

            TCP_timer->start(5000);
            Data_timer->start(Update_time);
            Start_send=1;//开始发送
            TCP_sta=1;
            qDebug()<<tr("successfully!");
        }
    else {
            socket_Disconnected();
        }
}

void MainWindow::on_toolButton_user_clicked()
{
    ui->stackedWidget_main->setCurrentIndex(0);
}

void MainWindow::on_toolButton_Manage_clicked()
{
     ui->stackedWidget_main->setCurrentIndex(1);
}

void MainWindow::on_toolButton_Analysis_clicked()
{
     ui->stackedWidget_main->setCurrentIndex(2);
}

void MainWindow::on_toolButton_set_clicked()
{
     ui->stackedWidget_main->setCurrentIndex(3);
}



void MainWindow::on_checkBox1_1_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[0]=TCP_well_buf[0]|1;
    else
        TCP_well_buf[0]=TCP_well_buf[0]&0xfe;
}

void MainWindow::on_checkBox1_2_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[0]=TCP_well_buf[0]|1<<1;
    else
        TCP_well_buf[0]=TCP_well_buf[0]&0xfd;
}

void MainWindow::on_checkBox1_3_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[0]=TCP_well_buf[0]|1<<2;
    else
        TCP_well_buf[0]=TCP_well_buf[0]&0xfb;
}

void MainWindow::on_checkBox1_4_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[0]=TCP_well_buf[0]|1<<3;
    else
        TCP_well_buf[0]=TCP_well_buf[0]&0xf7;
}

void MainWindow::on_checkBox1_5_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[0]=TCP_well_buf[0]|1<<4;
    else
        TCP_well_buf[0]=TCP_well_buf[0]&0xef;

    qDebug()<<TCP_well_buf<<endl;
}



void MainWindow::on_checkBox2_1_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[1]=TCP_well_buf[1]|1;
    else
        TCP_well_buf[1]=TCP_well_buf[1]&0xfe;
}

void MainWindow::on_checkBox2_2_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[1]=TCP_well_buf[1]|1<<1;
    else
        TCP_well_buf[1]=TCP_well_buf[1]&0xfd;
}

void MainWindow::on_checkBox2_3_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[1]=TCP_well_buf[1]|1<<2;
    else
        TCP_well_buf[1]=TCP_well_buf[1]&0xfb;
}

void MainWindow::on_checkBox2_4_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[1]=TCP_well_buf[1]|1<<3;
    else
        TCP_well_buf[1]=TCP_well_buf[1]&0xf7;
}

void MainWindow::on_checkBox2_5_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[1]=TCP_well_buf[1]|1<<4;
    else
        TCP_well_buf[1]=TCP_well_buf[1]&0xef;
}

void MainWindow::on_checkBox3_1_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[2]=TCP_well_buf[2]|1;
    else
        TCP_well_buf[2]=TCP_well_buf[2]&0xfe;
}

void MainWindow::on_checkBox3_2_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[2]=TCP_well_buf[2]|1<<1;
    else
        TCP_well_buf[2]=TCP_well_buf[2]&0xfd;
}

void MainWindow::on_checkBox3_3_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[2]=TCP_well_buf[2]|1<<2;
    else
        TCP_well_buf[2]=TCP_well_buf[2]&0xfb;
}

void MainWindow::on_checkBox3_4_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[2]=TCP_well_buf[2]|1<<3;
    else
        TCP_well_buf[2]=TCP_well_buf[2]&0xf7;
}

void MainWindow::on_checkBox3_5_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[2]=TCP_well_buf[2]|1<<4;
    else
        TCP_well_buf[2]=TCP_well_buf[2]&0xef;
}

void MainWindow::on_checkBox4_1_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[3]=TCP_well_buf[3]|1;
    else
        TCP_well_buf[3]=TCP_well_buf[3]&0xfe;
}

void MainWindow::on_checkBox4_2_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[3]=TCP_well_buf[3]|1<<1;
    else
        TCP_well_buf[3]=TCP_well_buf[3]&0xfd;
}

void MainWindow::on_checkBox4_3_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[3]=TCP_well_buf[3]|1<<2;
    else
        TCP_well_buf[3]=TCP_well_buf[3]&0xfb;
}

void MainWindow::on_checkBox4_4_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[3]=TCP_well_buf[3]|1<<3;
    else
        TCP_well_buf[3]=TCP_well_buf[3]&0xf7;
}

void MainWindow::on_checkBox4_5_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[3]=TCP_well_buf[3]|1<<4;
    else
        TCP_well_buf[3]=TCP_well_buf[3]&0xef;
}

void MainWindow::on_checkBox5_1_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[4]=TCP_well_buf[4]|1;
    else
        TCP_well_buf[4]=TCP_well_buf[4]&0xfe;
}

void MainWindow::on_checkBox5_2_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[4]=TCP_well_buf[4]|1<<1;
    else
        TCP_well_buf[4]=TCP_well_buf[4]&0xfd;
}

void MainWindow::on_checkBox5_3_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[4]=TCP_well_buf[4]|1<<2;
    else
        TCP_well_buf[4]=TCP_well_buf[4]&0xfb;
}

void MainWindow::on_checkBox5_4_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[4]=TCP_well_buf[4]|1<<3;
    else
        TCP_well_buf[4]=TCP_well_buf[4]&0xf7;
}

void MainWindow::on_checkBox5_5_stateChanged(int arg1)
{
    if(arg1)
        TCP_well_buf[4]=TCP_well_buf[4]|1<<4;
    else
        TCP_well_buf[4]=TCP_well_buf[4]&0xef;
}




