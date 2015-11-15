//
// Created by huangsy13 on 11/13/15.
//

//Field类：储存每种表的字段信息，会被Record类和Table类当成成员变量。

#include <utility>
#include <vector>
#include <string>
#include "../define.h"

#ifndef DATABASE_FIELD_H
#define DATABASE_FIELD_H

class Field{
public:
	vector < pair < string , DataType > > list;//name & type's pair
	vector < int > nums;
	Field() {

	}
	int size() {
		return list.size();
	}

	string getFieldName(int fieldID) {
		return list[fieldID].first;
	}

	DataType getFieldType(int fieldID) {
		return list[fieldID].second;
	}

	int getFieldSize(int fieldID) {
		return nums[fieldID];
	}

};

#endif //DATABASE_FIELD_H
