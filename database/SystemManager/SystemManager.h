//
// Created by huangsy13 on 11/13/15.
//

//有parse(string order)解析命令函数，解析后调用相关执行函数。createDB(string DBName)创建数据库函数。
// dropDB(string DBName)删除数据库函数。useDB(string DBName)使用数据库函数。showDB(string DBName),显示数据库中的所有表名。
// createTable(string tableName, Field field)创建表函数。drop(string tableName)删除表函数。
// showTable(string tableName)显示表字段函数。

#ifndef DATABASE_SYSTEMMANAGER_H
#define DATABASE_SYSTEMMANAGER_H

#include "../RecordManager/RecordManager.h"
#include "../tools/FileStruct.h"
#include "../Print.h"

#include "InfoManeger.h"

class SystemManager {
public:
    string DBNow;
    RecordManager *recordManager;
    SystemManager(RecordManager *recordManager) : recordManager(recordManager) {}

    void run(Order order) {
        switch (order.subType) {
            case CreateDataBase:
                createDataBase(order);
                break;
            case CreateTable:
                createTable(order);
                break;
            case UseDataBase:
                useDataBase(order);
                break;
            case DropDataBase:
                dropDB(order);
                break;
            case DropTable:
                dropTable(order);
                break;
            case ShowDataBase:
                showDB(order);
                break;
            case ShowTable:
                showTable(order);
                break;
            default:
                debug << "解析数据库操作命令出错！";
                break;
        }
    }

    void createDataBase(Order order) {

        string DBname = order.getDBName();
        if (isDir(DBname)){
            string dropOrder = "rm -rf " + DBname;
            system(dropOrder.c_str());
        }
        string createOrder = "mkdir " + DBname;
        string infoFile = DBname + "/" + ".info.txt";
        system(createOrder.c_str());
        ofstream out(infoFile.c_str());
        out.close();
    }

    void createTable(Order order) {
        string path = DBNow + "/" + order.getTableName();
        recordManager->createFile(path.c_str(), order.getField());
        InfoManager::createTable(DBNow,order.getTableName() , order.getField());
    }

    void useDataBase(Order order) {
        DBNow = order.getDBName();
    }

    void dropDB(Order order) {
        string dropOrder = "rm -rf " + order.getDBName();
        system(dropOrder.c_str());
    }

    void dropTable(Order order) {
        string dropOrder = "rm -f " + DBNow + "/" + order.getTableName();
        system(dropOrder.c_str());
        InfoManager::dropTable(DBNow,order.getTableName());
    }

    void showDB(Order order) {
        vector<string> tableList;
        InfoManager::getTables(order.getDBName() ,tableList);
        print << tableList << endl;
    }

    void showTable(Order order) {
        Field field = InfoManager::getField(DBNow, order.getTableName());
        print << field.toStr() <<endl;
    }
};

#endif //DATABASE_SYSTEMMANAGER_H
