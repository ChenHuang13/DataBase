//
// Created by huangsy13 on 11/15/15.
//
//管理表的信息
#ifndef DATABASE_INFOMANEGER_H
#define DATABASE_INFOMANEGER_H

#include "../tools/FileStruct.h"

class InfoManager {
public:
    static Field getField(string DBName, string tableName) {
        Field field;
        string path = DBName + "/" + ".info.txt";
        ifstream in(path.c_str());
        if(!in){
            cout << "打开文件失败！"<<endl;
            return field;
        }
        string line;
        while (getline(in,line)) {
            if (line == tableName){
                getline(in,line);
                field.fromStr(line);
                in.close();
                return field;
            }
            getline(in,line);
        }
    }

    static void getTables(string DB , vector<string>& Tablelist) {
        string path = DB + "/" + ".info.txt";
        ifstream in(path.c_str());
        if(!in){
            cout << "打开文件失败！"<<endl;
            return ;
        }
        string line;
        while (getline(in,line)) {
            Tablelist.push_back(line);
            getline(in,line);
        }
        in.close();

    }

    static void dropTable(string DB, string table) {
        vector<string> Tablelist;
        string path = DB + "/" + ".info.txt";
        ifstream in(path.c_str());
        string line;
        if(!in){
            cout << "打开文件失败！"<<endl;
            return;
        }
        while (getline(in,line)) {
            if (line == table) {
                getline(in,line);
                continue;
            }
            Tablelist.push_back(line);
        }
        in.close();

        ofstream out(path.c_str());
        if (!out) //检查文件是否正常打开
        {
            cout << "打开文件" << path << "出错" << endl;
            return;
        }
        for (int i = 0; i < Tablelist.size(); i++) {
            out << Tablelist[i] << endl;
        }
        out.close();
    }

    static void createTable(string DB, string table , Field field) {
        string path = DB + "/" + ".info.txt";
        ofstream out(path.c_str(), ios::app);
        if (!out) //检查文件是否正常打开
        {
            cout << "打开文件" << path << "出错" << endl;
        }
        else {
            out << table << endl << field.toStr() << endl;
        }
        out.close();
    }
};

#endif //DATABASE_INFOMANEGER_H
