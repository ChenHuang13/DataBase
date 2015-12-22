//
// Created by huangsy13 on 12/21/15.
//

#ifndef DATABASE_DATABASEMANAGER_H
#define DATABASE_DATABASEMANAGER_H

#define COL_NAME 0
#include <iostream>
#include "utils/pagedef.h"
#include "recordManager/ItemList.h"
#include "recordManager/layout.h"
#include "utils/MyBitMap.h"
#include "systemManager/DB.h"
#include "systemManager/systemManager.h"
#include <cstring>
#include <set>
#include "parser.h"
#include <limits.h>

#include "select.h"
#include "insert.h"
#include "delete.h"
#include "update.h"
#include "define.h"
#include "tool.h"

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

class DataBaseManager{
public:

    bool toTest;//是否进行测试
    SystemManager systemManager; //系统管理模块
    //数据库进行初始化
    void init() {
        MyBitMap::initConst();
        create.name = new char*[MAX_COL_NUM];
        create.c = new CB[MAX_COL_NUM];
        cr.a = new uchar[1000];
        cr.b = new uchar[1000];
        fm = new FileManager();
        bpm = new BufPageManager(fm);
        bpl = new MyLinkList(CAP, MAX_TYPE_NUM);
    }

    //选择是否进行测试和选择是否清空以前保存下来的数据库
    void seletModel(){
        char ans;
        cout << "进行自动测试? y/n" << endl;
        cin >> ans;
        if (ans == 'y') {
            toTest = true;
            clearDataBase();
        }
        else {
            cout<<"清空以前记录? y/n" << endl;
            cin >> ans;
            if (ans == 'y') {
                clearDataBase();
            }
        }
        loadDataBase();
    }

    //进行测试或者不断读入sql文件
    void run(){
        if (toTest){
            cout << "开始测试！"<<endl;
            fileInput("test.sql");
            fileInput("book.sql");
            fileInput("orders.sql");
            fileInput("select.sql");
            fileInput("join.sql");
            fileInput("delete.sql");
            fileInput("join.sql");
            fileInput("update.sql");
            cout << "测试完成！"<< endl;
        }
        else{
            string fileName;
            while (true) {
                cout << "请输入需要载入的包含sql语句的文件名（退出输入exit）：";
                cin >> fileName;
                if (fileName == "exit") break;
                else fileInput(fileName);
            }
        }
    }

    //退出数据库并写回
    void quit(){
        if (cdbs != NULL) {
            cdbs->closeDB();
            delete cdbs;
        }
        multiset<char*, Cmp>::iterator it;
        int fd = open("DataBaseInfo.db", O_RDWR);
        int k = dbs.size();
        write(fd, &k, sizeof(int));
        for (it = dbs.begin(); it != dbs.end(); it++) {
            char* n = *it;
            int len = strlen(n) + 1;
            write(fd, &len, sizeof(int));
            write(fd, n, len);
        }
        close(fd);
        dbs.clear();
        cout << "关闭数据库成功！" << endl;
        exit(0);
    }

    //从DataBaseInfo.db加载当前数据库信息
    void loadDataBase() {
        dbs.clear();
        int fd = open("DataBaseInfo.db", O_RDWR);
        int n;
        read(fd, &n, sizeof(int));
        for (int i = 0; i < n; ++ i) {
            int m;
            read(fd, &m, sizeof(int));
            char* buf = new char[m];
            read(fd, buf, m);
            dbs.insert(buf);
        }
        close(fd);
    }

    //清空数据库信息
    void clearDataBase() {
        fm->createFile("DataBaseInfo.db");
        int fd = open("DataBaseInfo.db", O_RDWR);
        int n = 0;
        write(fd, &n, sizeof(int));
        close(fd);
    }

    void getl(void* x, bool e, uchar* a) {
        memcpy(a, x, 8);
        if (e) {
            memset(a + 8, 0xff, 8);
        } else {
            memset(a + 8, 0x7f, 8);
        }
    }
    void getr(void* x, bool e, uchar* a) {
        memcpy(a, x, 8);
        if (e) {
            memset(a + 8, 0x7f, 8);
        } else {
            memset(a + 8, 0xff, 8);
        }
    }
    void getsl(char* x, bool e, int len, uchar* a) {
        strcpy((char*)a, x);
        if (e) {
            memset(a + len, 0xff, 8);
        } else {
            memset(a + len, 0x7f, 8);
        }
    }
    void getsr(char* x, bool e, int len, uchar* a) {
        strcpy((char*)a, x);
        if (e) {
            memset(a + len, 0x7f, 8);
        } else {
            memset(a + len, 0xff, 8);
        }
    }

    void execute(char* sql) {
        int len = strlen(sql);
        char* prev, *next;
        prev = sql;
        switch (state) {
            case START:
                next = paser.getWord(prev, ' ');
                if (strcmp(prev, "SHOW") == 0) {
                    if (cdbs == NULL) {
                        printf("error: no db\n");
                        return;
                    }
                    cdbs->printtb();
                    return;
                }
                if (strcmp(prev, "DESC") == 0) {
                    if (cdbs == NULL) {
                        printf("error: no db\n");
                        return;
                    }
                    prev = next + 1;
                    next = paser.getWord(prev, ';');
                    Table* tb = cdbs->getTable(prev);
                    if (tb == NULL) {
                        printf("error: no table\n");
                        return;
                    }
                    int kk = tb->cn;
                    for (int i = 0; i < kk; ++ i) {
                        printf("name:%s\t", tb->col[i].name);
                        printf("len:%d\t", tb->col[i].cb.cl);
                        int kkk = tb->col[i].cb.dt;
                        if (kkk == LL_TYPE) {
                            printf("type: integer\n");
                        }
                        if (kkk == DB_TYPE) {
                            printf("type: float\n");
                        }
                        if (kkk == ST_TYPE) {
                            printf("type: string\n");
                        }
                    }
                    return;
                }
                if (strcmp(prev, "INSERT") == 0) {
                    prev = next + 1;
                    next = paser.getWord(prev, ' ');
                    if (strcmp(prev, "INTO") == 0) {
                        prev = next + 1;
                        next = paser.getWord(prev, ' ');
                    }
                    ctb = NULL;
                    if (cdbs != NULL) ctb = cdbs->getTable(prev);
                    next ++;
                    while (*next != '(' && *next != '\0') ++ next;
                    state = INSERT;
                    if (ctb == NULL) {
                        printf("error: no table\n");
                    }
                    if (*next != '\0') exec_insert(next);
                } else if (strcmp(prev, "SELECT") == 0) {
                    exec_select(next + 1);
                } else if (strcmp(prev, "DELETE") == 0) {
                    exec_delete(next + 1);
                } else if (strcmp(prev, "UPDATE") == 0) {
                    exec_update(next + 1);
                } else if (strcmp(prev, "CREATE") == 0) {
                    prev = next + 1;
                    next = paser.getWord(prev, ' ');
                    if (strcmp(prev, "INDEX") == 0) {
                        if (cdbs == NULL) {
                            printf("error:no db\n");
                            return;
                        }
                        prev = next + 1;
                        next = paser.getWord(prev, '(');
                        Table* tb = cdbs->getTable(prev);
                        if (tb == NULL) {
                            printf("error:no table\n");
                            return;
                        }
                        prev = next + 1;
                        next = paser.getWord(prev, ')');
                        if (!tb->createIndex(prev)) {
                            printf("error:in creating index\n");
                            return;
                        }
                    } else if (strcmp(prev, "TABLE") == 0) {
                        prev = next + 1;
                        next = paser.getWord(prev, '(');
                        create.nl = strlen(prev) + 1;
                        create.tn = new char[create.nl];
                        strcpy(create.tn, prev);
                        state = CREATE_TB;
                        create.n = 0;
                        primary = -1;
                        return;
                    } else if (strcmp(prev, "DATABASE") == 0) {
                        prev = next + 1;
                        next = paser.getWord(prev, ';');
                        if (!systemManager.createdb(prev)) printf("error4");
                        return;
                    }
                } else if (strcmp(prev, "USE") == 0) {
                    prev = next + 1;
                    next = paser.getWord(prev, ';');
                    if (!usedb(prev)) {
                        printf("error\n");
                        return;
                    }
                } else if (strcmp(prev, "DROP") == 0) {
                    prev = next + 1;
                    next = paser.getWord(prev, ' ');
                    if (strcmp(prev, "TABLE") == 0) {
                        prev = next + 1;
                        next = paser.getWord(prev, ';');
                        if (cdbs == NULL || !cdbs->dropTB(prev)) {
                            printf("error\n");
                        }
                    } else if (strcmp(prev, "INDEX") == 0) {
                        if (cdbs == NULL){
                            printf("error: no db\n");
                            return;
                        }
                        prev = next + 1;
                        next = paser.getWord(prev, '(');
                        Table* tb = cdbs->getTable(prev);
                        if (tb == NULL) {
                            printf("error: no table\n");
                            return;
                        }
                        prev = next + 1;
                        next = paser.getWord(prev, ')');
                        if (!tb->dropIndex(prev)) {
                            printf("error: in droping index\n");
                            return;
                        }
                    } else {
                        prev = next + 1;
                        next = paser.getWord(prev, ';');
                        if (!systemManager.dropdb(prev)) {
                            printf("error\n");
                        }
                    }
                }
                break;
            case CREATE_TB:
                next = paser.getWord(prev, ' ');
                if (strcmp(prev, "PRIMARY") == 0) {
                    prev = next + 1;
                    next = paser.getWord(prev, '(');
                    prev = next + 1;
                    next = paser.getWord(prev, ')');
                    int n = create.n, i;
                    for (i = 0; i < n; ++ i) if (strcmp(create.name[i], prev) == 0) break;
                    create.c[i].ut = UNI;
                    create.c[i].nt = UN;
                    primary = i;
                } else if (strcmp(prev, "UNIQUE") == 0) {
                    prev = next + 1;
                    next = paser.getWord(prev, '(');
                    prev = next + 1;
                    next = paser.getWord(prev, ')');
                    int n = create.n, i;
                    for (i = 0; i < n; ++ i) if (strcmp(create.name[i], prev) == 0) break;
                    create.c[i].ut = UNI;
                } else {
                    int n = create.n;
                    create.c[n].nl = strlen(prev) + 1;
                    create.name[n] = new char[create.c[n].nl];
                    strcpy(create.name[n], prev);
                    prev = next + 1;
                    next = paser.getWord(prev, '(');
                    create.c[n].nt = N;
                    create.c[n].ut = UNUNI;
                    switch (*prev) {
                        case 'v':
                        case 'c':
                            create.c[n].dt = ST_TYPE;
                            prev = next + 1;
                            next = paser.getWord(prev, ')');
                            create.c[n].cl = atoi(prev) + 1;
                            break;
                        case 'i':
                            create.c[n].dt = LL_TYPE;
                            prev = next + 1;
                            next = paser.getWord(prev, ')');
                            create.c[n].cl = 8;
                            break;
                        default:
                            create.c[n].dt = DB_TYPE;
                            prev = next + 1;
                            next = paser.getWord(prev, ')');
                            create.c[n].cl = 8;
                    }
                    next ++;
                    if (*next == ' ') {
                        //create.c[n].cl += 8;
                        create.c[n].nt = UN;
                    }
                    create.n ++;
                }
                if (sql[len - 1] != ',') {
                    state = EOCTB;
                }
                break;
            case EOCTB:
                state = START;
                if (cdbs == NULL) {
                    printf("error\n");
                    return;
                }
                if (cdbs->createTB(&create)) {
                    Table* tb = cdbs->getTable(create.tn);
                    if (primary == -1) primary = 0;
                    tb->createIndex(create.name[primary]);
                } else {
                    printf("error\n");
                }
                break;
            case INSERT:
                exec_insert(sql);
                break;
            default: break;
        }
    }

    bool closedb() {
        if (cdbs == NULL) return false;
        cdbs->closeDB();
        delete cdbs;
        cdbs = NULL;
        return true;
    }

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

    void fileInput(string fileName) {
        fin = fopen( ("../sql/" + fileName).c_str() , "r" );
        char *str = fgets(buf, 1000, fin);
        state = START;
        while (str != NULL) {
            int len = strlen(buf);
            while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == 13)) {
                buf[len - 1] = '\0';
                len--;
            }
            if (len != 0) {
                execute(buf);
            }
            str = fgets(buf, 1000, fin);
        }
        fclose(fin);
    }
};

#endif //DATABASE_DATABASEMANAGER_H
