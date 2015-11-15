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
    RecordManager *recordManager;

    //全局初始化
    void globalInit() {
        recordManager = new RecordManager();
        systemManager = new SystemManager(recordManager);
        queryManager = new QueryManager(recordManager);
        indexManager = new IndexManager(recordManager);
    }

    //运行管理
    void run() {
        string line;
        Order order;
        while (cin >> line) {
            order.getString(line);
            if (order.type != UNFINISH) debug << order << endl;

            if (order.type != UNFINISH) {
                switch (order.type) {
                    case DDL:
                        systemManager->run(order);
                        break;
                    case DML:
                        queryManager->run(order);
                        break;
                    case UNFINISH:
                        break;
                    case ERR:
                    default:
                        debug << "解析命令出错！" << endl;
                        break;
                }
            }
        }
    }
};

#endif //DATABASE_DATABASEMANAGE_H
