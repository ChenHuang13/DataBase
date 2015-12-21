//
// Created by huangsy13 on 11/13/15.
//

#include <iostream>
#include "DataBaseManage.h"

using namespace std;

int main() {
    freopen("in.txt","r",stdin);
    freopen("out.txt","w",stdout);
    DataBaseManager* dataBaseManager = new DataBaseManager();
    dataBaseManager->globalInit();
    dataBaseManager->run();
    return 0;
}