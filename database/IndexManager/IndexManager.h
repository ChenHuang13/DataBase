//
// Created by huangsy13 on 11/13/15.
//

//IndexManager：索引管理类，提供创建，删除，打开，关闭索引

#ifndef DATABASE_INDEXMANAGER_H
#define DATABASE_INDEXMANAGER_H

#include "../RecordManager/RecordManager.h"

class IndexManager{
private:
    BufPageManager *bpm;
    FileManager *fm;
public:

    string getIndexFIleName(const char *fileName, int indexNo){
        string fileStr(fileName);
        fileStr += "." + numToStr(indexNo);
        return fileStr;
    }

    IndexManager(){
        fm = new FileManager();
        bpm = new BufPageManager(fm);
    }

    void createIndex(const char *fileName, int indexNo , DataType type , int typeLength){
        int fileID;
        fm->createFile(getIndexFIleName(fileName,indexNo).c_str());
        fm->openFile(fileName, fileID);

        int pageID = 0, index;
        BufType b = bpm->allocPage(fileID, pageID, index, false);



        bpm->close();
        fm->closeFile(fileID);
    }

    void destroyIndex(const char *fileName, int indexNo){
        fm->destroyFile(getIndexFIleName(fileName,indexNo).c_str());
    }

    void openIndex(){}

    void closeIndex(){}
};



#endif //DATABASE_INDEXMANAGER_H
