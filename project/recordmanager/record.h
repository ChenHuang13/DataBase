//
// Created by huangsy13 on 10/26/15.
//

#ifndef DATABASE_RECORD_H
#define DATABASE_RECORD_H

#include "cstring"
#include "../utils/pagedef.h"

//单条记录类
class Record {
private:
    BufType buffer;//待插入的记录

    char *table_name;
public:
    int result_begin, result_end;
    int weith;
    Record() { }

    virtual ~Record() { }

    BufType getBuffer() const {
        return buffer;
    }

    void setBuffer(BufType buffer, int weith) {
        this->weith = weith;
        this->buffer = buffer;
    }
};

#endif //DATABASE_RECORD_H
