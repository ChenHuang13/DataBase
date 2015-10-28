
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
    int record_lenth;
    recordManager->createTable("Student", record_lenth);

    cout << "insert test" << endl; //插入0到9的数据
    for (int i = 0; i < 10; i++) {
        Record *record = new Record();
        BufType data = new unsigned int[record_lenth];
        memset(data, 0, record_lenth);
        data[0] = (unsigned int) i;
        record->setBuffer(data, record_lenth);
        record->weith = record_lenth;
        recordManager->insert(*record); //记录插入测试
    }

    recordManager->query(*(new Query()), true); //记录查询测试，查询所有数据

    cout << "update test" << endl;
    BufType a = new unsigned int[1];
    a[0] = 5;
    Query *source = new Query(0, 1, a); //构造需要被更改的数据
    BufType b = new unsigned int[1];
    b[0] = 13;
    Query *target = new Query(0, 1, b); //构造更改的目标
    recordManager->update(*source, *target); //记录更新测试，把id = 5 的数据更新为 13

    recordManager->query(*(new Query()), true); //记录查询测试，查询所有

    cout << "delete test" << endl;//删除id从 0 到 4的数据
    for (int i = 0; i < 5; i++) {
        Record *record = new Record();
        BufType data = new unsigned int[record_lenth];
        memset(data, 0, record_lenth);
        data[0] = (unsigned int) i;
        record->setBuffer(data, record_lenth);
        record->weith = record_lenth;
        recordManager->delete_record(*record); //记录删除测试
    }

    recordManager->query(*(new Query()), true); //记录查询测试

    recordManager->close();//关闭数据库

    return 0;
}