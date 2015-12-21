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
#include <cstring>
#include <set>
#include "parser/parser.h"
#include <limits.h>

#include "select.h"
#include "insert.h"
#include "delete.h"
#include "update.h"
#include "define.h"
#include "tool.h"




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
void init() {
    MyBitMap::initConst();
    create.name = new char*[MAX_COL_NUM];
    create.c = new CB[MAX_COL_NUM];
    cr.a = new uchar[1000];
    cr.b = new uchar[1000];
}
void load() {
    dbs.clear();
    int fd = open("sys.b", O_RDWR);
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
void clearAll() {
    fm->createFile("sys.b");
    int fd = open("sys.b", O_RDWR);
    int n = 0;
    write(fd, &n, sizeof(int));
    close(fd);
}
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


void closeDataBase() {
    if (cdbs != NULL) {
        cdbs->closeDB();
        delete cdbs;
    }
    multiset<char*, Cmp>::iterator it;
    int fd = open("sys.b", O_RDWR);
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

void execute(char* sql) {
    int len = strlen(sql);
    char* prev, *next;
    prev = sql;
    switch (state) {
        case START:
            next = getWord(prev, ' ');
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
                next = getWord(prev, ';');
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
                next = getWord(prev, ' ');
                if (strcmp(prev, "INTO") == 0) {
                    prev = next + 1;
                    next = getWord(prev, ' ');
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
                next = getWord(prev, ' ');
                if (strcmp(prev, "INDEX") == 0) {
                    if (cdbs == NULL) {
                        printf("error:no db\n");
                        return;
                    }
                    prev = next + 1;
                    next = getWord(prev, '(');
                    Table* tb = cdbs->getTable(prev);
                    if (tb == NULL) {
                        printf("error:no table\n");
                        return;
                    }
                    prev = next + 1;
                    next = getWord(prev, ')');
                    if (!tb->createIndex(prev)) {
                        printf("error:in creating index\n");
                        return;
                    }
                } else if (strcmp(prev, "TABLE") == 0) {
                    prev = next + 1;
                    next = getWord(prev, '(');
                    create.nl = strlen(prev) + 1;
                    create.tn = new char[create.nl];
                    strcpy(create.tn, prev);
                    state = CREATE_TB;
                    create.n = 0;
                    primary = -1;
                    return;
                } else if (strcmp(prev, "DATABASE") == 0) {
                    prev = next + 1;
                    next = getWord(prev, ';');
                    if (!createdb(prev)) printf("error4");
                    return;
                }
            } else if (strcmp(prev, "USE") == 0) {
                prev = next + 1;
                next = getWord(prev, ';');
                if (!usedb(prev)) {
                    printf("error\n");
                    return;
                }
            } else if (strcmp(prev, "DROP") == 0) {
                prev = next + 1;
                next = getWord(prev, ' ');
                if (strcmp(prev, "TABLE") == 0) {
                    prev = next + 1;
                    next = getWord(prev, ';');
                    if (cdbs == NULL || !cdbs->dropTB(prev)) {
                        printf("error\n");
                    }
                } else if (strcmp(prev, "INDEX") == 0) {
                    if (cdbs == NULL){
                        printf("error: no db\n");
                        return;
                    }
                    prev = next + 1;
                    next = getWord(prev, '(');
                    Table* tb = cdbs->getTable(prev);
                    if (tb == NULL) {
                        printf("error: no table\n");
                        return;
                    }
                    prev = next + 1;
                    next = getWord(prev, ')');
                    if (!tb->dropIndex(prev)) {
                        printf("error: in droping index\n");
                        return;
                    }
                } else {
                    prev = next + 1;
                    next = getWord(prev, ';');
                    if (!dropdb(prev)) {
                        printf("error\n");
                    }
                }
            }
            break;
        case CREATE_TB:
            next = getWord(prev, ' ');
            if (strcmp(prev, "PRIMARY") == 0) {
                prev = next + 1;
                next = getWord(prev, '(');
                prev = next + 1;
                next = getWord(prev, ')');
                int n = create.n, i;
                for (i = 0; i < n; ++ i) if (strcmp(create.name[i], prev) == 0) break;
                create.c[i].ut = UNI;
                create.c[i].nt = UN;
                primary = i;
            } else if (strcmp(prev, "UNIQUE") == 0) {
                prev = next + 1;
                next = getWord(prev, '(');
                prev = next + 1;
                next = getWord(prev, ')');
                int n = create.n, i;
                for (i = 0; i < n; ++ i) if (strcmp(create.name[i], prev) == 0) break;
                create.c[i].ut = UNI;
            } else {
                int n = create.n;
                create.c[n].nl = strlen(prev) + 1;
                create.name[n] = new char[create.c[n].nl];
                strcpy(create.name[n], prev);
                prev = next + 1;
                next = getWord(prev, '(');
                create.c[n].nt = N;
                create.c[n].ut = UNUNI;
                switch (*prev) {
                    case 'v':
                    case 'c':
                        create.c[n].dt = ST_TYPE;
                        prev = next + 1;
                        next = getWord(prev, ')');
                        create.c[n].cl = atoi(prev) + 1;
                        break;
                    case 'i':
                        create.c[n].dt = LL_TYPE;
                        prev = next + 1;
                        next = getWord(prev, ')');
                        create.c[n].cl = 8;
                        break;
                    default:
                        create.c[n].dt = DB_TYPE;
                        prev = next + 1;
                        next = getWord(prev, ')');
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


void fileInput(string fileName) {
    fin = fopen( ("../sql/"+fileName).c_str() , "r" );
        

    char* str = fgets(buf, 1000, fin);
    state = START;
    while (str != NULL) {
        int len = strlen(buf);
        while (len > 0 && (buf[len - 1] == '\n' || buf[len - 1] == 13)) {
            buf[len - 1] = '\0';
            len --;
        }
        if (len != 0) {
            execute(buf);
        }
        str = fgets(buf, 1000, fin);
    }
    fclose(fin);
}

void test(){
    clearAll();
    load();
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
    closeDataBase();
}

#endif //DATABASE_DATABASEMANAGER_H
