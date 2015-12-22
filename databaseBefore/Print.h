//
// Created by huangsy13 on 11/15/15.
//

#ifndef DATABASE_PRINT_H
#define DATABASE_PRINT_H

#include <iostream>
#include <fstream>
#include "Order.h"
#include "RecordManager/Field.h"

using namespace std;

struct Print {
    Print operator <<(const int   n) { cout<<n; return *this;}
    Print operator <<(const float n) { cout<<n; return *this;}
    Print operator <<(const double n){ cout<<n; return *this;}
    Print operator <<(const char  c) { cout<<c; return *this;}
    Print operator <<(const char* s) { cout<<s; return *this;}
    Print operator <<(const string s){ cout<<s; return *this;}
    Print operator <<(ostream& (*op)(ostream& ) ){ cout << endl;return *this;}
    Print operator <<(const Order o){ cout<<o.line; return *this;}

    Print operator <<(const vector<string> &list) {
        for (int i = 0 ; i  <  list.size() ; i++){
            *this << list[i] << " ";
        }
        return *this;
    }

    Print operator <<(const Field &field){
//        for(int i = 0 ; i < field.size() ; i++){
//            *this << field.getFieldName(i) << " " << field.getFieldType(i) << " " << field.getRemark(i) << ",";
//        }
        cout << field.toStr() << endl;
        return *this;
    }


}print;

#endif //DATABASE_PRINT_H
