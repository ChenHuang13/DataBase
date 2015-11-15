//
// Created by huangsy13 on 11/15/15.
//

#ifndef DATABASE_INFOMANEGER_H
#define DATABASE_INFOMANEGER_H

class InfoManager {
public:
    static Field getField(string DBName, string tableName){
        return Field();
    }

    static vector<string> getTables(string DB) {
        vector<string> Tablelist;
        return Tablelist;
    }
};

#endif //DATABASE_INFOMANEGER_H
