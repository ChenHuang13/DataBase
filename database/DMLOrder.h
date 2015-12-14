#ifndef DATABASE_DMLOrder_H
#define DATABASE_DMLOrder_H

#include "define.h"
#include "Debug.h"
#include "cstring"
#include "RecordManager/Field.h"
#include <sstream>
#include <algorithm>

using namespace std;

class DMLOrder {

    void removeBlank(string str, string& dst) {
        dst.clear();
        for (int i = 0; i < str.length(); ++i)
            if (str[i] != ' ') {
                dst = dst + str[i]
            }
    }

    void parse(string str, vector < string > &cls, vector < string > &atr) {
        int pos = str.find('.');
        if (pos != -1) {
            cls.push_back(str.substr(0, pos));
            atr.push_back(str.substr(pos + 1, str.length() - pos - 1));
        } else {
            cls.push_back("");
            atr.push_back(str);
        }
    }

    void opera(string str, vector < string > &cls, vector < string > &atr, vector < string > &opr) {
        equal = str.find('=');
        less = str.find('<');
        great = str.find('>');
        is = str.find(' is ');
        if (equal >= 0) {
            parse(str.substr(0, equal), cls, atr);
            parse(str.substr(equal + 1, str.length() - equal), cls, atr);
            opr.push_back("=");
            opr.push_back("");
        } else if (less >= 0) {
            parse(str.substr(0, less), cls, atr);
            parse(str.substr(less + 1, str.length() - less), cls, atr);
            opr.push_back("<");
            opr.push_back("");
        } else if (great >= 0) {
            parse(str.substr(0, less), cls, atr);
            parse(str.substr(less + 1, str.length() - less), cls, atr);
            opr.push_back("<");
            opr.push_back("");
        } else if (is >= 0) {
            parse(str.substr(0, less), cls, atr);
            parse(str.substr(less + 1, str.length() - less), cls, atr);
            opr.push_back("<");
            opr.push_back("");
        } else {
            parse(str, cls, atr);
            opr.push_back("");
        }
    }

    void split(string str, vector < string > &cls, vector < string > &atr) {
        str += ',';
        for (int i = j = 0; i < str.length(); ++i)
            if (str[i] == ',') {
                opera(str.substr(j, i - j), cls, atr);
                j = i + 1;
            }
    }

    Order getString(string line) {
        while (line[0] == ' ') line.erase(0, 1);
        head_into = line.find('into');      tail_into = head_into + 4;
        head_values = line.find('values');  tail_values = head_values + 6;
        head_from = line.find('from');      tail_from = head_from + 4;
        head_where = line.find('where');    tail_where = head_where + 5;
        head_set = line.find('set');        tail_set = head_set + 3;
        head_and = line.find('and');        tail_and = head_and + 3;
        end = line.find(';');
        left = line.find('(');
        right = line.find(')');
        if (line.find('insert') == 0) {
            table = line.substr(tail_into, head_values - tail_into);
            values = line.substr(left + 1, right - left - 1);
            split(table, cls_table, atr_table);
            split(values, cls_values, atr_values);
            for (table : cls_table) {
                Filed field(table, atr_values);
                RecordManager rm;
                rm.open(table);
                rm.insert(field);
            }
        }

        if (line.find('delete') == 0) {
            table = line.substr(tail_from, head_where - tail_from);
            where = line.substr(tail_where, end - tail_where);
            split(table, cls_table, atr_table, opr_table);
            split(where, cls_where, atr_where, opr_where);
            for (table : cls_table) {
                Filed field(table, atr_where, opr_where);
                RecordManager rm;
                rm.open(table);
                rm.delete(field);
            }
        }

        if (line.find('update') == 0) {
            table = line.substr(tail_update, head_set - tail_update);
            set = line.substr(tail_set, head_where - tail_set);
            where = line.substr(tail_where, end - tail_where);
            split(table, cls_table, atr_table, opr_table);
            split(set, cls_set, atr_set, opr_set);
            split(where, cls_where, atr_where, opr_where);
            for (table : cls_table) {
                Filed field(table, atr_where, opr_where, atr_set);
                RecordManager rm;
                rm.open(table);
                rm.delete(field);
            }
        }

        if (line.find('select') == 0) {
            name = line.substr(tail_select, head_from - tail_select);
            table = line.substr(tail_from, head_where - tail_from);
            where = line.substr(tail_where, end - tail_where);
            split(table, cls_table, atr_table, opr_table);
            split(name, cls_name, atr_name, opr_name);
            split(where, cls_where, atr_where, opr_where);
            if (cls_table.size() == 1) {
                table = cls_table[0];
                Filed field(table, atr_where, opr_where);
                RecordManager rm;
                rm.open(table);
                vector < Field > result;
                result = rm.query(field);
            } else {
                table = cls_table[0];
                _atr_where.clear();
                _opr_where.clear();
                for (i = 0; i < atr_where.size(); i += 2)
                    if ((cls_where[i] == table && cls_where[i] == "") && (cls_where[i + 1] == table && cls_where[i + 1] == "")) {
                        _atr_where.push_back(atr_where[i]);
                        _atr_where.push_back(atr_where[i + 1]);
                        _opr_where.push_back(opr_where[i]);
                        _opr_where.push_back(opr_where[i + 1]);
                    }

                Filed field(table, _atr_where, _opr_where);
                RecordManager rm;
                rm.open(table);
                vector < Field > result;
                result = rm.query(field);
                for (Field it : result) {
                    _atr_where.clear();
                    _opr_where.clear();
                    for (i = 0; i < atr_where.size(); ++i) {
                        if (cls_where[i] == table) {
                            _atr_where.push_back(it.get(atr_where[i]).toString());
                        }
                        _opr_where.push_back(opr_where[i]);
                    }
                    Filed field(table, _atr_where, _opr_where);
                    rm.query(field);
                }
            }
        }
    }
}

#endif //DATABASE_ORDER_H
