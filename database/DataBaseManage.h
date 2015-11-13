//
// Created by huangsy13 on 11/13/15.
//

//DataBaseManager类：总管理类。

#ifndef DATABASE_DATABASEMANAGE_H
#define DATABASE_DATABASEMANAGE_H

#include <iostream>
#include "define.h"
#include "RecordManager/RecordManager.h"
#include "QueryManager/QueryManager.h"
#include "SystemManager/SystemManager.h"

using namespace std;

class DataBaseManager{
public:
    void globalInit(){

    }

    void run(){
        string line;
        while(cin >>line)
            cout << line << endl;
    }
};

#endif //DATABASE_DATABASEMANAGE_H
