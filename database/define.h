//
// Created by huangsy13 on 12/21/15.
//

#ifndef DATABASE_DEFINE_H
#define DATABASE_DEFINE_H
#include <iostream>
#include "utils/pagedef.h"
#include "recordManager/ItemList.h"
#include "recordManager/layout.h"
#include "utils/MyBitMap.h"
#include "systemManager/databaseInfo.h"
#include <cstring>
#include <set>
#include <limits.h>

int e = 0;
FileManager* fm;
BufPageManager* bpm;
MyLinkList* bpl;
DatabaseInfo* cdbs;
multiset <char*, Cmp> dbs;
char buf[1000];
char itemBuf[1000];
char updatebuf[1000];
Range cr;
Range range[10];
FILE* fin;
int state;
int primary;
int errwhere;
int bm;
static Create create;
ll llmin = LONG_LONG_MIN;
ll llmax = LONG_LONG_MAX;
double dbmin = LONG_LONG_MIN;
double dbmax = LONG_LONG_MAX;
TableInfo* ctb, *stb, *ttb;
int cidx, sidx, tidx;
vector <char*> tname;
vector <char*> cname;
int cid[100];
int cis[100];
vector <TableInfo*> tabs;
int cost[100];
int idx[100];
double query_sum, query_max, query_min;
vector <pair<int, int> > ans1, ans2, ans3;
vector <TableInfo*> tab1, tab2;
vector <int> col1, col2, op;
vector <int> like_col;
vector <string> like_string;
bool all;
bool print_flag;

#endif //DATABASE_DEFINE_H
