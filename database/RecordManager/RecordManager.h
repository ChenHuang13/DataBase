//
// Created by huangsy13 on 11/13/15.
//

//RecordManager类：被系统管理模块、查询解析模块和索引模块调用，执行底层的记录查询，记录插入，记录删除功能。

#ifndef DATABASE_RECORDMANAGER_H
#define DATABASE_RECORDMANAGER_H
#include "../utils/pagedef.h"
#include "../bufmanager/BufPageManager.h"
#include "RecordID.h"
#include "../define.h"
#include "Field.h"

class RecordManager{
private:
	BufPageManager *bpm;
	FileManager *fm;
public:
    RecordManager  () {
		fm = new FileManager();
		buf = new BufPageManager(fm);
	}            // Constructor
    ~RecordManager () {
		delete fm;
		delete bpm;
	}                           // Destructor

    void createFile  (const char *fileName, Field field){
		this->field = field;
		fm->createFile(fileName);
		fm->openFile(fileName, fileID);
		int pageID = 0, index;
		BufType b = bpm->allocPage(fileID, pageID, index, false);
		TableInfo* info = (TableInfo*) b;
		strcpy(info.tableName, fileName);
		int size = info.size = field.size();

		BufType body = (BufType) &info.body;
		char* fieldName = (char*) body;
		for (int i = 0; i < size; ++i) {
			strcpy(fieldName, field.getFieldName(i).c_str);
			fieldName += MAX_FIELD_SIZE;
			*((int*) fieldName) = field.getFieldSize(i);
			fieldName += 1;
			recordSize += field.getFieldSize(i);
		}
		bpm->markDirty(index);
		bpm->close();
		fm->close(fileID);
    }
    // Create a new file
    void destroyFile (const char *fileName){
		fm->destroyFile(fileName);
    }     // Destroy a file

    RecordHandle* openFile    (const char *fileName) {
		FileManager _fm = new FileManager();
		BufPageManager _bpm = new BufPageManager(_fm);

		_fm->openFile(fileName, fileID);
		int pageID = 0, index;
		BufType b = _bpm->getPage(fileID, pageID, index);
		TableInfo* info = (TableInfo*) b;
		int size = info.size;
		int recordSize;
		Field field;
		field.list.clear();
		BufType body = (BufType) &info.body;
		char* fieldName = (char*) body;
		for (int i = 0; i < size; ++i) {
			string field_name(fieldName);
			fieldName += MAX_FIELD_SIZE;
			int field_size = *((int*) fieldName);
			fieldName += 1;
			field.list.push_back(make_pair(field_name, field_size));
		}

		return new　RecordHandle(fileID, field, recordSize, _bpm, _fm);
    }
    // Open a file

    void closeFile   (RecordHandle* handle){
		handle->bpm->close();
		handle->fm->close(handle.fileID);
    }  // Close a file
};


class RecordHandle{
private:
	BufPageManager *bpm;
	FileManager *fm;
	int fileID;
	int recordSize;
	Field field;
	
public:
    RecordHandle  (int fileID, Field field, int recordSize, BufPageManager* bpm, FileManager *fm): fileID(fileID), field(field), recordSize(recordSize + 1), bpm(bpm), fm(fm) {

	}            // Constructor
    ~RecordManager () {
		delete fm;
		delete bpm;
	}                           // Destructor

	void getField() {
		return field;
	}

	Record getRecord(RecordID rid) {
		int index;
		BufType b = bpm->getPage(fileID, rid.page, inedx);
		
		PageInfo info = (PageInfo*) b;
		BufType body = (BufType) &info.body;
		body += recordSize * rid.slot;

		if (*(body + recordSize - 1) == 1) return new Record();
		else return new Record(field, body, rid);
	}

	RecordID insertRecord(Record rec) {
		RecordID rid = rec.rid;
		
		int pageID = 1;
		int index;
		for (; ; pageID++) {
			BufType b = bpm->getPage(fileID, pageID, inedx);
		
			PageInfo info = (PageInfo*) b;
			int used = info->used;
			if (used + recordSize < PAGE_SIZE) {
				BufType body = (BufType) &info.body;

				body += recordSize * rid.slot;
		
				rec.toBuffer(Field, body);
				info->used += recordSize;
				break;
			}
		}
	}

	void deleteRecord(RecordID rid) {

		int index;
		BufType b = bpm->getPage(fileID, rid.page, inedx);
		
		PageInfo info = (PageInfo*) b;
		BufType body = (BufType) &info.body;
		BufType r = body + recordSize * rid.slot;
		*(r + recordSize - 1) = 1;// lazy delete
		memcpy(d, r, recordSize * 4);

		return new Record(Field, body, rid);
		//TODO
	}

	void updateRecord(Record rec) {
		RecordID rid = rec.rid;
		
		int index;
		BufType b = bpm->getPage(fileID, rid.page, inedx);
		
		PageInfo info = (PageInfo*) b;
		BufType body = (BufType) &info.body;

		body += recordSize * rid.slot;
		
		rec.toBuffer(Field, body);
	}

	void forcePages(int pageID = -1) {
		if (pageID == -1)
			bpm->close();
		else {
			int index;
			bpm->getPage(fielID, pageID, index);
			bpm->writeBack(index);
		}
	}
};



#endif //DATABASE_RECORDMANAGER_H
