//TableInfo类：通过TableInfo,读出该文件页头的信息。
#include "../define.h"

#ifndef DATABASE_TABLEINFO_H
#define DATABASE_TABLEINFO_H

class TableInfo{
public:
	char tableName[MAX_FIELD_LENGTH];
	int size;
	int recordSize;
	int body;
};

#endif //DATABASE_TABLEINFO_H
