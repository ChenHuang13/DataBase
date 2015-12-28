#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QFile>
#include <QMessageBox>
#include <QTextStream>
#include <QDebug>
#include <QTableView>
#include <common.h>
#include "QStandardItemModel"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    buttonEnable(false);
    databaseManager.init();
    databaseManager.loadDataBase();
}

MainWindow::~MainWindow()
{
    delete ui;
    databaseManager.quit();
    closeConnection();
}

void MainWindow::on_OpenDatabase_clicked(){
    //   QString fileName = QFileDialog::getOpenFileName(this, tr("Open Database"), QString(),  tr("All Files (*.*);;Database Files (*.sqlite *.sqlite3 *.db)"));
    bool isOK;
    QString fileName = QInputDialog::getText( NULL,"OPEN","Please input database name", QLineEdit::Normal,"",&isOK);

    if( ! isOK ) {
        return;
    }

    if ( ! fileName.isEmpty() ) {
        char*  ch;
        QByteArray ba = fileName.toLatin1();
        ch = ba.data();
       if (databaseManager.systemManager.usedb(  ch )  ){
           ui->status->setText("Database has been opened");
           updateComboBox();
           showTable(ui->comboBox->currentText());
           buttonEnable(true);
           currentDataBase = fileName;
           setWindowTitle(currentDataBase);
       }
       else{
           ui->status->setText("Open Database failed!");
           buttonEnable(false);
       }
    }
}


void MainWindow::on_NewDatabase_clicked(){
    /**
     * Opening existing database
     *
     * On clicked event save dialog is called
     * to get the new database full name,
     * after that new file and database connection creates,
     * in case of success cobmoBox and teble
     * view are set to display corresponding information
     * which in empty
     *
     * @return void
     */

    //QString fileName = QFileDialog::getSaveFileName(this, tr("New Database"), QString(), tr("All Files (*.*);;Database Files (*.sqlite *.sqlite3 *.db)"));
    bool isOK;
    QString fileName = QInputDialog::getText( NULL,"CREATE","Please input database name", QLineEdit::Normal,"",&isOK);

    if( ! isOK ) {
        return;
    }

    if (!fileName.isEmpty())
    {
        char*  ch;
        QByteArray ba = fileName.toLatin1();
        ch = ba.data();
        if (databaseManager.systemManager.createdb(ch)){
            if(databaseManager.systemManager.usedb(  ch )  ){
                ui->status->setText("Database has been created");
                updateComboBox();
                showTable(ui->comboBox->currentText());
                buttonEnable(true);
                currentDataBase = fileName;
                setWindowTitle(currentDataBase);
            }
            else{
                ui->status->setText("Database open failed!");
            }

        }
        else{
            ui->status->setText("Create Database failed!");
            buttonEnable(false);
        }
//        QFile file(fileName);
//        if (!file.open(QIODevice::WriteOnly))
//        {
//            qDebug() << "Imposible to create database!";
//            ui->status->setText("Impossible to create new database!");
//        }
//        else
//        {
//            file.close();
//            if (!createConnection(fileName))
//                ui->status->setText("Impossible to open database");
//            else
//            {
//                ui->status->setText("Database has been created");
//                updateComboBox();
//                showTable(ui->comboBox->currentText());
//            }
//        }

    }
}

void MainWindow::on_comboBox_activated(const QString &arg1)
{
    /**
     * Refreshing tableView
     *
     * Refreshing tableView on correspondent
     * intem from comboBox
     *
     * @return void
     */
    qDebug() << arg1;

    showTable(arg1);
    ui->status->setText("Table '"+arg1+"' displayed");
}

bool MainWindow::createConnection(QString databaseName)
{
    /**
     * Connection to database function
     *
     * Before setting up and openning new connection
     * checks for existing one and close it,
     * after that setting up new database name
     * and tries to connect ot it
     *
     * @databaseName QString full name of database file
     *
     * @return bool true on success, false othervise
     */
    if (db.isOpen())
    {
        db.close();
        //qDebug() << "+1";
    }

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(databaseName);


    if (!db.open())
    {
        qDebug() << "Can't open database: " << db.lastError().text();
        return false;
    }

    query = new QSqlQuery;
    QStringList lst = db.tables();

    foreach (QString str, lst)
    {
        qDebug() << "Table: " << str;
    }

    buttonEnable(true);
    return true;
}

void MainWindow::closeConnection()
{
    /**
     * Closing connectins
     *
     * Clsosing connection to opened database
     * set buttons connectet to database proccesing
     * disabled
     *
     * @return void
     */

    buttonEnable(false);
    db.close();
    db.removeDatabase(QSqlDatabase::defaultConnection);
}

void MainWindow::updateComboBox()
{
    /**
     * Updating comboBox
     *
     * Updating comboBox content on
     * database structure change or
     * after establishing connection
     * Should be used in corresponding slots
     *
     * @return void
     */
    ui->comboBox->clear();

    QStringList lst = cdbs->getTableList();//db.tables();
    qDebug() << lst.size();
    foreach (QString str, lst)
    {
        qDebug()<<str;
        ui->comboBox->addItem(str);
    }
}

void MainWindow::showTable(QString tableName)
{
    /**
     * Displaying table
     *
     * Displaying selected table from current database
     * in tableView with editStrategy OnFieldChange
     *
     * @tableName QString name of table to be displayed
     *
     * @return void
     */

    char*  ch;
    QByteArray ba = tableName.toLatin1();
    ch = ba.data();

    QStandardItemModel *model = new QStandardItemModel();

    QStringList headerList;// = getHeaderList(tableName);

    TableInfo* tb = cdbs->getTable(ch);


    if (tb == NULL) {
        printf("error: no table\n");
        return;
    }

    int kk = tb->cn;
    for (int i = 0; i < kk; ++ i) {
        headerList.append( tb->col[i].name );
    }

    int colLength  = headerList.length();
    for (int i = 0 ; i < colLength ; i++){
        model->setHorizontalHeaderItem(i, new QStandardItem( headerList[i]  ));
    }

    QList <QStringList> dataList = QueryManager::getDataList(tableName);
    for (int i = 0 ; i < dataList .length(); i++){
        for (int j = 0 ; j < colLength ; j++){
            model->setItem(i, j, new QStandardItem(dataList[i][j]));
        }
    }

    //利用setModel()方法将数据模型与QTableView绑定
    ui->tableView->setModel(model);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->resizeColumnsToContents();
}

void MainWindow::on_pushButtonNewRecord_clicked()
{
    /**
     * Inserting new record to table
     *
     * Inserting new record to current
     * table selected in comboBox
     * through executing INSERT query
     * with default values and refreshing
     * tableView calling showTable()
     *
     * @return void
     */
    if(db.isOpen())
    {
        if (!query->exec("INSERT INTO "+ui->comboBox->currentText()+" DEFAULT VALUES"))
        {
            qDebug() << "INSERT INTO "+ui->comboBox->currentText()+" DEFAULT VALUES";
            qDebug() << "Insertion error: " << query->lastError().text();
        }

        showTable(ui->comboBox->currentText());
        ui->status->setText("New record inserted");
    }
    else
        ui->status->setText("No database connection!");
}

void MainWindow::on_pushButtonDeleteRecord_clicked()
{
    /**
     * Deleting selected record(s) from table
     *
     * Deleting selected record(s) from current
     * table selected in comboBox
     * through QSqlTableModel interface
     * and refreshing tableView calling showTable()
     *
     * @return void
     */
    if(db.isOpen())
    {
        QSqlTableModel * model = new QSqlTableModel();

        model->setTable(ui->comboBox->currentText());

        model->select();

        model->setEditStrategy(QSqlTableModel::OnFieldChange);
        QModelIndexList selected = ui->tableView->selectionModel()->selectedIndexes();

        for (int i = 0; i < selected.size(); ++i)
            model->removeRows(selected.at(i).row(), 1);

        if(!model->submitAll())
        {
            qDebug() << "Insertion error!";
        }

        showTable(ui->comboBox->currentText());
        ui->status->setText("Record(s) deleted");
    }
    else
        ui->status->setText("No database connection!");

}

void MainWindow::on_CreateTable_clicked()
{
    /**
     * Creating new table dialog
     *
     * Initializing create new table dialog
     * in case database connection is established
     * and looking for done() signal to refresh
     * tableView and comboBox contnet after
     * it finished
     *
     * @return void
     */
//    if(db.isOpen())
//    {
//        qDebug() << "creating table..";
        tableCre = new CreateTable(0);
        connect(tableCre, SIGNAL(done( QString,QStringList )), this, SLOT(onTableCreate(QString,QStringList)));
        tableCre->show();
//    }
     //ui->status->setText("No database connection!");
}

void MainWindow::on_DeleteTable_clicked()
{
    /**
     * Deleting table dialog
     *
     * Initializing delete table dialog on
     * current table selected in comboBox
     * in case database connection is established
     * and looking for done() signal to refresh
     * tableView and comboBox contnet after
     * it finished
     *
     * @return void
     */
//    if (db.isOpen())
//    {
//        qDebug() << "deleting table..";
//        tableDel = new deleteTable(0, query, ui->comboBox->currentText());
//        connect(tableDel, SIGNAL(done()), this, SLOT(onTableDelete()));
//        tableDel->show();
//    }
//    else
//        ui->status->setText("No database connection!");
        QString str = ui->comboBox->currentText();
        char*  ch;
        QByteArray ba = str.toLatin1();
        ch = ba.data();
        cdbs->dropTB(ch);
        updateComboBox();
}

void MainWindow::onTableCreate(QString name , QStringList params)
{
    /**
     * Action after tablecreate dialog confirmed
     *
     * Updates comboBox content and refresh
     * tebleView
     *
     * @return void
     */
    state = START;
    QString order = "CREATE TABLE "+name+" (";
    char*  ch;
    QByteArray ba = order.toLatin1();
    ch = ba.data();
    databaseManager.execute(ch);
    for (int i = 0 ; i < params.size(); i++){
        order = params[i];
        if (i != params.size()-1) order += ",";
         ba = order.toLatin1();
         ch = ba.data();
         databaseManager.execute(ch);
    }
    order = ");";
    ba = order.toLatin1();
    ch = ba.data();
    databaseManager.execute(ch);

    updateComboBox();
    showTable(ui->comboBox->currentText());
    ui->status->setText("Table created");
}

void MainWindow::onTableDelete()
{
    /**
     * Action after tableDelete dialog confirmed
     *
     * Updates comboBox content and refresh
     * tebleView
     *
     * @return void
     */
    updateComboBox();
    showTable(ui->comboBox->currentText());
    ui->status->setText("Table deleted");
}

void MainWindow::buttonEnable(bool flag)
{
    /**
     * Enebling/Disablin buttons
     *
     * Enebling/Disabling database proccesing buttons
     * base on input flag
     * Should be callde with 'false' value when database
     * connection is not exists and with 'true' after
     * establishing connection
     *
     * @flag bool Enable/Disable flag
     *
     * @return void
     */
    ui->CreateTable->setEnabled(flag);
    ui->DeleteTable->setEnabled(flag);
    ui->excuteSQL->setEnabled(flag);
    //ui->pushButtonDeleteRecord->setEnabled(flag);
}

void MainWindow::on_excuteSQL_clicked()
{
    bool isOK;
    QString fileName = QInputDialog::getText( NULL,"OPEN","Please input database name", QLineEdit::Normal,"",&isOK);

    if( ! isOK ) {
        return;
    }

    if ( ! fileName.isEmpty() ) {
        char*  ch;
        QByteArray ba = fileName.toLatin1();
        ch = ba.data();
        databaseManager.execute(ch);
        updateComboBox();
        showTable(ui->comboBox->currentText());
    }
}

void MainWindow::on_excuteFile_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open SQL File"), QString(),
                                                        tr("SQL Files (*.sql)"));

       // qDebug() << fileName;
        if (!fileName.isEmpty()) {
            databaseManager.paseFile( fileName.toStdString());
            updateComboBox();
            showTable(ui->comboBox->currentText());
        }
}
