//
// Created by huangsy13 on 10/26/15.
//

#ifndef DATABASE_RECORDMANAGER_H
#define DATABASE_RECORDMANAGER_H

#include "record.h"
#include "query.h"
#include "vector"
#include "../bufmanager/BufPageManager.h"

class RecordManager {
private:
    BufPageManager *bufPageManager;
    FileManager *fileManager;
	int SQLID;
public:
    RecordManager() {
    	fileManager = new FileManager();
	    BufPageManager *bufPageManager = new BufPageManager(fm);
    }

    RecordManager(BufPageManager *bufPageManager) {
		this.bufPageManager = bufPageManager;
	}

    virtual ~RecordManager() {

    }

    int insert(Record record) {

    }

    int delete_record(Record record) {

    }

    vector <Record> query(Query query_source) {
        vector <Record> list;
        return list;
    }

    Record form_record(Record old, Query target) {
        return Record();
    }

    int update(Query query_source, Query target) {
        vector <Record> record_list;
        record_list = query(query_source);
        for (int i = 0; i < record_list.size(); i++) {
            Record record_new = form_record(record_list[i]., target);
            delete_record(record_list[i]);
            insert(record_new);
        }
    }

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
		fileManager->closeFile(name);
		return true;
    }

	bool openSQL(const char* name) {
		fileManager->openFile(name, SQLID);
	}

	bool createTable(const char *name, int width) {
		int index;
		BufType b = bufPageManager->allocPage(SQLID, 0, index, false);
		ByfType e = b + PAGE_INT_NUM - PAGE_BOTTOM;
		int& pageNum = b[1];
		b += PAGE_TOP;
		while (b[0]) b += 4;
		while (e[0]) e -= e[0];
		e[0] = ceil(strlen(name) / 4) + 1;
		memcpy(e - e[0] + 1, name, strlen(name));
		b[0] = 1, b[1] = e, b[2] = pageNum++;
		int pageID = b[2];
		bufPageManager->markDirty(index);
		b = bufPageManager->allocPage(SQLID, pageID, index, false);
		b[0] = pageID;
		bufPageManager->markDirty(index);
	}


};

#endif //DATABASE_RECORDMANAGER_H
