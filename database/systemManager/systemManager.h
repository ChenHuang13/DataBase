//
// Created by huangsy13 on 12/21/15.
//

#ifndef DATABASE_SYSTEMMANAGER_H
#define DATABASE_SYSTEMMANAGER_H

#include <iostream>
#include "../utils/pagedef.h"
#include "../recordManager/ItemList.h"
#include "../recordManager/layout.h"
#include "../utils/MyBitMap.h"
#include "../systemManager/DB.h"
#include <cstring>
#include <set>
#include <limits.h>

#include "../queryManager/select.h"
#include "../queryManager/insert.h"
#include "../queryManager/delete.h"
#include "../queryManager/update.h"
#include "../define.h"
#include "../tool.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

//系统管理模块，用于创建数据库，删除数据库，使用数据库和关闭数据库

class SystemManager{
public:
    //创建一个数据库
    bool createdb(char* name) {
        multiset<char*, Cmp>::iterator it = dbs.find(name);
        if (it != dbs.end()) {
            return false;
        }
        fm->createFile(name);
        int l = strlen(name);
        char* n = new char[l + 1];
        strcpy(n, name);
        dbs.insert(n);
        char* fn = new char[l + 5];
        strcpy(fn, n);
        strcpy(fn + l, ".txt");
        fm->createFile(fn);
        int fd = open(fn, O_RDWR);
        int k = 0;
        write(fd, &k, sizeof(int));
        write(fd, &k, sizeof(int));
        close(fd);
        delete[] fn;
        return true;
    }

    //删除一个数据库
    bool dropdb(char* name) {
        multiset<char*, Cmp>::iterator it = dbs.find(name);
        if (it == dbs.end()) {
            return false;
        }
        if (cdbs != NULL && strcmp(cdbs->dname, name) == 0) {
            cdbs->dropDB();
            delete cdbs;
            cdbs = NULL;
        }
        char* n = *it;
        int l = strlen(n);
        char* fn = new char[l + 5];
        strcpy(fn, n);
        strcpy(fn + l, ".txt");
        remove(n);
        remove(fn);
        dbs.erase(it);
        delete[] n;
        delete[] fn;
        return true;
    }

    //关闭一个数据库
    bool closedb() {
        if (cdbs == NULL) return false;
        cdbs->closeDB();
        delete cdbs;
        cdbs = NULL;
        return true;
    }

    //使用一个数据库
    bool usedb(char* name) {
        multiset<char*, Cmp>::iterator it = dbs.find(name);
        if (it == dbs.end()) {
            return false;
        }
        if (cdbs != NULL){
            if (strcmp(cdbs->dname, name) == 0) return false;
            closedb();
        }
        cdbs = new DB(
                name, fm, bpm, bpl
        );
        return true;
    }

};

#endif //DATABASE_SYSTEMMANAGER_H
