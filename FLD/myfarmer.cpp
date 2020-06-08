#include "myfarmer.h"
#include "ui_myfarmer.h"


Myfarmer::Myfarmer(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Myfarmer)
{
    ui->setupUi(this);
    My_set = new SetData();
    My_set->hide();

    ui->stackedWidget_farm->setCurrentIndex(0);
    ui->label_Auto_1->setStyleSheet(QString("color: rgb(255, 0, 0);"));
    ui->label_Auto_2->setStyleSheet(QString("color: rgb(255, 0, 0);"));
    ui->label_Auto_3->setStyleSheet(QString("color: rgb(255, 0, 0);"));
    ui->label_Auto_4->setStyleSheet(QString("color: rgb(255, 0, 0);"));
    ui->label_Auto_5->setStyleSheet(QString("color: rgb(255, 0, 0);"));

    connect(this,&Myfarmer::Start_set,My_set,&SetData::Change_Farm_Number);//改变设置最上方的农田编号

}

Myfarmer::~Myfarmer()
{
    delete ui;
}

static int Char_to_data(char *ptr)
{
    quint8 Data_H,Data_L;
    int Data,R_data;
    Data_H=(quint8)*(ptr+1);
    Data_L=(quint8)*ptr;
    Data=(Data_H<<8)|Data_L;
    R_data=Data/9;
    if(Data&0xF000)
        return 0;
    else
        return R_data;
}

static int Char_to_data_2(char *ptr)
{
    quint8 Data_H,Data_L;
    int Data,R_data;
    Data_H=(quint8)*(ptr+1);
    Data_L=(quint8)*ptr;
    Data=(Data_H<<8)|Data_L;
    return Data;

}

void Myfarmer::Time_Dispose_data(QByteArray buffer)
{

    char *Deal_data;      //定义需要处理的数据
    Deal_data = buffer.data();

//    for(int i;i<1002;i++)
//        qDebug("%d:%02x\r",i,buffer.at(i));

    if(Deal_data[0]!='\xAA'||Deal_data[1001]!='\xBB')
        return;

    m_Farm_Data.date=Data_string;

    /********************1号田数据更新*********************/
    ui->lineEdit_one_1->setText(QString::number(Char_to_data(&Deal_data[3])));//数据1
    ui->lineEdit_one_2->setText(QString::number(Char_to_data(&Deal_data[5])));//数据2
    ui->lineEdit_one_3->setText(QString::number(Char_to_data(&Deal_data[7])));//数据3

    ui->lineEdit_one_4->setText(QString::number(Char_to_data(&Deal_data[19])));//数据4
    ui->lineEdit_one_5->setText(QString::number(Char_to_data(&Deal_data[21])));//数据5
    ui->lineEdit_one_6->setText(QString::number(Char_to_data(&Deal_data[23])));//数据6

    ui->lineEdit_one_7->setText(QString::number(Char_to_data(&Deal_data[35])));//数据7
    ui->lineEdit_one_8->setText(QString::number(Char_to_data(&Deal_data[37])));;//数据8
    ui->lineEdit_one_9->setText(QString::number(Char_to_data(&Deal_data[39])));//数据9

//      m_Farm_Data.Data_1="132";
    m_Farm_Data.Data_1=ui->lineEdit_one_1->text();
    m_Farm_Data.Data_2=ui->lineEdit_one_2->text();
    m_Farm_Data.Data_3=ui->lineEdit_one_3->text();
    m_Farm_Data.Data_4=ui->lineEdit_one_4->text();
    m_Farm_Data.Data_5=ui->lineEdit_one_5->text();
    m_Farm_Data.Data_6=ui->lineEdit_one_6->text();


    Auto_1_state=Char_to_data_2(&Deal_data[161]);//Auto状态
//    qDebug("%x",Auto_1_state);
    switch(Auto_1_state)
    {
    case 1: ui->label_Auto_1->setText("手动模式中");m_Farm_Data.sta=tr("手动模式");break;
    case 2: ui->label_Auto_1->setText("自动模式中");m_Farm_Data.sta=tr("自动模式");break;
    default: break;

    }

    ui->lineEdit_one_set_1->setText(QString::number(Char_to_data(&Deal_data[165])));//阈值1
    ui->lineEdit_one_set_2->setText(QString::number(Char_to_data(&Deal_data[167])));//阈值2
    ui->lineEdit_one_set_3->setText(QString::number(Char_to_data(&Deal_data[169])));//阈值3

    Relay_1_state=Char_to_data_2(&Deal_data[179]);//继电器状态
//    qDebug("%x",Relay_1_state);
    if(Relay_1_state&1<<0) {ui->label_farmone_LED1->setPixmap(QPixmap(":/source/pic/light_on.gif"));    m_Farm_Data.con_1=tr("打开");} else {ui->label_farmone_LED1->setPixmap(QPixmap(":/source/pic/light_off.gif"));    m_Farm_Data.con_1=tr("关闭");}
    if(Relay_1_state&1<<1) {ui->label_farmone_LED2->setPixmap(QPixmap(":/source/pic/light_on.gif"));     m_Farm_Data.con_2=tr("打开");} else {ui->label_farmone_LED2->setPixmap(QPixmap(":/source/pic/light_off.gif"));    m_Farm_Data.con_2=tr("关闭");}
    if(Relay_1_state&1<<2) {ui->label_farmone_LED3->setPixmap(QPixmap(":/source/pic/light_on.gif"));     m_Farm_Data.con_3=tr("打开");} else {ui->label_farmone_LED3->setPixmap(QPixmap(":/source/pic/light_off.gif"));    m_Farm_Data.con_3=tr("关闭");}

    /**以上共200个字节**/

    /********************2号田数据更新*********************/
    /****************************************************/
    ui->lineEdit_two_1->setText(QString::number(Char_to_data(&Deal_data[203])));//数据1
    ui->lineEdit_two_2->setText(QString::number(Char_to_data(&Deal_data[205])));//数据2
    ui->lineEdit_two_3->setText(QString::number(Char_to_data(&Deal_data[207])));//数据3

    ui->lineEdit_two_4->setText(QString::number(Char_to_data(&Deal_data[219])));//数据4
    ui->lineEdit_two_5->setText(QString::number(Char_to_data(&Deal_data[221])));//数据5
    ui->lineEdit_two_6->setText(QString::number(Char_to_data(&Deal_data[223])));//数据6

    ui->lineEdit_two_7->setText(QString::number(Char_to_data(&Deal_data[235])));//数据7
    ui->lineEdit_two_8->setText(QString::number(Char_to_data(&Deal_data[237])));//数据8
    ui->lineEdit_two_9->setText(QString::number(Char_to_data(&Deal_data[239])));//数据9

    Auto_2_state=Char_to_data(&Deal_data[361]);//Auto状态
    if(Auto_2_state) ui->label_Auto_2->setText("自动模式中"); else ui->label_Auto_2->setText("手动模式中");

    ui->lineEdit_two_set_1->setText(QString::number(Char_to_data(&Deal_data[363])));//阈值1
    ui->lineEdit_two_set_2->setText(QString::number(Char_to_data(&Deal_data[365])));//阈值2
    ui->lineEdit_two_set_3->setText(QString::number(Char_to_data(&Deal_data[367])));//阈值3

    Relay_2_state=Char_to_data(&Deal_data[379]);//继电器状态
    if(Relay_2_state&1<<0) ui->label_farmtwo_LED1->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmtwo_LED1->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_2_state&1<<1) ui->label_farmtwo_LED2->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmtwo_LED2->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_2_state&1<<2) ui->label_farmtwo_LED3->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmtwo_LED3->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    /**以上共200个字节**/
    /********************3号田数据更新*********************/
    /****************************************************/
    ui->lineEdit_three_1->setText(QString::number(Char_to_data(&Deal_data[403])));//数据1
    ui->        lineEdit_three_2->setText(QString::number(Char_to_data(&Deal_data[405])));//数据2
    ui->lineEdit_three_3->setText(QString::number(Char_to_data(&Deal_data[407])));//数据3

    ui->lineEdit_three_4->setText(QString::number(Char_to_data(&Deal_data[419])));//数据4
    ui->lineEdit_three_5->setText(QString::number(Char_to_data(&Deal_data[421])));//数据5
    ui->lineEdit_three_6->setText(QString::number(Char_to_data(&Deal_data[423])));//数据6

    ui->lineEdit_three_7->setText(QString::number(Char_to_data(&Deal_data[435])));//数据7
    ui->lineEdit_three_8->setText(QString::number(Char_to_data(&Deal_data[437])));//数据8
    ui->lineEdit_three_9->setText(QString::number(Char_to_data(&Deal_data[439])));//数据9

    Auto_3_state=Char_to_data(&Deal_data[561]);//Auto状态
    if(Auto_3_state) ui->label_Auto_3->setText("自动模式中"); else ui->label_Auto_3->setText("手动模式中");

    ui->lineEdit_three_set_1->setText(QString::number(Char_to_data(&Deal_data[563])));//阈值1
    ui->lineEdit_three_set_2->setText(QString::number(Char_to_data(&Deal_data[565])));//阈值2
    ui->lineEdit_three_set_3->setText(QString::number(Char_to_data(&Deal_data[567])));//阈值3

    Relay_3_state=Char_to_data(&Deal_data[579]);//继电器状态
    if(Relay_3_state&1<<0) ui->label_farmthree_LED1->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmthree_LED1->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_3_state&1<<1) ui->label_farmthree_LED2->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmthree_LED2->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_3_state&1<<2) ui->label_farmthree_LED3->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmthree_LED3->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    /**以上共200个字节**/

    /********************4号田数据更新*********************/
    /****************************************************/
    ui->lineEdit_four_1->setText(QString::number(Char_to_data(&Deal_data[603])));//数据1
    ui->lineEdit_four_2->setText(QString::number(Char_to_data(&Deal_data[605])));//数据2
    ui->lineEdit_four_3->setText(QString::number(Char_to_data(&Deal_data[607])));//数据3

    ui->lineEdit_four_4->setText(QString::number(Char_to_data(&Deal_data[619])));//数据4
    ui->lineEdit_four_5->setText(QString::number(Char_to_data(&Deal_data[621])));//数据5
    ui->lineEdit_four_6->setText(QString::number(Char_to_data(&Deal_data[623])));//数据6

    ui->lineEdit_four_7->setText(QString::number(Char_to_data(&Deal_data[635])));//数据7
    ui->lineEdit_four_8->setText(QString::number(Char_to_data(&Deal_data[637])));//数据8
    ui->lineEdit_four_9->setText(QString::number(Char_to_data(&Deal_data[639])));//数据9

    Auto_4_state=Char_to_data(&Deal_data[761]);//Auto状态
    if(Auto_4_state) ui->label_Auto_4->setText("自动模式中"); else ui->label_Auto_4->setText("手动模式中");

    ui->lineEdit_four_set_1->setText(QString::number(Char_to_data(&Deal_data[763])));//阈值1
    ui->lineEdit_four_set_2->setText(QString::number(Char_to_data(&Deal_data[765])));//阈值2
    ui->lineEdit_four_set_3->setText(QString::number(Char_to_data(&Deal_data[767])));//阈值3

    Relay_4_state=Char_to_data(&Deal_data[779]);//继电器状态
    if(Relay_4_state&1<<0) ui->label_farmfour_LED1->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmfour_LED1->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_4_state&1<<1) ui->label_farmfour_LED2->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmfour_LED2->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_4_state&1<<2) ui->label_farmfour_LED3->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmfour_LED3->setPixmap(QPixmap(":/source/pic/light_off.gif"));

    /**以上共200个字节**/

    /********************5号田数据更新*********************/
    /****************************************************/
    ui->lineEdit_five_1->setText(QString::number(Char_to_data(&Deal_data[803])));//数据1
    ui->lineEdit_five_2->setText(QString::number(Char_to_data(&Deal_data[805])));//数据2
    ui->lineEdit_five_3->setText(QString::number(Char_to_data(&Deal_data[807])));//数据3

    ui->lineEdit_five_4->setText(QString::number(Char_to_data(&Deal_data[819])));//数据4
    ui->lineEdit_five_5->setText(QString::number(Char_to_data(&Deal_data[821])));//数据5
    ui->lineEdit_five_6->setText(QString::number(Char_to_data(&Deal_data[823])));//数据6

    ui->lineEdit_five_7->setText(QString::number(Char_to_data(&Deal_data[835])));//数据7
    ui->lineEdit_five_8->setText(QString::number(Char_to_data(&Deal_data[837])));//数据8
    ui->lineEdit_five_9->setText(QString::number(Char_to_data(&Deal_data[839])));//数据9

    Auto_5_state=Char_to_data(&Deal_data[961]);//Auto状态
    if(Auto_5_state) ui->label_Auto_5->setText("自动模式中"); else ui->label_Auto_5->setText("手动模式中");

    ui->lineEdit_five_set_1->setText(QString::number(Char_to_data(&Deal_data[963])));//阈值1
    ui->lineEdit_five_set_2->setText(QString::number(Char_to_data(&Deal_data[965])));//阈值2
    ui->lineEdit_five_set_3->setText(QString::number(Char_to_data(&Deal_data[967])));//阈值3

    Relay_5_state=Char_to_data(&Deal_data[979]);;//继电器状态
    if(Relay_5_state&1<<0) ui->label_farmfive_LED1->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmfive_LED1->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_5_state&1<<1) ui->label_farmfive_LED2->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmfive_LED2->setPixmap(QPixmap(":/source/pic/light_off.gif"));
    if(Relay_5_state&1<<2) ui->label_farmfive_LED3->setPixmap(QPixmap(":/source/pic/light_on.gif")); else ui->label_farmfive_LED3->setPixmap(QPixmap(":/source/pic/light_off.gif"));

    /**以上共200个字节**/

    /********************数据库更新**************************/
    if(My_db_data->DB_sta)
    {
        My_db_data->ADD_Data_DB(&m_Farm_Data);//添加数据
//        My_db_data->ui->spinBox_page->setValue(My_db_data->Page_Count());

    }


}


void Myfarmer::on_toolButton_farmone_clicked()
{
    ui->stackedWidget_farm->setCurrentIndex(0);
    My_set->Farm_Number=0;
}

void Myfarmer::on_toolButton_farmtwo_clicked()
{
    ui->stackedWidget_farm->setCurrentIndex(1);
    My_set->Farm_Number=1;
}

void Myfarmer::on_toolButton_farmthree_clicked()
{
    ui->stackedWidget_farm->setCurrentIndex(2);
    My_set->Farm_Number=2;
}

void Myfarmer::on_toolButton_farmfour_clicked()
{
    ui->stackedWidget_farm->setCurrentIndex(3);
    My_set->Farm_Number=3;
}

void Myfarmer::on_toolButton_farmfive_clicked()
{
    ui->stackedWidget_farm->setCurrentIndex(4);
    My_set->Farm_Number=4;
}


void Myfarmer::on_toolButton_Setdata_clicked()//设置当前田的初始值
{
    emit Start_set();
    My_set->show();
}
