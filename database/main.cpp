//
// Created by huangsy13 on 12/21/15.
//
#include <iostream>

#include "dataBaseManager.h"


using namespace std;

int main() {

    init();

    fm = new FileManager();
    bpm = new BufPageManager(fm);
    bpl = new MyLinkList(CAP, MAX_TYPE_NUM);

    char ans;
    cout << "进行自动测试? y/n" << endl;
    cin >> ans;
    if (ans == 'y') {
        test();
    }
    else {
        cout<<"清空以前记录? y/n" << endl;
        cin >> ans;
        if (ans == 'y') {
            clearAll();
            load();
        } else {
            load();
        }
        string fileName;
        while (true) {
            cout << ">>";
            cin >> fileName;
            if (fileName == "exit") break;
            else fileInput(fileName);
        }
        closeDataBase();
    }
    return 0;
}
