#include "excel_data.h"
#include "ui_excel_data.h"
QString insert_data="insert into table1 values(?,?,?,?,?,?,?,?,?,?,?,?)";
QString data_num="SELECT count(*) FROM table1";
void Excel_Data::Creatconnection()
{
//    Farmdb = QSqlDatabase::addDatabase("QMYSQL");
    Farmdb = QSqlDatabase::addDatabase("QSQLITE");
//    Farmdb.setDatabaseName("C:/Users/35166/Desktop/test_FLD1/test_FLD/FLD/famggber.db");       //这里输入你的数据库名
//    Farmdb.setHostName("localhost");
    QString name_str=QApplication::applicationDirPath()+"/famggber.db";
    Farmdb.setDatabaseName(name_str);       //这里输入你的数据库名
//    Farmdb.setUserName("root");
//    Farmdb.setPassword("123456");   //这里输入你的密码
    if(!Farmdb.open())
    {
        QMessageBox::warning(NULL,QObject::tr("错误！"),QObject::tr("检查文件的完整性，数据库无法使用！"));
        DB_sta=0x00;
    }else DB_sta=0x01;


}
void Excel_Data::ADD_Data_DB(Farm_Data* Get_Data)
{
        if(DB_sta)
        {
            QSqlQuery My_query(Farmdb);
            My_query.exec(data_num);
            My_query.next();
            Query_count=My_query.value(0).toInt();//得到数据库行数

            My_query.prepare(insert_data);//插入数据
            My_query.addBindValue(Query_count);
            My_query.addBindValue(Get_Data->date);
            My_query.addBindValue(Get_Data->Data_1);
            My_query.addBindValue(Get_Data->Data_2);
            My_query.addBindValue(Get_Data->Data_3);
            My_query.addBindValue(Get_Data->Data_4);
            My_query.addBindValue(Get_Data->Data_5);
            My_query.addBindValue(Get_Data->Data_6);
            My_query.addBindValue(Get_Data->con_1);
            My_query.addBindValue(Get_Data->con_2);
            My_query.addBindValue(Get_Data->con_3);
            My_query.addBindValue(Get_Data->sta);
            My_query.exec();


/******************第二种方法**********************/

//            while(DB_Model->canFetchMore())
//                DB_Model->fetchMore();
//            Get_Data->num=DB_Model->rowCount();
//            DB_Model->insertRows(Get_Data->num,1);
//            DB_Model->setData(DB_Model->index(Get_Data->num,0),Get_Data->num);
//            DB_Model->setData(DB_Model->index(Get_Data->num,1),Get_Data->date);
//            DB_Model->setData(DB_Model->index(Get_Data->num,2),Get_Data->Data_1);
//            DB_Model->setData(DB_Model->index(Get_Data->num,3),Get_Data->Data_2);
//            DB_Model->setData(DB_Model->index(Get_Data->num,4),Get_Data->Data_3);
//            DB_Model->setData(DB_Model->index(Get_Data->num,5),Get_Data->Data_4);
//            DB_Model->setData(DB_Model->index(Get_Data->num,6),Get_Data->Data_5);
//            DB_Model->setData(DB_Model->index(Get_Data->num,7),Get_Data->Data_6);
//            DB_Model->setData(DB_Model->index(Get_Data->num,8),Get_Data->con_1);
//            DB_Model->setData(DB_Model->index(Get_Data->num,9),Get_Data->con_2);
//            DB_Model->setData(DB_Model->index(Get_Data->num,10),Get_Data->con_3);
//            DB_Model->setData(DB_Model->index(Get_Data->num,11),Get_Data->sta);
//            DB_Model->submitAll();
//            ui->tableView_farm->verticalScrollBar()->setSliderPosition(Get_Data->num);
         }
}

Excel_Data::Excel_Data(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Excel_Data)
{
    ui->setupUi(this);

    Creatconnection();
    My_tableview = ui->tableView_farm;
    DB_Model= new QSqlTableModel(this);
    DB_Model->setTable("table1");
    DB_Model->select();
    DB_Model->setEditStrategy(QSqlTableModel::OnManualSubmit);//字段变化之后直接更新数据库

    ui->tableView_farm->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置为不可编辑
    QFont View_font;
    View_font.setBold(true);
//    View_font.setPointSize(11);
//    View_font.setFamily(QString("黑体"));
    ui->tableView_farm->horizontalHeader()->setFont(View_font);
//    ui->tableView_farm->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);//自适应列宽
    ui->tableView_farm->horizontalHeader()->setDefaultSectionSize(100);
    ui->tableView_farm->verticalHeader()->setVisible(false);
    ui->tableView_farm->setShowGrid(true);
    ui->tableView_farm->horizontalHeader()->setStyleSheet("QHeaderView::section{background:skyblue;}"); //设置表头背景色
    ui->tableView_farm->setStyleSheet("selection-background-color:lightblue;"); //设置选中背景色
    ui->tableView_farm->setModel(DB_Model);
    ui->tableView_farm->scrollToBottom();
    ui->tableView_farm->horizontalHeader()->resizeSection(0,50);
    ui->tableView_farm->horizontalHeader()->resizeSection(1,220);

    while(DB_Model->canFetchMore())
        DB_Model->fetchMore();
    ui->tableView_farm->verticalScrollBar()->setSliderPosition(DB_Model->rowCount());

}

Excel_Data::~Excel_Data()
{
    Farmdb.close();
    delete ui;
}



void Excel_Data::on_pushButton_update_clicked()
{
    DB_Model->select();
    while(DB_Model->canFetchMore())
        DB_Model->fetchMore();
    ui->tableView_farm->verticalScrollBar()->setSliderPosition(DB_Model->rowCount());
}
