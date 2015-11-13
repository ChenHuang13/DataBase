//
// Created by huangsy13 on 11/13/15.
//

//RecordManager类：被系统管理模块、查询解析模块和索引模块调用，执行底层的记录查询，记录插入，记录删除功能。

#ifndef DATABASE_RECORDMANAGER_H
#define DATABASE_RECORDMANAGER_H
#include "../bufmanager/BufPageManager.h"
#include "RecordID.h"
#include "../define.h"

class RecordManager{
public:
    RecordManager  (BufPageManager &bufPageManager);            // Constructor
    ~RecordManager ();                           // Destructor

    void CreateFile  (const char *fileName, int recordSize){

    }
    // Create a new file
    void DestroyFile (const char *fileName){

    }     // Destroy a file

    void OpenFile    (const char *fileName, FileID fileID){

    }
    // Open a file

    void CloseFile   (FileID fileID){

    }  // Close a file
};



#endif //DATABASE_RECORDMANAGER_H
