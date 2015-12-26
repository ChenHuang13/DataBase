//
// Created by huangsy13 on 12/21/15.
//

#ifndef PARSER__H_
#define PARSER__H_

#include <iostream>
#include "define.h"

#define START 0
#define INSERT 1
#define CREATE_TB 2
#define EOCTB 3


using namespace std;

struct Paser{
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

    char* getWord(char*& s, char e) {
        while (*s == ' ') ++ s;
        if (*s == '\0') {
            return NULL;
        }
        char* t = s;
        while (*t != e) ++ t;
        char* k = t - 1;
        while (*k == ' ') --k;
        *(k + 1) = '\0';
        return t;
    }

    bool isCmp(char k) {
        return (k == '>' || k == '<' || k == '=');
    }

    bool isCha(char k) {
        return (('a' <= k && k <= 'z') || ('A' <= k && k <= 'Z'));
    }

    bool isDig(char k) {
        return '0' <= k && k <= '9';
    }

    void exec_where(char* sql) {
        errwhere = 0;
        tab1.clear();
        tab2.clear();
        col1.clear();
        col2.clear();
        op.clear();
        int n = tabs.size();
        char* prev, *next;
        prev = sql;
        for (int i = 0; i < n; ++ i) tabs[i]->rangeAll();
        //int kkk = 0;
        while (true) {
            while (*prev == ' ') ++ prev;
            next = prev + 1;
            int cp, cc;
            while (*next != '.' && ! isCmp(*next) && *next != ' ') ++ next;
            if (*next == '.') {
                *next = '\0';
                ctb = cdbs->getTable(prev);
                if (ctb == NULL) {
                    errwhere = 1;
                    printf("error: no table\n");
                    return;
                }
                prev = next + 1;
                next = prev + 1;
                while (! isCmp(*next) && *next != ' ') ++ next;
            }
            char ch = *next;
            *next = '\0';
            //cc = ctb->cmap[prev];
            cc = ctb->getcolid(prev);
            if (cc == -1) {
                errwhere = 1;
                printf("error: no column\n");
                return;
            }
            //	cout <<++kkk <<" "<<ctb->tn <<"."<<prev<<endl;
            *next = ch;
            while (*next == ' ') ++ next;
            //	cout <<*next<<endl;
            switch (*next) {
                case '=':
                    cp = E;
                    ++next;
                    break;
                case '>':
                    ++ next;
                    if (*next == '=') {
                        cp = GE;
                        ++next;
                    } else {
                        cp = G;
                    }
                    break;
                case '<':
                    ++ next;
                    if (*next == '=') {
                        cp = LE;
                        ++next;
                    } else {
                        cp = L;
                    }
                    break;
                case 'i':
                    cp = IS;
                    next += 2;
            }
            //	cout <<*next<<endl;
            while (*next == ' ') ++ next;
            prev = next;
            char brk;
            //	cout << cp << endl;
            if (*prev == '\'') {
                //st const
                prev ++;
                next = prev;
                while (*next != '\'') ++ next;
                *next = '\0';
                getRange(prev, strlen(prev) + 1, ctb->col[cc].cb.cl, ctb->col[cc].cb.dt, cp);
                ctb->merge(cc, cr);
                while (*next != ' ' && *next != ';') ++ next;
                brk = *next;
            } else if ( isCha(*prev)) {
                next = prev + 1;
                while (*next != '.' && *next != ' ' && *next != ';') ++ next;
                brk = *next;
                *next = '\0';
                if (strcmp(prev, "null") == 0) {
                    //null const
                    cr.rt = ISNULL;
                    ctb->merge(cc, cr);
                } else {
                    // connect
                    tab1.push_back(ctb);
                    col1.push_back(cc);
                    ctb = cdbs->getTable(prev);
                    if (ctb == NULL) {
                        errwhere = 1;
                        printf("error:no table\n");
                        return;
                    }
                    tab2.push_back(ctb);
                    op.push_back(cp);
                    prev = next + 1;
                    next ++;
                    while (*next != ' ' && *next != ';') ++ next;
                    brk = *next;
                    *next = '\0';
                    int kkk = ctb->getcolid(prev);
                    if (kkk == -1) {
                        errwhere = 1;
                        printf("error:no column\n");
                        return;
                    }
                    col2.push_back(kkk);
                    //	cout <<"connect\t"<<ctb->tn<<"."<<prev<<endl;
                }
            } else {
                // ll / db const
                next = prev + 1;
                while (*next != ' ' && *next != ';') ++ next;
                brk = *next;
                *next = '\0';
                void* src;
                if (ctb->col[cc].cb.dt == LL_TYPE) {
                    ll key = atoll(prev);
                    src = &key;
                    getRange(src, 8, 8, ctb->col[cc].cb.dt, cp);
                    //		cout << key << endl;
                    //exit(0);
                } else {
                    getRange(src, 8, 8, ctb->col[cc].cb.dt, cp);
                    db keyd = atof(prev);
                    src = &keyd;
                    getRange(src, 8, 8, ctb->col[cc].cb.dt, cp);
                }
                ctb->merge(cc, cr);
            }
            if (brk == ';') break;
            //	if (brk == ' ') cout <<"space"<<endl;
            // and
            next ++;
            prev = getWord(next, ' ');
            prev ++;
        }
    }

    void getCost() {
        int n = tabs.size();
        for (int i = 0; i < n; ++ i) {
            cost[i] = INT_MAX;
            idx[i] = -1;
            TableInfo* tb = tabs[i];
            for (int j = 0; j < tb->cn; ++ j) {
                if (tb->col[j].bpt[0] != NULL) {
                    int k = tb->rangeCount(j);
                    if (k < cost[i]) {
                        cost[i] = k;
                        idx[i] = j;
                    }
                }
            }
        }
        ctb = tabs[0];
        cidx = idx[0];
        if (n == 1) return;
        if (cost[0] > cost[1]) {
            ctb = tabs[1];
            stb = tabs[0];
            cidx = idx[1];
            sidx = idx[0];
        } else {
            stb = tabs[1];
            ctb = tabs[0];
            cidx = idx[0];
            sidx = idx[1];
        }
        if (n == 2) return;
        ttb = tabs[2];
        tidx = idx[2];
    }
}paser;

#endif
