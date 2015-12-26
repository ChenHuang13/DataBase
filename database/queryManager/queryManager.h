//
// Created by huangsy13 on 12/23/15.
//

#ifndef DATABASE_QUERYMANAGER_H
#define DATABASE_QUERYMANAGER_H

#include "../define.h"
#include "../parser.h"
#include "print.h"
#include "../tool.h"

enum extop {
    extop_nop, extop_sum, extop_average, extop_max, extop_min
};

class QueryManager {
public:
    static void exec_delete(char* sql) {
        char* prev, *next;
        prev = sql;
        next = paser.getWord(prev, ' ');
        prev = next + 1;
        next = paser.getWord(prev, ' ');
        *next = '\0';
        if (cdbs == NULL) {
            printf("error: no db\n");
            return;
        }
        ctb = cdbs->getTable(prev);
        if (ctb == NULL) {
            cout << "表 " << prev << " 不存在\r";
            return;
        }
        tabs.clear();
        tabs.push_back(ctb);
        prev = next + 1;
        next = paser.getWord(prev, ' ');
        paser.exec_where(next + 1);
        if (errwhere == 1) {
            return;
        }
        cidx = -1;
        int w = -1;
        for (int j = 0; j < ctb->cn; ++j) {
            if (ctb->col[j].bpt[0] != NULL) {
                if (w == -1 || w > ctb->rangeCount(j)) {
                    cidx = j;
                    w = ctb->rangeCount(j);
                }
            }
        }
        ans1.clear();
        ctb->select(cidx, ctb->range, ans1);
        int m = ans1.size();
        for (int i = 0; i < m; ++i) {
            int p = ans1[i].first;
            int s = ans1[i].second;
            ctb->erase(p, s);
        }
    }


    static void exec_insert(char *sql) {
        char *next = sql;
        while (*next == ' ') ++next;
        char *prev;
        int bm = 0;
        char *start = itemBuf + 4;
        if (ctb == NULL) return;
        int mc = ctb->cn - 1;
        for (int cc = 0; cc <= mc; ++cc) {
            const CB &cb = ctb->col[cc].cb;
            int dt = cb.dt;
            int nt = cb.nt;
            char e = (cc == mc) ? ')' : ',';
            int len = cb.cl;
            prev = next + 1;
            switch (dt) {
                case LL_TYPE:
                    next = paser.getWord(prev, e);
                    //printf("data\t%s\n", prev);
                    if (*prev == '\'') {
                        printf("error: insert string instead of int\n");
                        return;
                    }
                    if (*prev != 'n') {
                        long long d = atoll(prev);
                        memcpy(start, &d, len);
                    } else if (nt == N) {
                        bm ^= (1 << cc);
                    } else {
                        printf("error: insert null value, but not null type\n");
                        return;
                    }
                    break;
                case ST_TYPE:
                    while (*prev == ' ') ++prev;
                    if (*prev == '\'') {
                        next = prev + 1;
                        int k = 0;
                        while (*next != '\'') {
                            k++;
                            next++;
                        }
                        memcpy(start, prev + 1, k);
                        *(start + k) = '\0';
                        //printf("data\t%s\n", start);
                        while (*next != e) ++next;
                    } else if (*prev == 'n' && nt == N) {
                        bm ^= (1 << cc);
                        next = prev + 1;
                        while (*next != e) ++next;
                        //printf("data\t%s\n", next + 1);
                    } else {
                        printf("error:insert null string, but not null type\n");
                        return;
                    }
            }
            start += len;
        }
        memcpy(itemBuf, &bm, 4);
        int p, s;
        if (ctb != NULL) {
            bool ret = ctb->insert((uchar *) itemBuf, p, s);
            if (!ret) printf("error: repeated occurrence\n");
        } else printf("error\n");
        next++;
        while (*next == ' ') ++next;
        state = (*next == ';') ? START : INSERT;
    }

    static void exec_select(char *sql) {
        if (cdbs == NULL) {
            printf("error: no db\n");
            return;
        }
        extop extop_flag = extop_nop;
        while (sql[0] == ' ')
            ++sql;
        string s_sql(sql);
        if (s_sql.find("sum ") == 0)
            extop_flag = extop_sum, sql = sql + s_sql.find("sum ") + 4;
        else if (s_sql.find("average ") == 0)
            extop_flag = extop_average, sql = sql + s_sql.find("average ") + 8;
        else if (s_sql.find("max ") == 0)
            extop_flag = extop_max, sql = sql + s_sql.find("max ") + 4;
        else if (s_sql.find("min ") == 0)
            extop_flag = extop_min, sql = sql + s_sql.find("min ") + 4;
        ctb = NULL;
        tname.clear();
        cname.clear();
        tabs.clear();
        char *prev = sql;
        char *next;
        all = false;
        while (true) {
            next = prev + 1;
            while (*next != '.' && *next != ',' && *next != ' ') ++next;
            if (*next == '.') {
                *next = '\0';
                tname.push_back(prev);
                prev = next + 1;
                while (*next != ' ' && *next != ',') ++next;
            }
            if (*next == ',') {
                *next = '\0';
                cname.push_back(prev);
                prev = next + 1;
            } else {
                *next = '\0';
                cname.push_back(prev);
                prev = next + 1;
                break;
            }
        }
        if (cname.size() == 1 && *cname[0] == '*') all = true;
        while (*prev != ' ') ++prev;
        ++prev;
        while (true) {
            next = prev + 1;
            while (*next != ',' && *next != ' ') ++next;
            char ch = *next;
            *next = '\0';
            TableInfo *ttb = cdbs->getTable(prev);
            if (ttb == NULL) {
                cout << "表 " << prev << " 不存在\r";

                return;
            }
            tabs.push_back(ttb);
            prev = next + 1;
            if (ch == ' ') break;
        }
        int n = tabs.size();
        if (n == 1) ctb = tabs[0];
        int csizek = cname.size();
        for (int i = 0; i < csizek; ++i) {
            if (strcmp(cname[i], "*") == 0) break;
            if (n == 1) {
                if (ctb->getcolid(cname[i]) == -1) {
                    printf("error: no column\n");
                    return;
                }
            } else {
                TableInfo *ctt = cdbs->getTable(tname[i]);
                if (ctt == NULL) {
                    cout << "表 " << tname[i] << " 不存在\r";
                    return;
                }
                if (ctt->getcolid(cname[i]) == -1) {
                    printf("error: no column\n");
                    return;
                }
            }
        }
        next = paser.getWord(prev, ' ');
        paser.exec_where(next + 1);
        if (errwhere == 1) return;
        paser.getCost();
        ans1.clear();
        //cout << cidx << endl;
        ctb->select(cidx, ctb->range, ans1);
        int m = ans1.size();
        //cout << "ok" << endl;
        int countt = 0;
        if (n == 1) {
            query_sum = 0;
            query_max = llmin;
            query_min = llmax;
            print_flag = extop_flag == extop_nop;

            for (int i = 0; i < m; ++i) {
                int p = ans1[i].first;
                int s = ans1[i].second;
                //cout << p <<"\t"<<s<<"\t"<<i - m<<endl;
                uchar *it = ctb->getItem(p, s);
                if (all) printItem(ctb, it);
                else printCols1(it);
                if (extop_flag == extop_nop) printf("\n");
                countt++;
            }
            if (extop_flag == extop_nop) {
                printf("counted:%d\n", countt);
            } else if (extop_flag == extop_sum) {
                printf("sum:%lld\n", query_sum);
            } else if (extop_flag == extop_average) {
                printf("average:%lf\n", 1.0 * query_sum / countt);
            } else if (extop_flag == extop_max) {
                printf("max:%lld\n", query_max);
            } else if (extop_flag == extop_min) {
                printf("min:%lld\n", query_min);
            }
            return;
        }
        int cns;
        if (!all) {
            cns = cname.size();
            for (int i = 0; i < cns; ++i) {
                if (strcmp(tname[i], ctb->tn) == 0) {
                    isFirst[i] = true;
                    cid[i] = ctb->cmap[cname[i]];
                } else {
                    isFirst[i] = false;
                    cid[i] = stb->cmap[cname[i]];
                }
            }
        }
        int cn = tab2.size();
        for (int i = 0; i < stb->cn; ++i) connect[i] = false;
        for (int i = 0; i < cn; ++i) {
            if (strcmp(tab2[i]->tn, stb->tn) == 0) {
                TableInfo *t = tab2[i];
                tab2[i] = tab1[i];
                tab1[i] = t;
                int c = col1[i];
                col1[i] = col2[i];
                col2[i] = c;
                op[i] = (op[i] <= 3) ? 3 - op[i] : op[i];
            }
            connect[col1[i]] = true;
        }
        stb->copyRange(connect);
        for (int i = 0; i < m; ++i) {
            if (i != 0) stb->backRange(connect);
            int p = ans1[i].first;
            int s = ans1[i].second;
            uchar *it = ctb->getItem(p, s);
            int bm1;
            memcpy(&bm1, it, 4);
            uchar *cd;
            ans2.clear();
            for (int j = 0; j < cn; ++j) {
                const Col &col = ctb->col[col2[j]];
                cd = it + col.cs;
                getRange(cd, col.cb.cl, col.cb.cl, col.cb.dt, op[j]);
                stb->merge(col1[j], cr);
            }
            int idx = -1;
            int w = -1;
            for (int j = 0; j < stb->cn; ++j) {
                if (stb->col[j].bpt[0] != NULL) {
                    if (w == -1 || w > stb->rangeCount(j)) {
                        idx = j;
                        w = stb->rangeCount(j);
                    }
                }
            }
            stb->select(idx, stb->range, ans2);
            int m2 = ans2.size();
            for (int j = 0; j < m2; ++j) {
                countt++;
                int p2 = ans2[j].first;
                int s2 = ans2[j].second;
                uchar *it2 = stb->getItem(p2, s2);
                int bm2;
                memcpy(&bm2, it2, 4);
                if (all) {
                    printItem(ctb, it);
                    printItem(stb, it2);
                } else {
                    for (int k = 0; k < cns; ++k) {
                        if (isFirst[k]) {
                            bm = bm1;
                            printCol(ctb, cid[k], it);
                        } else {
                            bm = bm2;
                            printCol(stb, cid[k], it2);
                        }
                    }
                }
                printf("\n");
            }
        }
        printf("counted:%d\n", countt);
    }


    static void exec_update(char *sql) {
        if (cdbs == NULL) {
            printf("error : no db\n");
            return;
        }
        char *prev, *next;
        prev = sql;
        next = paser.getWord(prev, ' ');
        ctb = cdbs->getTable(prev);
        if (ctb == NULL) {
            cout << "表 " << prev << " 不存在\r";
            return;
        }
        prev = next + 1;
        next = paser.getWord(prev, ' ');
        int cc;
        int cn = 0;
        char *start = updatebuf;
        int cbm = 0;
        while (true) {
            prev = next + 1;
            next = paser.getWord(prev, '=');
            //cc = ctb->cmap[prev];
            cc = ctb->getcolid(prev);
            if (cc == -1) {
                printf("error: no column\n");
                return;
            }
            cid[cn++] = cc;
            const Col &col = ctb->col[cc];
            prev = next + 1;
            while (*prev == ' ') ++prev;
            char brk;
            switch (col.cb.dt) {
                case LL_TYPE:
                    if (*prev == '\'') {
                        printf("error: update a integer-type element as a string.");
                        return;
                    }
                    next = prev + 1;
                    while (*next != ' ' && *next != ',') ++next;
                    brk = *next;
                    *next = '\0';
                    if (*prev == 'n') {
                        if (col.cb.nt == UN) {
                            printf("error: update a unnull-type element as null.");
                            return;
                        }
                        cbm ^= (1 << cc);
                        start += 8;
                    } else {
                        ll key = atoll(prev);
                        memcpy(start, &key, 8);
                        start += 8;
                    }
                    break;
                case ST_TYPE:
                    if (*prev == '\'') {
                        prev++;
                        next = prev;
                        while (*next != '\'') ++next;
                        *next = '\0';
                        strcpy(start, prev);
                    } else if (*prev == 'n') {
                        if (col.cb.dt == UN) {
                            printf("error: update a unnull-type element as null.");
                            return;
                        }
                        next = prev + 1;
                        cbm ^= (1 << cc);
                    } else {
                        printf("error: update a string with non-string value.");
                        return;
                    }
                    start += col.cb.cl;
                    while (*next != ' ' && *next != ',') ++next;
                    brk = *next;
            }
            if (brk == ' ') {
                *next = brk;
                while (*next == ' ') ++next;
                brk = *next;
            }
            if (brk != ',') break;
        }
        prev = next;
        next = paser.getWord(prev, ' ');
        tabs.clear();
        tabs.push_back(ctb);
        paser.exec_where(next + 1);
        if (errwhere == 1) {
            return;
        }
        cidx = -1;
        int w = -1;
        for (int j = 0; j < ctb->cn; ++j) {
            if (ctb->col[j].bpt[0] != NULL) {
                if (w == -1 || w > ctb->rangeCount(j)) {
                    cidx = j;
                    w = ctb->rangeCount(j);
                }
            }
        }
        ans1.clear();
        ctb->select(cidx, ctb->range, ans1);
        int m = ans1.size();
        int ppp, sss;
        for (int i = 0; i < m; ++i) {
            int p = ans1[i].first;
            int s = ans1[i].second;
            uchar *it = ctb->getItem(p, s);
            memcpy(itemBuf, it, ctb->len);
            int kbm;
            memcpy(&kbm, itemBuf, 4);
            kbm |= cbm;
            memcpy(itemBuf, &kbm, 4);
            ctb->erase(p, s);
            start = updatebuf;
            for (int j = 0; j < cn; ++j) {
                int cc = cid[j];
                if ((cbm & (1 << cc)) == 0) {
                    memcpy(itemBuf + ctb->col[cc].cs, start, ctb->col[cc].cb.cl);
                }
                start += ctb->col[cc].cb.cl;
            }
            int pp, ss;
            bool ret = ctb->insert((uchar *) itemBuf, pp, ss);
            if (!ret) {
                for (int j = 0; j <= i; ++j) ctb->reinsert(ans1[j].first, ans1[j].second);
                if (i > 0) ctb->erase(ppp, sss);
                printf("error: repeated occurrence\n");
                break;
            }
            ppp = pp;
            sss = ss;
        }
    }
};

#endif //DATABASE_QUERYMANAGER_H
