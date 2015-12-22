//
// Created by huangsy13 on 11/13/15.
//

//Record类:一条抽象记录类，包含数据段和其他信息段。


#ifndef DATABASE_RECORD_H
#define DATABASE_RECORD_H

#include <vector>
#include "Object.h"
#include "Field.h"
#include "RecordID.h"
#include "../define.h"
#include "../utils/pagedef.h"

class Record {
public:
    RecordID rid;
    std::vector<Object *> data;

    Record() {
        rid.page = rid.slot = -1;
    }

    Record(Field field, BufType buf, RecordID rid) {
        this->rid = rid;
        data.clear();
        for (int i = 0; i < field.size(); ++i) {
            switch (field.getFieldType(i)) {
                case OBEJECT:
                    data.push_back(new Object());
                    break;
                case INTEGER:
                    data.push_back(new Integer(buf));
                    break;
                case FLOAT:
                    data.push_back(new Float(buf));
                    break;
                case STRING:
                    data.push_back(new String(buf));
                    break;
            }
            buf += field.getFieldSize(i);
        }
    }

    void toBuffer(Field field, BufType buf) {
        for (int i = 0; i < field.size(); ++i) {

            data[i]->toBuffer(buf);

            buf += field.getFieldSize(i);
        }
    }

    void destroy() {
        for (int i = 0; i < data.size(); ++i)
            delete data[i];
        data.clear();
    }

};

#endif //DATABASE_RECORD_H
