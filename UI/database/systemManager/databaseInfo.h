//
// Created by huangsy13 on 12/23/15.
//

#ifndef DATABASE_DATABASEINFO_H
#define DATABASE_DATABASEINFO_H

//这里是不同数据库操作底层接口

#include <map>
#include "tableInfo.h"
#include <vector>
#include <fstream>

using namespace std;

struct DatabaseInfo {

    FileManager* fm;
    BufPageManager* bpm;

    MyLinkList* bpl;

    NodeManager* nm;

    int fid;
    char* dname;
    char* fn;

    //表名和表类的映射
    map<char*, TableInfo *, Cmp> cmap;

    //由数据库的名字读取表的信息，形成表名和表类的映射
    DatabaseInfo(char* name , FileManager* fmg , BufPageManager* bpmg , MyLinkList* bpList ) {
        int l = strlen(name);
        dname = new char[l + 1];
        strcpy(dname, name);
        fn = new char[l + 5];
        strcpy(fn, dname);
        strcpy(fn + l, ".txt");
        fm = fmg;
        bpm = bpmg;
        bpl = bpList;
        fm->openFile(dname, fid);
        int fd = open(fn, O_RDWR);
        int nodenum;
        read(fd, &nodenum, 4);
        nm = new NodeManager(
                bpl, fm, bpm, fid, nodenum
        );
        int tn;
        read(fd, &tn, 4);
        cmap.clear();
        for (int i = 0; i < tn; ++ i) {
            TableInfo * tb = new TableInfo(fid, nm, false, &fd);
            cmap[tb->tn] = tb;
        }
        close(fd);
    }

    //创建表
    bool createTB(Create* c) {
        map<char*, TableInfo *, Cmp>::iterator it = cmap.find(c->tn);
        if (it != cmap.end()) {
            return false;
        }
        TableInfo * table = new TableInfo(
                fid, nm, true, c
        );
        cmap[c->tn] = table;
        return true;
    }

    //根据表明返回表类
    TableInfo * getTable(char* name) {
        map<char*, TableInfo *, Cmp>::iterator it = cmap.find(name);
        if (it == cmap.end()) {
            return NULL;
        }
        return it->second;
    }

    //删除表
    bool dropTB(char* name) {
        map<char*, TableInfo *, Cmp>::iterator it = cmap.find(name);
        if (it == cmap.end()) {
            return false;
        }
        TableInfo * tb = it->second;
        tb->closeTB(false);
        cmap.erase(it);
        delete tb;
        return true;
    }

    //删除数据库
    void dropDB() {
        map<char*, TableInfo *, Cmp>::iterator it;
        for(it = cmap.begin(); it != cmap.end();) {
            TableInfo * t = it->second;
            t->closeTB(false);
            cmap.erase(it++);
            delete t;
        }
        fm->closeFile(fid);
    }

    //关闭数据库
    void closeDB() {
        int fd = open(fn, O_RDWR);
        write(fd, &(nm->num), sizeof(int));
        int n = cmap.size();
        write(fd, &n, sizeof(int));
        map<char*, TableInfo *, Cmp>::iterator it;
        for (it = cmap.begin(); it != cmap.end();) {
            TableInfo * t = it->second;
            t->closeTB(true);
            t->save(fd);
            cmap.erase(it++);
            delete t;
        }
        close(fd);
        fm->closeFile(fid);
    }

    //打印表名
    void printtb() {
        printf("%d\n", int(cmap.size()));
        map<char*, TableInfo *, Cmp>::iterator it;
        for (it = cmap.begin(); it != cmap.end(); ++ it) {
            printf("%s ", it->second->tn);
        }
        printf("\n");
    }

    ~DatabaseInfo() {
        delete[] dname;
        delete[] fn;
    }
};

#endif