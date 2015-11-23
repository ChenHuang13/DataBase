//
// Created by huangsy13 on 10/26/15.
//

#ifndef DATABASE_RECORDMANAGER_H
#define DATABASE_RECORDMANAGER_H

#include "record.h"
#include "query.h"
#include "vector"
#include "../bufmanager/BufPageManager.h"
#include "cstring"
#include "cmath"
#include "iostream"

//记录管理模块
class RecordManager {
private:
    BufPageManager *bufPageManager;  //传入的缓存管理模块
    FileManager *fileManager;  //传入的文件管理模块
    int fileID; //打开文件的id
    int tableWidth = 6;
    int tableType[6] = {0, 0, 1, 0, 1, 0};

    int tableID;
    int dataWidth;
    int* dataType;
public:
    //构造函数
    RecordManager() {
        fileManager = new FileManager();
        bufPageManager = new BufPageManager(fileManager);
    }

    //构造函数需要传入文件管理类和缓存管理类
    RecordManager(FileManager *fileManager, BufPageManager *bufPageManager) {
        this->fileManager = fileManager;
        this->bufPageManager = bufPageManager;
    }

    //解析函数
    virtual ~RecordManager() {

    }

    //插入记录函数
    int insertRecord(Record record) {
        int index;
        int pageID;
        BufType b = bufPageManager->getPage(fileID, tableID, index); //获取第一页
        BufType head = b;
        while (b[1] + dataWidth + PAGE_TOP + PAGE_BOTTOM < PAGE_INT_NUM) {
            if (b[2]) pageID = b[2];
            else pageID = b[2] = insertPage();
            BufType b = bufPageManager->getPage(fileID, pageID, index); //获取下一页
        }
        BufType e = b + PAGE_INT_NUM - PAGE_BOTTOM; //获取末尾
        b += PAGE_TOP; //跳过信息头

        while (b[0] && b[1] == 0) b += b[0];
        b[0] = dataWidth + 2;
        b[1] = 0;
        insertData(b, record.getBuffer(), dataType, dataWidth);
        while (b[0]) b += b[0];
        head[1] = b - head;
        bufPageManager->markDirty(index);
    }

    //删除记录函数
    bool deleteRecord(Record record) {
        vector<Record> list;
        int index;
        BufType b = bufPageManager->getPage(fileID, tableID, index); //获取第一页
        BufType e = b + PAGE_INT_NUM - PAGE_BOTTOM; //获取末尾
        b += PAGE_TOP; //跳过信息头
        while (b[0]) {
            BufType data = getData(b, e, dataWidth, dataType);
            if (compile(data, record.getBuffer(), dataWidth, dataType)) {
                b[1] = 1;
                for (int i = 0; i < dataWidth; ++i) {
                    if (dataType[i]) {
                        BufType tmp = data[i] - 2;
                        tmp[1] = 1;
                    }
                }
            }

            b += b[0];
        }
    }

    void search(Query query, bool print = false) {
        vector <Record> list;
        queryPage(query, fileID, tableID, list, print);
    }

    //查询函数，输入参数为查询类，输出为查询到的记录列表
    void queryPage(Query query, int fileID, int pageID, vector <Record> &list, bool print = false) {
        int index;
        BufType b = bufPageManager->getPage(fileID, pageID, index); //获取第一页
        BufType e = b + PAGE_INT_NUM - PAGE_BOTTOM; //获取末尾
        b += PAGE_TOP; //跳过信息头
        while (b[0]) {
            if (b[1] == 0) {
                BufType data = getData(b, e, dataWidth, dataType);

                if (query.getSearchPart() == nullptr) {
                    Record *record = new Record();
                    record->setBuffer(data, dataWidth);
                    record->result_end = query_source.getEnd();
                    list.push_back(*record);
                } else {
                    if (compile(data, query.getSearchPart(), dataWidth, dataType, query.getSearchType())) {
                        Record *record = new Record();
                        record->setBuffer(data, dataWidth);
                        record->result_begin = query.getStart();
                        record->result_end = query.getEnd();
                        list.push_back(*record);
                    }
                }
                if (print) std::cout << data[0] << std::endl;
            }
            b += b[0];
        }
    }

    //根据旧的记录产生新纪录
    Record formRecord(Record old, Query target) {
        BufType oldBuf = old.getBuffer();
        if (target.getSearch_part() != nullptr) {
            for (int i = 0; i < dataWidth; ++i)
                if (target.getSearchType) {
                    oldBuf[i] = target.getSearchPart();
                }
        }
        return old;
    }

    //跟新函数
    int update(Query query, Query target) {
        vector <Record> recordList;
        recordList = search(query);
        for (int i = 0; i < recordList.size(); i++) {
            delete_record(recordList[i]);
            Record recordNew = formRecord(recordList[i], target);
            insert(recordNew);
        }
    }

    //新建数据库
    bool createSQL(const char *name) {
        fileManager->createFile(name);
        int fileID;
        fileManager->openFile(name, fileID);
        int pageID = 0;
        int index;
        BufType b = bufPageManager->allocPage(fileID, pageID, index, false);
        b[0] = pageID;
        b += PAGE_TOP;
        bufPageManager->markDirty(index);
        return true;
    }

    //打开存在的数据库
    bool openSQL(const char *name) {
        fileManager->openFile(name, fileID);
    }

    //新建表
    int openTable(const char *name) {
        //return the tableID
        vector <Record> list;
        queryPage(new Query(0, 1, &name), fileID, 0, list, true);
        tableID = list[0].getBuffer()[1];
        dataWidth = list[0].getBuffer()[2];
        dataType = list[0].getBuffer()[3];
        return tableID;
    }

    int insertPage() {
        int index;
        BufType b = bufPageManager->getPage(fileID, 0, index); //获取第一页
        unsigned int &pageNum = b[1];
        pageNum++;
        b = bufPageManager->allocPage(fileID, pageNum, index, false);
        b[0] = pageNum;
        bufPageManager->markDirty(index);
        return pageNum;
    }

    BufType insertData(BufType b, BufType e, int width, BufType data, BufType dataType) {
        while (b[0]) b += b[0];
        b[0] = width + 2;
        b += 2;
        for (int i = 0; i < width; ++i) {
            if (dataType[i] == 0)
                b[i] = data[i];
            else {
                e = insertString(b, (BufType ) data[i]);
                b[i] = e - b;
            }
        }
        return b;
    }

    BufType insertString(BufType b, BufType data) {
        int width = ceil(strlen(data) / 4.0);
        while (b[0]) b += e[0];
        e[0] = width + 2;
        memcpy(e, name, strlen(name));
        return e;
    }

    int createTable(const char *name, int width, BufType type) {
        //data is the 0/1 array to determine whether this value is String or Data
        //for example, {int, char*, char} to data is [0, 1, 0]
        tableID = 0;
        dataType = tableType;

        int table[4];
        table[0] = name, table[1] = insertPage(), table[2] = width, table[3] = type;

        Record record = new Record();
        record.setBuffer(table, tableWidth);
        insertRecord(record);
        return table[1];
    }

    bool close() {
        bufPageManager->close(); //关闭缓存类并写回
        fileManager->closeFile(fileID); //关闭数据库文件
    }
};

#endif //DATABASE_RECORDMANAGER_H
