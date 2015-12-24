//
// Created by huangsy13 on 12/23/15.
//

//这里对索引文件进行读取

#ifndef DATABASE_NODEMANAGER_H
#define DATABASE_NODEMANAGER_H
#include "../utils/MyLinkList.h"
#include "../recordManager/ItemList.h"
#include "../recordManager/layout.h"
#include <string>

using namespace std;

struct Layout {
    ListLayout* innerLayout;
    ListLayout* leafLayout;
};

struct NodeManager {
    int fileID;
    int num;

    BufPageManager* bpm;
    FileManager* fm;
    MyLinkList* bpl;

    NodeManager(MyLinkList* bplist, FileManager* fmg, BufPageManager* bufPM, int fid, int nodeNum ) {
        bpl = bplist;
        fm = fmg;
        bpm = bufPM;
        fileID = fid;
        num = nodeNum;
    }

    ItemList* getList(int tid, int nodeID, const Layout& layout) {
        int index;
        BufType b = bpm->getPage(fileID, nodeID, index);
        bpl->insert(tid, index);
        ListHeader* header = (ListHeader*) b;
        ListLayout* lo = (header->listType == LEAF_TYPE)
                         ? layout.leafLayout : layout.innerLayout;
        ItemList* list = new ItemList(  b, lo, bpm, index, nodeID);
        return list;
    }

    ItemList* getList(int tid, int nodeID, ListLayout* lo) {
        int index;
        BufType b = bpm->getPage(fileID, nodeID, index);
        bpl->insert(tid, index);
        ItemList* list = new ItemList(b, lo, bpm, index, nodeID);
        return list;
    }

    ItemList* newList(int tid, bool isLeaf, ListLayout* layout) {
        int pos = num ++;
        int index;
        BufType b = bpm->allocPage(fileID, pos, index);
        bpl->insert(tid, index);
        layout->clear(b, isLeaf);
        ItemList* list = new ItemList( b, layout, bpm, index, pos);
        list->markDirty();
        return list;
    }

    void release(ItemList* node) {
        int index = node->bufIndex;
        bpm->release(index);
        bpl->del(index);
    }

    int nt() {
        return fm->newType();
    }

    void ct(int t, bool reserve) {
        int k = bpl->getFirst(t);
        while (!bpl->isHead(k)) {
            int n = bpl->next(k);
            if (reserve)
                bpm->writeBack(k);
            else
                bpm->release(k);
            bpl->del(k);
            k = n;
        }
        fm->closeType(t);
    }
};
#endif //DATABASE_NODEMANAGER_H
