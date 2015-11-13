//
// Created by huangsy13 on 11/13/15.
//

//命令解析类

#ifndef DATABASE_ORDER_H
#define DATABASE_ORDER_H

#include "define.h"
#include "Debug.h"

class Order{
public:
    OrderType type;
    std::string line;
    Order(std::string line):line(line){}


};

#endif //DATABASE_ORDER_H
