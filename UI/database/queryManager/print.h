//
// Created by huangsy13 on 12/23/15.
//

#ifndef DATABASE_PRINT_H
#define DATABASE_PRINT_H
#include <cstdio>
#include "../utils/compare.h"
#include "../dataBaseManager.h"
void printCol(TableInfo* tb, int i, uchar* it) {
    if ((bm & (1 << i)) != 0) {
        printf("null\t");
    } else {
        if (tb->col[i].cb.dt == LL_TYPE) {
            ll v;
            memcpy(&v, it + tb->col[i].cs, 8);
            if (print_flag) printf("%lld\t", v);
            query_sum += v;
            query_max = max(query_max, v);
            query_min = min(query_min, v);
        } else {
            if (print_flag) printf("%s\t", (char*)(it + tb->col[i].cs));
        }
    }
}
void printCols1(uchar* it) {
    int n = cname.size();
    memcpy(&bm, it, 4);
    for (int i = 0; i < n; ++ i) printCol(ctb, ctb->cmap[cname[i]], it);
}
void printItem(TableInfo* tb, uchar* it) {
    memcpy(&bm, it, 4);
    for (int i = 0; i < tb->cn; ++ i) printCol(ctb, i, it);
}
#endif //DATABASE_MYPRINT_H
