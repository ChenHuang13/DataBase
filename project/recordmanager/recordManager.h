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

//记录管理模块
class RecordManager {
private:
    BufPageManager *bufPageManager;  //传入的缓存管理模块
    FileManager *fileManager;  //传入的文件管理模块
    int SQLID; //打开文件的id
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
    int insert(Record record) {

    }

    //删除记录函数
    int delete_record(Record record) {

    }

    //查询函数，输入参数为查询类，输出为查询到的记录列表
    vector <Record> query(Query query_source) {
        vector <Record> list;
        return list;
    }

    //根据旧的记录产生新纪录
    Record form_record(Record old, Query target) {
        return Record();
    }

    //跟新函数
    int update(Query query_source, Query target) {
        vector <Record> record_list;
        record_list = query(query_source);
        for (int i = 0; i < record_list.size(); i++) {
            Record record_new = form_record(record_list[i], target);
            delete_record(record_list[i]);
            insert(record_new);
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
		b[1] = 1;
		b += 32;
        bufPageManager->markDirty(index);
        bufPageManager->close();
        fileManager->closeFile(fileID);
		return true;
    }

    //打开存在的数据库
	bool openSQL(const char* name) {
		fileManager->openFile(name, SQLID);
	}

    //新建表
	bool createTable(const char *name, int width) {
		int index;

        BufType b = bufPageManager->getPage(SQLID, 0, index); //获取第一页

        BufType e = b + PAGE_INT_NUM - PAGE_BOTTOM; //获取末尾

        unsigned int *pageHead = b;
        unsigned int &pageNum = b[1];

        b += PAGE_TOP; //跳过信息头

		while (b[0]) b += 4;
		while (e[0]) e -= e[0];
		e[0] = ceil(strlen(name) / 4) + 1;
		memcpy(e - e[0] + 1, name, strlen(name));
        b[0] = 1, b[1] = e - pageHead, b[2] = pageNum++, b[3] = width;
		int pageID = b[2];
		bufPageManager->markDirty(index);
		b = bufPageManager->allocPage(SQLID, pageID, index, false);
		b[0] = pageID;
		bufPageManager->markDirty(index);
	}

    bool close() {
        bufPageManager->close(); //关闭缓存类并写回
        fileManager->closeFile(SQLID); //关闭数据库文件
    }
};

#endif //DATABASE_RECORDMANAGER_H
