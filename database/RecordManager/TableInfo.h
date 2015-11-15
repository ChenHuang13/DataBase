//TableInfo类：通过TableInfo,读出该文件页头的信息。
#include "../define.h"

#ifndef DATABASE_TABLEINFO_H
#define DATABASE_TABLEINFO_H

class TableInfo{
public:
	TableInfo(){}

	TableInfo(string path){
		FileManager *_fm = new FileManager();
		BufPageManager *_bpm = new BufPageManager(_fm);
		int fileID;
		_fm->openFile(path.c_str(), fileID);
		int pageID = 0, index;
		BufType b = _bpm->getPage(fileID, pageID, index);
		TableInfo* tableInfo = (TableInfo *)b;
		for (int i = 0 ; i < MAX_FIELD_LENGTH ; i++){
			this->tableName[i] = tableInfo->tableName[i];
		}


		this->size = tableInfo->size;
		this->recordSize = tableInfo->recordSize;
		this->body = tableInfo->body;
	}

	char tableName[MAX_FIELD_LENGTH];
	int size;
	int recordSize;
	int body;

	TableInfo operator = (TableInfo* tableInfo){
		for (int i = 0 ; i < MAX_FIELD_LENGTH ; i++){
			this->tableName[i] = tableInfo->tableName[i];
		}
		this->size = tableInfo->size;
		this->recordSize = tableInfo->recordSize;
		this->body = tableInfo->body;
		return *this;
	}
};

#endif //DATABASE_TABLEINFO_H
