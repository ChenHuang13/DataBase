//Object类:多种抽象数据类型的基类和派生类，包含各种类型。
#include <vector>
#include "Object.h"
#include "../define.h"

#ifndef DATABASE_RECORD_H
#define DATABASE_RECORD_H

class Object　{
public:
	DataType type;
	Object() : type(OBEJECT) {

	}
};

class Integer : public Object {
public:
	int data;
	Integer(data) {
		Object::type = INTEGER;
		this->data = data;
	}

	int getInteger() {
		return data;
	}
}

class Float : public Object {
public:
	float data;
	Float(data) {
		Object::type = FLOAT;
		this->data = data;
	}

	int getFloat() {
		return data;
	}
}

class String : public Object {
public:
	string data;
	String(data) {
		Object::type = String;
		this->data = data;
	}

	int getString() {
		return data;
	}
}




#endif //DATABASE_RECORD_H
