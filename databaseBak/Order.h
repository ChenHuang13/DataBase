//
// Created by huangsy13 on 11/13/15.
//

//命令解析类

#ifndef DATABASE_ORDER_H
#define DATABASE_ORDER_H

#include "define.h"
#include "Debug.h"
#include "cstring"
#include "RecordManager/Field.h"
#include <sstream>
#include <algorithm>

using namespace std;

class Order {
public:
    std::string DBName;
    OrderType type;
    SubType subType;
    std::string line;
    std::string tableName;
    Field field;

    void getString(std::string subline) {
        if (subline == "") {
            return;
        }

        if (type == UNFINISH) {
            line = line + " " + subline;
        }
        else {
            line = subline;
            type = UNFINISH;
        }
        if (line[line.size() - 1] == ';') {
            type = FINISH;
            parseOrder();
        }

    }

    void parseOrder() {
        std::stringstream ss;
        ss << line;
        std::string partStr;
        ss >> partStr;
        std::transform(partStr.begin(), partStr.end(), partStr.begin(), ::tolower);
        if (partStr == "create") {
            ss >> partStr;
            std::transform(partStr.begin(), partStr.end(), partStr.begin(), ::tolower);
            if (partStr == "database") {
                ss >> partStr;
                type = DDL;
                subType = CreateDataBase;
                DBName = partStr.substr(0, partStr.size() - 1);
                return;
            }
            if (partStr == "table") {
                ss >> partStr;
                type = DDL;
                subType = CreateTable;
                string fieldStr;
                int i;
                for (i = 0; i < partStr.size(); i++) {
                    if (partStr[i] == '(') {
                        tableName = partStr.substr(0, i);
                        break;
                    }
                }
                if (i == partStr.size()) {
                    tableName = partStr;
                }

                for (i = 0; i < line.size(); i++) {
                    if (line[i] == '(') {
                        for (int j = line.size() - 1; j > i; j--)
                            if (line[j] == ')') {
                                fieldStr = line.substr(i + 1, j - i - 1);
                                break;
                            }
                        break;
                    }
                }
                field.fromStr(fieldStr);
                return;
            }
            return;
        }

        if (partStr == "use") {
            ss >> partStr;
            type = DDL;
            subType = UseDataBase;
            DBName = partStr.substr(0, partStr.size() - 1);
            return;
        }

        if (partStr == "show") {
            ss >> partStr;
            std::transform(partStr.begin(), partStr.end(), partStr.begin(), ::tolower);
            if (partStr == "database") {
                ss >> partStr;
                type = DDL;
                subType = ShowDataBase;
                DBName = partStr.substr(0, partStr.size() - 1);
                return;
            }
            if (partStr == "table") {
                ss >> partStr;
                type = DDL;
                subType = ShowTable;
                string fieldStr;
                tableName = partStr.substr(0, partStr.size() - 1);
                return;
            }
            return;
        }
        if (partStr == "drop") {
            ss >> partStr;
            std::transform(partStr.begin(), partStr.end(), partStr.begin(), ::tolower);
            if (partStr == "database") {
                ss >> partStr;
                type = DDL;
                subType = DropDataBase;
                DBName = partStr.substr(0, partStr.size() - 1);
                return;
            }
            if (partStr == "table") {
                ss >> partStr;
                type = DDL;
                subType = DropTable;
                string fieldStr;
                tableName = partStr.substr(0, partStr.size() - 1);
                return;
            }
            return;
        }

    }

    string getDBName() {
        return DBName;
    }

    string getTableName() {
        return tableName;
    }

    Field getField() {
        return field;
    }


};

#endif //DATABASE_ORDER_H
