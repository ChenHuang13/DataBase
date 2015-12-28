//
// Created by huangsy13 on 12/23/15.
//

#ifndef DATABASE_QUERYMANAGER_H
#define DATABASE_QUERYMANAGER_H

#include <regex>
#include "../define.h"
#include "../parser.h"
#include "print.h"
#include "../tool.h"

enum extop {
    extop_nop, extop_sum, extop_average, extop_max, extop_min
};

bool order_cmp(pair < int, int > A, pair < int, int > B) {
    uchar* it1 = ctb->getItem(A.first, A.second);
    uchar* it2 = ctb->getItem(B.first, B.second);
    if (ctb->col[order_col].cb.dt == LL_TYPE) {
        ll v1, v2;
        memcpy(&v1, it1 + ctb->col[order_col].cs, 8);
        memcpy(&v2, it2 + ctb->col[order_col].cs, 8);
        if (order_flag == 1)
            return !(v1 < v2);
        else
            return v1 < v2;
    } else if (ctb->col[order_col].cb.dt == DB_TYPE) {
        double v1, v2;
        memcpy(&v1, it1 + ctb->col[order_col].cs, 8);
        memcpy(&v2, it2 + ctb->col[order_col].cs, 8);
        if (order_flag == 1)
            return !(v1 < v2);
        else
            return v1 < v2;
    } else {
        uchar* v1, *v2;
        v1 = it1 + ctb->col[order_col].cs;
        v2 = it2 + ctb->col[order_col].cs;
        if (order_flag == 1)
            return strcmp((char*)v1, (char*)v2) > 0;
        else
            return strcmp((char*)v1, (char*)v2) < 0;
    }
}

class QueryManager {
public:

    static void like(bool &like_flag, const char* str1, string &str2) {
        regex pattern(str2);
        like_flag &= regex_match(str1, pattern);
    }

    static void exec_delete(char *sql) {
        char *prev, *next;
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
                case DB_TYPE:
                    next = paser.getWord(prev, e);
                    //printf("data\t%s\n", prev);
                    if (*prev == '\'') {
                        printf("error: insert string instead of double\n");
                        return;
                    }
                    if (*prev != 'n') {
                        double d = atof(prev);
                        memcpy(start, &d, len);
                    } else if (nt == N) {
                        bm ^= (1 << cc);
                    } else {
                        printf("error: insert null value, but not null type\n");
                        return;
                    }
                    break;
                case DT_TYPE:
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
            if (order_flag > 0)
                sort(ans1.begin(), ans1.end(), order_cmp);

            for (int i = 0; i < m; ++i) {
                int p = ans1[i].first;
                int s = ans1[i].second;
                //cout << p <<"\t"<<s<<"\t"<<i - m<<endl;
                uchar *it = ctb->getItem(p, s);
                bool like_flag = true;
                for (int j = 0; j < like_col.size(); ++j)
                    like(like_flag, (char*)(it + ctb->col[like_col[j]].cs), like_string[j]);
                if (like_flag) {
                    if (all) printItem(ctb, it);
                    else printCols1(it);
                    if (extop_flag == extop_nop) printf("\n");
                    countt++;
                }
            }
            if (extop_flag == extop_nop) {
                printf("counted:%d\n", countt);
            } else if (extop_flag == extop_sum) {
                printf("sum:%lf\n", query_sum);
            } else if (extop_flag == extop_average) {
                printf("average:%lf\n", 1.0 * query_sum / countt);
            } else if (extop_flag == extop_max) {
                printf("max:%lf\n", query_max);
            } else if (extop_flag == extop_min) {
                printf("min:%lf\n", query_min);
            }
            return;
        }
        int cns;
        if (!all) {
            cns = cname.size();
            for (int i = 0; i < cns; ++i) {
                if (strcmp(tname[i], ctb->tn) == 0) {
                    cis[i] = 0;
                    cid[i] = ctb->cmap[cname[i]];
                } else if (strcmp(tname[i], stb->tn) == 0) {
                    cis[i] = 1;
                    cid[i] = stb->cmap[cname[i]];
                } else {
                    cis[i] = 2;
                    cid[i] = ttb->cmap[cname[i]];
                }
            }
        }
        bool connect[100];
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
            if (strcmp(tab1[i]->tn, stb->tn) == 0)
                connect[col1[i]] = true;
        }
        stb->copyRange(connect);

        bool connect2[100];
        if (n == 3) {
            for (int j = 0; j < ttb->cn; ++j) connect2[j] = false;
            for (int j = 0; j < cn; ++j) {
                if (strcmp(tab1[j]->tn, ttb->tn) == 0)
                    connect2[col1[j]] = true;
                if (strcmp(tab2[j]->tn, ttb->tn) == 0)
                    connect2[col2[j]] = true;
            }
            ttb->copyRange(connect2);
        }

        for (int i = 0; i < m; ++i) {
            if (i != 0) stb->backRange(connect);

            int p = ans1[i].first;
            int s = ans1[i].second;
            uchar *it = ctb->getItem(p, s);
            int bm1;
            memcpy(&bm1, it, 4);
            uchar *cd;
            ans2.clear();
            for (int j = 0; j < cn; ++j)
                if (strcmp(tab1[j]->tn, stb->tn) == 0) {
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
            if (n == 2) {
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
                            if (cis[k] == 0) {
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
            } else {
                int m2 = ans2.size();
                for (int j = 0; j < m2; ++j) {
                    ttb->backRange(connect2);

                    int p2 = ans2[j].first;
                    int s2 = ans2[j].second;
                    uchar *it2 = stb->getItem(p2, s2);

                    ll v;
                    memcpy(&v, it2 + stb->col[0].cs, 8);
                    if (v == 218353) {
                        printf("Got");
                    }
                    int bm2;
                    memcpy(&bm2, it2, 4);
                    uchar *cd2;
                    ans3.clear();
                    for (int k = 0; k < cn; ++k)
                        if (strcmp(tab1[k]->tn, ttb->tn) == 0) {
                            if (strcmp(tab2[k]->tn, ctb->tn) == 0) {
                                const Col &col = ctb->col[col2[k]];
                                cd = it + col.cs;
                                getRange(cd, col.cb.cl, col.cb.cl, col.cb.dt, op[k]);
                            } else {
                                const Col &col = stb->col[col2[k]];
                                cd = it2 + col.cs;
                                getRange(cd, col.cb.cl, col.cb.cl, col.cb.dt, op[k]);
                            }
                            ttb->merge(col1[k], cr);
                        } else if (strcmp(tab2[k]->tn, ttb->tn) == 0) {
                            if (strcmp(tab1[k]->tn, ctb->tn) == 0) {
                                const Col &col = ctb->col[col1[k]];
                                cd = it + col.cs;
                                getRange(cd, col.cb.cl, col.cb.cl, col.cb.dt, (op[k] <= 3) ? 3 - op[k] : op[k]);
                            } else {
                                const Col &col = stb->col[col1[k]];
                                cd = it2 + col.cs;
                                getRange(cd, col.cb.cl, col.cb.cl, col.cb.dt, (op[k] <= 3) ? 3 - op[k] : op[k]);
                            }
                            ttb->merge(col2[k], cr);
                        }

                    int idx2 = -1;
                    int w2 = -1;
                    for (int k = 0; k < ttb->cn; ++k) {
                        if (ttb->col[k].bpt[0] != NULL) {
                            if (w2 == -1 || w2 > ttb->rangeCount(k)) {
                                idx2 = k;
                                w2 = ttb->rangeCount(k);
                            }
                        }
                    }
                    ttb->select(idx2, ttb->range, ans3);
                    int m3 = ans3.size();
                    for (int k = 0; k < m3; ++k) {
                        countt++;
                        int p3 = ans3[j].first;
                        int s3 = ans3[j].second;
                        uchar *it3 = ttb->getItem(p3, s3);
                        int bm3;
                        memcpy(&bm3, it3, 4);
                        if (all) {
                            printItem(ctb, it);
                            printItem(stb, it2);
                            printItem(ttb, it3);
                        } else {
                            for (int l = 0; l < cns; ++l) {
                                if (cis[l] == 0) {
                                    bm = bm1;
                                    printCol(ctb, cid[l], it);
                                } else if (cis[l] == 1) {
                                    bm = bm2;
                                    printCol(stb, cid[l], it2);
                                } else {
                                    bm = bm3;
                                    printCol(ttb, cid[l], it3);
                                }
                            }
                        }
                        printf("\n");
                    }
                }
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
                case DB_TYPE:
                    if (*prev == '\'') {
                        printf("error: update a double-type element as a string.");
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
                        double key = atof(prev);
                        memcpy(start, &key, 8);
                        start += 8;
                    }
                    break;
                case DT_TYPE:
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
