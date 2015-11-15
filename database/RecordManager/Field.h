//
// Created by huangsy13 on 11/13/15.
//

//Field类：储存每种表的字段信息，会被Record类和Table类当成成员变量。

#ifndef DATABASE_FIELD_H
#define DATABASE_FIELD_H

#include <utility>
#include <vector>
#include <cstring>
#include "../define.h"

class Field {
public:
    std::string line;
    std::vector<std::pair<std::string, DataType> > list;
    //name & type's pair
    std::vector<int> nums;
    std::vector<RemarkType> remarks;

    Field() {}

    Field(std::string line):line(line) {
        fromStr(line);
    }

    int size() const {
        return list.size();
    }

    std::string getFieldName(const int fieldID) const {
        return list[fieldID].first;
    }

    DataType getFieldType(const int fieldID) const {
        return list[fieldID].second;
    }

    int getFieldSize(const int fieldID) const {
        return nums[fieldID];
    }

    RemarkType getRemark(const int fieldID) const {
        return remarks[fieldID];
    }

    void fromStr(std::string str){
        //TODO
    }

    std::string toStr(){
        //TODO
        line = "id int(32)";
        return line;
    }
};

#endif //DATABASE_FIELD_H
