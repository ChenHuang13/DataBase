//
// Created by huangsy13 on 11/13/15.
//

#ifndef DATABASE_INDEXMANAGER_H
#define DATABASE_INDEXMANAGER_H

#include "../RecordManager/RecordManager.h"

class IndexManager{
public:
    RecordManager* recordManager;
    IndexManager(RecordManager* recordManager):recordManager(recordManager){}

    void createIndex(){}

    void destroyIndex(){

    }

    void openIndex(){}

    void closeIndex(){}
};



#endif //DATABASE_INDEXMANAGER_H
