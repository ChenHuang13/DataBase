//
// Created by huangsy13 on 11/13/15.
//

//RecordManager类：被系统管理模块、查询解析模块和索引模块调用，执行底层的记录查询，记录插入，记录删除功能。

#ifndef DATABASE_RECORDMANAGER_H
#define DATABASE_RECORDMANAGER_H
#include "../bufmanager/BufPageManager.h"
#include "RecordID.h"
#include "../define.h"
#include "Field.h"

class RecordManager{
private:
	Field field;
	BufPageManager *bpm;
	FileManager *fm;
	int fileID;
public:
    RecordManager  () {
		fm = new FileManager();
		buf = new BufPageManager(fm);
	}            // Constructor
    ~RecordManager () {
		delete fm;
		delete buf;
	}                           // Destructor

	void getField() {
		return field;
	}

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
		}
		bpm->markDirty(index);
		fm->close(fileID);
    }
    // Create a new file
    void destroyFile (const char *fileName){
		fm->destroyFile(fileName);
    }     // Destroy a file

    void openFile    (const char *fileName, FileID fileID){
		fm->openFile(fileName, fileID);
		int pageID = 0, index;
		BufType b = bpm->allocPage(fileID, pageID, index, false);
		TableInfo* info = (TableInfo*) b;
		int size = info.size;
		field.list.clear();
		BufType body = (BufType) &info.body;
		char* fieldName = (char*) body;
		for (int i = 0; i < size; ++i) {
			string field_name(fieldName);
			fieldName += MAX_FIELD_SIZE;
			int field_size = *((int*) fieldName);
			field.list.push_back(make_pair(field_name, field_size));
		}
		bpm->markDirty(index);
		fm->close(fileID);
    }
    // Open a file

    void closeFile   (FileID fileID){
		fm->close(fileID);
    }  // Close a file
};



#endif //DATABASE_RECORDMANAGER_H
