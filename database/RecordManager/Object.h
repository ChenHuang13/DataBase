//Object类:多种抽象数据类型的基类和派生类，包含各种类型。


#ifndef DATABASE_OBJECT_H
#define DATABASE_OBJECT_H

#include <vector>
#include <cstring>
#include <string>
#include "../define.h"

class Object{
public:
	DataType type;
	Object(){
	}

	virtual void toBuffer(BufType buf) {

	}

};

class Integer : public Object {
public:
	int data;
	Integer(BufType buf) {
		Object::type = INTEGER;
		this->data = *((int*)buf);
	}

	void toBuffer(BufType buf) {
		*((int*)buf) = data;
	}

	int getInteger() {
		return data;
	}
};

class Float : public Object {
public:
	float data;
	Float(BufType buf) {
		Object::type = FLOAT;
		this->data = *((float*)buf);
	}

	void toBuffer(BufType buf) {
		*((float*)buf) = data;
	}

	float getFloat() {
		return data;
	}
};

class String : public Object {
public:
	string data;
	String(BufType buf) {
		Object::type = STRING;
		data = (char*) buf;
	}

	void toBuffer(BufType buf) {
		strcpy((char*)buf, data.c_str());
	}

	string getString() {
		return data;
	}
};




#endif //DATABASE_OBJECT_H
