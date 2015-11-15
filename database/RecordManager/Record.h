//
// Created by huangsy13 on 11/13/15.
//

//Record类:一条抽象记录类，包含数据段和其他信息段。
#include <vector>
#include "Object.h"
#include "Field.h"
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
		this->rid = rid;
		data.clear();
		for (int i = 0; i < field.size; ++i) {
			switch (field.getFieldType()) {
				OBEJECT:
					data.push_back(new Object());
					break;
				INTEGER:
					data.push_back(new Integer(buf));
					break;
				FLOAT:
					data.push_back(new Float(buf));
					break;
				STRING:
					data.push_back(new String(buf));
					break;
			}
			buf += field.getFieldSize();
		}
	}

	void toBuffer(Field field, BufType buf) {
		for (int i = 0; i < field.size; ++i) {
			switch (field.getFieldType()) {
					data[i].toBuffer(buf);
					break;
			}
			buf += field.getFieldSize();
		}
	}

	void destroy() {
		for (int i = 0; i < data.size; ++i)
			data[i].destroy();
		data.clear();
	}


};

#endif //DATABASE_RECORD_H
