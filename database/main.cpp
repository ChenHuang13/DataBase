//
// Created by huangsy13 on 12/21/15.
//
#include <iostream>

#include "dataBaseManager.h"


using namespace std;

int main() {
    //创建数据库管理类
    DataBaseManager dataBaseManager;
    //数据库初始化
    dataBaseManager.init();
    //数据库运行模式
    dataBaseManager.seletModel();
    //运行数据库
    dataBaseManager.run();
    //数据库退出
    dataBaseManager.quit();
}
