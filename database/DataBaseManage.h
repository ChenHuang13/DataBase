//
// Created by huangsy13 on 11/13/15.
//

//DataBaseManager类：总管理类。

#ifndef DATABASE_DATABASEMANAGE_H
#define DATABASE_DATABASEMANAGE_H

#include <iostream>
#include "define.h"
#include "Debug.h"
#include "Order.h"
#include "RecordManager/RecordManager.h"
#include "QueryManager/QueryManager.h"
#include "SystemManager/SystemManager.h"
#include "IndexManager/IndexManager.h"

using namespace std;

class DataBaseManager {
public:
    SystemManager *systemManager;
    QueryManager *queryManager;
    IndexManager *indexManager;

    //全局初始化
    void globalInit() {
        systemManager = new SystemManager();
        queryManager  = new QueryManager();
        indexManager  = new IndexManager();
    }

    //运行管理
    void run() {
        string line;
        while (cin >> line){
            Order order(line);
            debug << order << endl;
            switch (order.type){
                case DDL:
                    systemManager->run(order);
                    break;
                case DML:
                    queryManager->run(order);
                    break;
                case ERR:
                default:
                    debug << "解析命令出错！"<<endl;
                    break;
            }
        }
    }
};

#endif //DATABASE_DATABASEMANAGE_H
