//
// Created by huangsy13 on 11/15/15.
//

#ifndef DATABASE_FILESTRUCT_H
#define DATABASE_FILESTRUCT_H

#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <vector>
#include <cstring>
#include <iostream>
#include <sstream>
#include <fstream>

using namespace std;

//工具箱：
//1.int转string
//2.判断文件夹是否存在
//3.列出文件列表和文件计数

bool isDir(string dir) {
    struct stat fileStat;
    if ((stat(dir.c_str(), &fileStat) == 0) && S_ISDIR(fileStat.st_mode)) {
        return true;
    }
    return false;
}

string numToStr(int numn) {
    stringstream ss;
    ss << numn;
    string s2;
    ss >> s2;
    return s2;
}

void tranFile(string inFile, string outFile) {
    fstream fsCopee(inFile.data(), ios::binary | ios::in);
    fstream fsCoper(outFile.data(), ios::binary | ios::out);
    fsCoper << fsCopee.rdbuf();
}

struct FileStruct {
    string path;
    std::vector<string> fileNames;
    std::vector<string> dirNames;

    FileStruct() {
    }

    FileStruct(string path0) {
        setUp(path0);
    }

    void setUp(string path0) {
        fileNames.clear();
        dirNames.clear();
        path = path0;
        DIR *dp;
        struct dirent *dirp;
        if ((dp = opendir(path.data())) == NULL) {
            return;
        }
        while ((dirp = readdir(dp)) != NULL) {
            if ((strcmp(dirp->d_name, ".") == 0) || (strcmp(dirp->d_name, "..") == 0))
                continue;
            struct stat st;
            string ss = path + "/" + dirp->d_name;
            stat(ss.data(), &st);
            if (S_ISDIR(st.st_mode))
                dirNames.push_back(dirp->d_name);
            else
                fileNames.push_back(dirp->d_name);
        }
        closedir(dp);
    }

    int getFileNum() {
        return fileNames.size();
    }

    int getDirNum() {
        return dirNames.size();
    }

    string getFullDirPath(int o) {
        if (o >= getDirNum()) {
            return "";
        }
        return path + "/" + dirNames[o];
    }

    string getFullFilePath(int o) {
        if (o >= getFileNum()) {
            return "";
        }
        return path + "/" + fileNames[o];
    }

    string getFullDirPath(string str) {
        return path + "/" + str;
    }

    string getFullFilePath(string str) {
        return path + "/" + str;
    }

    bool isDir(string sPath = "") {
        struct stat st;
        string ss;
        if (sPath == "") {
            ss = path;
        }
        else {
            ss = sPath;
        }
        stat(ss.data(), &st);
        if (S_ISDIR(st.st_mode))
            return true;
        else
            return false;
    }

    int getAllFileNum() {
        int fileNum = 0;
        for (int i = 0; i < getDirNum(); i++) {
            FileStruct childFile(getFullDirPath(i));
            fileNum += childFile.getAllFileNum();
        }
        return fileNum + getFileNum();
    }

    int getAllDirNum() {
        int dirNum = 0;
        for (int i = 0; i < getDirNum(); i++) {
            FileStruct childFile(getFullDirPath(i));
            dirNum += 1 + childFile.getAllDirNum();
        }
        return dirNum;
    }
};

#endif //DATABASE_FILESTRUCT_H
