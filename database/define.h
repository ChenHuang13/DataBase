//
// Created by huangsy13 on 11/13/15.
//

//定义一些枚举体和常量

#ifndef DATABASE_DEFINE_H
#define DATABASE_DEFINE_H

#include <cstring>

enum PageType{recordPage, tablePage, indexPage};
enum OrderType{DDL, DML, ERR};
typedef int PageID,FileID;

const int MAX_FIELD_LENGTH = 128;

#endif //DATABASE_DEFINE_H
