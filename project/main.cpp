
/*
 * testfilesystem.cpp
 *
 *  Created on: 2015年10月6日
 *      Author: lql
 */

#include "./bufmanager/BufPageManager.h"
#include "./recordmanager/recordManager.h"

using namespace std;

int main() {
    RecordManager *recordManager = new RecordManager();
    recordManager->createSQL("my.db");
    recordManager->openSQL("my.db");
    recordManager->createTable("Student", 50);

//    for (int i =  0; i < 10; i++){
//        recordManager->insert(*(new Record())); //记录插入测试
//    }
//
//    recordManager->query(*(new Query())); //记录查询测试
//
//    recordManager->update(*(new Query), *(new Query)); //记录更新测试
//
//    recordManager->query(*(new Query())); //记录查询测试
//
//    recordManager->delete_record(*(new Record())); //记录删除测试
//
//    recordManager->query(*(new Query())); //记录查询测试

    recordManager->close();//关闭数据库

    return 0;
}