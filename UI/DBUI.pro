#-------------------------------------------------
#
# Project created by QtCreator 2015-12-26T10:00:40
#
#-------------------------------------------------

QT       += core gui
QT       += sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DBUI
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    createtable.cpp \
    deletetable.cpp

HEADERS  += mainwindow.h \
    createtable.h \
    deletetable.h \
    main.h \
    database/bufmanager/BufPageManager.h \
    database/bufmanager/FindReplace.h \
    database/fileio/FileManager.h \
    database/fileio/FileTable.h \
    database/indexManager/indexManager.h \
    database/indexManager/nodeManager.h \
    database/queryManager/print.h \
    database/queryManager/queryManager.h \
    database/recordManager/ItemList.h \
    database/recordManager/layout.h \
    database/recordManager/recordManager.h \
    database/systemManager/databaseInfo.h \
    database/systemManager/systemManager.h \
    database/systemManager/tableInfo.h \
    database/utils/compare.h \
    database/utils/MyBitMap.h \
    database/utils/MyHashMap.h \
    database/utils/MyLinkList.h \
    database/utils/pagedef.h \
    database/dataBaseManager.h \
    database/define.h \
    database/parser.h \
    database/tool.h \
    common.h


FORMS    += mainwindow.ui \
    createtable.ui \
    deletetable.ui

CONFIG += mobility
MOBILITY = 

