//
// Created by huangsy13 on 11/13/15.
//

//定义一些枚举体和常量

#ifndef DATABASE_DEFINE_H
#define DATABASE_DEFINE_H

#include <cstring>

enum DataType{OBEJECT, INTEGER, FLOAT, STRING};
enum PageType{RecordPage, TablePage, IndexPage};
enum OrderType{DDL, DML, UNFINISH , FINISH , ERR};
enum SubType{CreateDataBase, UseDataBase, CreateTable, DropDataBase, DropTable, ShowDataBase, ShowTable};
enum RemarkType{NOTNULL, PRIMARYKEY, FOREIGNKEY};
typedef int PageID,FileID;

const int MAX_FIELD_LENGTH = 128;
const int PAGE_HEAD_SIZE = 128;

#endif //DATABASE_DEFINE_H
