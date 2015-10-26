/*
 * testfilesystem.cpp
 *
 *  Created on: 2015年10月6日
 *      Author: lql
 */
#include "./bufmanager/BufPageManager.h"
#include "./recordmanager/recordManager.h"

using namespace std;

int main() {
    FileManager* fm = new FileManager();
    BufPageManager* bpm = new BufPageManager(fm);
    fm->createFile("testfile.txt"); //新建文件
    RecordManager *recordManager = new RecordManager(bpm);
    recordManager->insert(*(new Record()));
    recordManager->insert(*(new Record()));
    recordManager->update(*(new Query), *(new Query));

    return 0;
}