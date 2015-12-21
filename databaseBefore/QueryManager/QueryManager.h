//
// Created by huangsy13 on 11/13/15.
//

#ifndef DATABASE_QUERYMANAGER_H
#define DATABASE_QUERYMANAGER_H

#include "../RecordManager/RecordManager.h"

class QueryManager {
public:
    RecordManager* recordManager;
    QueryManager(RecordManager* recordManager):recordManager(recordManager){}

    void run(Order order) {

    }
};

#endif //DATABASE_QUERYMANAGER_H
