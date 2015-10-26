//
// Created by huangsy13 on 10/26/15.
//

#ifndef DATABASE_RECORDMANAGER_H
#define DATABASE_RECORDMANAGER_H

#include "record.h"
#include "query.h"
#include "vector"
#include "../bufmanager/BufPageManager.h"

class RecordManager {
private:
    BufPageManager *bufPageManager;
public:
    RecordManager() {

    }

    RecordManager(BufPageManager *bufPageManager0) : bufPageManager(bufPageManager0) { }

    virtual ~RecordManager() {

    }

    int insert(Record record) {

    }

    int delete_record(Record record) {

    }

    vector <Record> query(Query query_source) {
        vector <Record> list;
        return list;
    }

    Record form_record(Record old, Query target) {
        return Record();
    }

    int update(Query query_source, Query target) {
        vector <Record> record_list;
        record_list = query(query_source);
        for (int i = 0; i < record_list.size(); i++) {
            Record record_new = form_record(record_list[i]., target);
            delete_record(record_list[i]);
            insert(record_new);
        }
    }


};

#endif //DATABASE_RECORDMANAGER_H

