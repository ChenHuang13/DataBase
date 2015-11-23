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

    int *tableID;
public:
    int result_begin, result_end;
    int width;
    Record() { }

    virtual ~Record() { }

    BufType getBuffer() const {
        return buffer;
    }

    void setBuffer(BufType buffer, int width) {
        this->width = width;
        this->buffer = buffer;
    }
};

#endif //DATABASE_RECORD_H
