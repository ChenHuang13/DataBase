//
// Created by huangsy13 on 10/26/15.
//

#ifndef DATABASE_RECORD_H
#define DATABASE_RECORD_H

//单条记录类
class Record {
private:
    int *buffer;//待插入的记录
public:
    Record() { }

    virtual ~Record() { }

    int *getBuffer() const {
        return buffer;
    }

    void setBuffer(int *buffer) {
        Record::buffer = buffer;
    }
};

#endif //DATABASE_RECORD_H
