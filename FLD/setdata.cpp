#include "setdata.h"
#include "ui_setdata.h"

SetData::SetData(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SetData)
{
    ui->setupUi(this);
    this->resize(250,300);
    this->setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    this->setWindowTitle("农田属性设置");

    TCP_SendData.resize(1002);//定义数组大小为1002
    TCP_SendData.fill(0);

    TCP_SendData[0]='\xAA';
    TCP_SendData[1001]='\xCC';

//    ui->radioButton_state_no->setCheckable(true);
}

SetData::~SetData()
{
    delete ui;
}


void SetData::on_pushButton_Sure_clicked()
{

    Farm_Number=ui->comboBox_Number->currentIndex();//得到当前的田编号

    if(ui->checkBox_Contrl_1->isChecked())  Relay_Send|=1;    else Relay_Send&=0xfffe;//获取继电器值
    if(ui->checkBox_Contrl_2->isChecked())  Relay_Send|=1<<1; else Relay_Send&=0xfffd;
    if(ui->checkBox_Contrl_3->isChecked())  Relay_Send|=1<<2; else Relay_Send&=0xfffb;

    if(ui->radioButton_hand->isChecked()) Auto_Send=1;                  //获取自动或者手动模式
    if(ui->radioButton_Auto->isChecked()) Auto_Send=2;

    if(ui->radioButton_state_yes->isChecked()) State_Send=0x5555;
    if(ui->radioButton_state_no->isChecked()) State_Send=0;

    TCP_SendData[Farm_Number*200+162]=((Auto_Send>>8)&0xff);                 //模式控制
    TCP_SendData[Farm_Number*200+161]=Auto_Send&0xff;

    TCP_SendData[Farm_Number*200+164]=(ui->spinBox_Set_1->value()>>8)&0xff;//阈值1
    TCP_SendData[Farm_Number*200+163]=ui->spinBox_Set_1->value()&0xff;
    TCP_SendData[Farm_Number*200+166]=(ui->spinBox_Set_2->value()>>8)&0xff;//阈值2
    TCP_SendData[Farm_Number*200+165]=ui->spinBox_Set_2->value()&0xff;
    TCP_SendData[Farm_Number*200+168]=(ui->spinBox_Set_3->value()>>8)&0xff;//阈值3
    TCP_SendData[Farm_Number*200+167]=ui->spinBox_Set_3->value()&0xff;

    TCP_SendData[Farm_Number*200+180]=(Relay_Send>>8)&0xff;//继电器写入值
    TCP_SendData[Farm_Number*200+179]=Relay_Send&0xff;

    TCP_SendData[Farm_Number*200+186]=(State_Send>>8)&0xff;               //状态查询写入值
    TCP_SendData[Farm_Number*200+185]=State_Send&0xff;
    TCP_SendData[Farm_Number*200+188]=(ui->spinBox_add_number->value()>>8)&0xff;//添加的新板查询写入值
    TCP_SendData[Farm_Number*200+187]=ui->spinBox_add_number->value()&0xff;

//    qDebug()<<TCP_SendData<<endl;
    emit Data_set_OK();

    this->hide();

}

void SetData::Data_sta_clear()
{
    Relay_Send=0;
    State_Send=0;
    Auto_Send=0;
}

void SetData::on_pushButton_Cancel_clicked()
{
    this->hide();
}


void SetData::Change_Farm_Number()
{
    ui->comboBox_Number->setCurrentIndex(Farm_Number);
    ui->radioButton_hand->setChecked(true);
}

void SetData::on_checkBox_Contrl_1_stateChanged(int arg1)
{
    Relay_Send|=0xf000;
}

void SetData::on_checkBox_Contrl_2_stateChanged(int arg1)
{
     Relay_Send|=0xf000;
}

void SetData::on_checkBox_Contrl_3_stateChanged(int arg1)
{
     Relay_Send|=0xf000;
}
