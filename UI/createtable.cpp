#include "createtable.h"
#include "ui_createtable.h"


CreateTable::CreateTable(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CreateTable)
{
    ui->setupUi(this);
    //qDebug()<<"in";
    ui->tableName->setPlaceholderText("enter new table's' name..");
    ui->columns->setPlaceholderText("\"Specify column_name column_type pairs separated by ',' \"\n"
                                 "ex.:\n"
                                 "column1_name column1_type,\n"
                                 "column2_name column2_type,\n"
                                 "...");
}

CreateTable::~CreateTable()
{
    delete ui;
}

void CreateTable::on_buttonBox_accepted()
{
    /**
     * Creating table proccessing
     *
     * Reads value from text edits for
     * tableName and columns configuration
     * and executes 'CREATE TABLE..' statement
     * at the and emits done() signal
     *
     * @return void
     */
    QString tableName;
    QStringList tableParams;
    tableName = ui->tableName->text();
    //qDebug() <<"tableName:"<< tableName<<endl;
    tableParams = ui->columns->toPlainText().split('\n');
    //qDebug() <<"tableSize:"<< tableParams.size()<<endl;
    if(!tableName.isEmpty() && !tableParams.isEmpty())
    {
        //qDebug() <<"tableSize:"<< tableParams.size()<<endl;
        //for (int i = 0 ; i < tableParams.size() ; i++){
            //qDebug() << tableParams<<endl;
        //}
        emit done(tableName, tableParams);
    }
}

void CreateTable::on_buttonBox_rejected()
{

}
