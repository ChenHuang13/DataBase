//
// Created by huangsy13 on 11/13/15.
//

//Record类:一条抽象记录类，包含数据段和其他信息段。
#include <vector>
#include "Object.h"
#include "../define.h"

#ifndef DATABASE_RECORD_H
#define DATABASE_RECORD_H

class Record{
public:
	RecordID rid;
	vector < Object* > data;
	Record() {
		rid.page = rid.slot = -1;
	}

	Record(Field field, BufType buf, RecordID rid) {
		//TODO
	}

	void toBuffer(Field field, BufType buf) {
		//TODO
	}

	void destroy() {
		for (int i = 0; i < data.size; ++i)
			data[i].destroy();
		data.clear();
	}

	
};

#endif //DATABASE_RECORD_H
