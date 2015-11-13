//
// Created by huangsy13 on 11/13/15.
//

//调试类

#ifndef DATABASE_DEBUG_H
#define DATABASE_DEBUG_H

#include <iostream>
#include <fstream>
#include "Order.h"

using namespace std;

struct Debug
{
    Debug operator <<(const int   n) { cout<<n; return *this;}
    Debug operator <<(const float n) { cout<<n; return *this;}
    Debug operator <<(const double n){ cout<<n; return *this;}
    Debug operator <<(const char  c) { cout<<c; return *this;}
    Debug operator <<(const char* s) { cout<<s; return *this;}
    Debug operator <<(const string s){ cout<<s; return *this;}
    Debug operator <<(ostream& (*op)(ostream& ) ){ cout << endl;return *this;}

    Debug operator <<(const Order o){ cout<<o.line; return *this;}

}debug;


#endif //DATABASE_DEBUG_H
