//
// Created by huangsy13 on 11/13/15.
//

//RecordManager类：被系统管理模块、查询解析模块和索引模块调用，执行底层的记录查询，记录插入，记录删除功能。


#ifndef DATABASE_RECORDMANAGER_H
#define DATABASE_RECORDMANAGER_H


#include "../utils/pagedef.h"
#include "../bufmanager/BufPageManager.h"
#include "RecordID.h"
#include "../define.h"
#include "Field.h"
#include "TableInfo.h"
#include "Record.h"
#include "PageInfo.h"


class RecordHandle {
private:
    int recordSize;
    Field field;
public:
    BufPageManager *bpm;
    FileManager *fm;
    int fileID;

    RecordHandle(int fileID, Field field, int recordSize, BufPageManager *bpm, FileManager *fm) : fileID(fileID),
                                                                                                  field(field),
                                                                                                  recordSize(
                                                                                                          recordSize +
                                                                                                          1), bpm(bpm),
                                                                                                  fm(fm) {

    }            // Constructor
    ~RecordHandle() {
        delete fm;
        delete bpm;
    }                           // Destructor

    Field getField() {
        return field;
    }

    Record getRecord(RecordID rid) {
        int index;
        BufType b = bpm->getPage(fileID, rid.page, index);

        PageInfo* info = (PageInfo *) b;
        BufType body = (BufType) &info->body;
        body += recordSize * rid.slot;

        if (*(body + recordSize - 1) == 1) return Record();
        else return Record(field, body, rid);
    }

    RecordID insertRecord(Record rec) {
        RecordID rid = rec.rid;

        int pageID = 1;
        int index;
        for (; ; pageID++) {

            BufType b = bpm->getPage(fileID, pageID, index);

            PageInfo* info = (PageInfo *) b;
            int used = info->used;
            if (used + recordSize < PAGE_SIZE) {
                BufType body = (BufType) &info->body;

                body += recordSize * rid.slot;

                rec.toBuffer(field, body);
                info->used += recordSize;
                break;
            }
        }
    }

    void deleteRecord(RecordID rid) {

        int index;
        BufType b = bpm->getPage(fileID, rid.page, index);

        PageInfo* info = (PageInfo *) b;
        BufType body = (BufType) &info->body;
        BufType r = body + recordSize * rid.slot;
        *(r + recordSize - 1) = 1;// lazy delete

        //  return Record(field, body, rid);
        //TODO
    }

    void updateRecord(Record rec) {
        RecordID rid = rec.rid;

        int index;
        BufType b = bpm->getPage(fileID, rid.page, index);

        PageInfo* info = (PageInfo *) b;
        BufType body = (BufType) &info->body;

        body += recordSize * rid.slot;

        rec.toBuffer(field, body);
    }

    void forcePages(int pageID = -1) {
        if (pageID == -1)
            bpm->close();
        else {
            int index;
            int fileID;
            bpm->getPage(fileID, pageID, index);
            bpm->writeBack(index);
        }
    }
};

class RecordManager {
private:
    BufPageManager *bpm;
    FileManager *fm;
public:
    RecordManager() {
        fm = new FileManager();
        bpm = new BufPageManager(fm);
    }            // Constructor
    ~RecordManager() {
        delete fm;
        delete bpm;
    }                           // Destructor

    void createFile(const char *fileName, Field field) {
        int fileID;
        //this->field = field;
        fm->createFile(fileName);
        fm->openFile(fileName, fileID);
        int pageID = 0, index;
        BufType b = bpm->allocPage(fileID, pageID, index, false);
        TableInfo *info = (TableInfo *) b;
        strcpy(info->tableName, fileName);
        int size = info->size = field.size();
        int recordSize = 0;
        BufType body = (BufType) &info->body;
        char *fieldName = (char *) body;
        for (int i = 0; i < size; ++i) {
            strcpy(fieldName, field.getFieldName(i).c_str());
            fieldName += MAX_FIELD_LENGTH;
            *((DataType *) fieldName) = field.getFieldType(i);
            fieldName += 1;
            *((int *) fieldName) = field.getFieldSize(i);
            fieldName += 1;
            recordSize += field.getFieldSize(i);
        }
        info->recordSize = recordSize;
        bpm->markDirty(index);
        bpm->close();
        fm->closeFile(fileID);
    }

    // Create a new file
    void destroyFile(const char *fileName) {
        fm->destroyFile(fileName);
    }     // Destroy a file

    RecordHandle openFile(const char *fileName) {
        FileManager *_fm = new FileManager();
        BufPageManager *_bpm = new BufPageManager(_fm);
        int fileID;
        _fm->openFile(fileName, fileID);
        int pageID = 0, index;
        BufType b = _bpm->getPage(fileID, pageID, index);
        TableInfo *info = (TableInfo *) b;
        int size = info->size;
        int recordSize = info->recordSize;
        Field field;
        field.list.clear();
        BufType body = (BufType) &info->body;
        char *fieldName = (char *) body;
        for (int i = 0; i < size; ++i) {
            string field_name(fieldName);
            fieldName += MAX_FIELD_LENGTH;
            DataType field_type = *((DataType *) fieldName);
            fieldName += 1;
            int field_size = *((int *) fieldName);
            fieldName += 1;
            field.list.push_back(make_pair(field_name, field_type));
            field.nums.push_back(field_size);
        }
        return RecordHandle(fileID, field, recordSize, _bpm, _fm);
    }
    // Open a file

    void closeFile(RecordHandle *handle) {
        handle->bpm->close();
        handle->fm->closeFile(handle->fileID);
    }  // Close a file
};




#endif //DATABASE_RECORDMANAGER_H
