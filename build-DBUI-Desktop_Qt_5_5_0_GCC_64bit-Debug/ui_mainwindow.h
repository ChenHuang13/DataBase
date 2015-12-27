/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QFormLayout>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QTableView>
#include <QtWidgets/QToolButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralWidget;
    QGridLayout *gridLayout;
    QSplitter *splitter_2;
    QSplitter *splitter;
    QWidget *layoutWidget;
    QHBoxLayout *horizontalLayout;
    QToolButton *NewDatabase;
    QToolButton *OpenDatabase;
    QSpacerItem *horizontalSpacer;
    QToolButton *CreateTable;
    QToolButton *DeleteTable;
    QWidget *layoutWidget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label;
    QComboBox *comboBox;
    QSpacerItem *horizontalSpacer_2;
    QPushButton *pushButtonNewRecord;
    QPushButton *pushButtonDeleteRecord;
    QTableView *tableView;
    QWidget *layoutWidget2;
    QFormLayout *formLayout;
    QLabel *label_2;
    QLabel *status;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QStringLiteral("MainWindow"));
        MainWindow->resize(578, 480);
        centralWidget = new QWidget(MainWindow);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        gridLayout = new QGridLayout(centralWidget);
        gridLayout->setSpacing(6);
        gridLayout->setContentsMargins(11, 11, 11, 11);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QStringLiteral("splitter_2"));
        splitter_2->setOrientation(Qt::Vertical);
        splitter = new QSplitter(splitter_2);
        splitter->setObjectName(QStringLiteral("splitter"));
        splitter->setOrientation(Qt::Vertical);
        layoutWidget = new QWidget(splitter);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        horizontalLayout = new QHBoxLayout(layoutWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        NewDatabase = new QToolButton(layoutWidget);
        NewDatabase->setObjectName(QStringLiteral("NewDatabase"));

        horizontalLayout->addWidget(NewDatabase);

        OpenDatabase = new QToolButton(layoutWidget);
        OpenDatabase->setObjectName(QStringLiteral("OpenDatabase"));

        horizontalLayout->addWidget(OpenDatabase);

        horizontalSpacer = new QSpacerItem(58, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        CreateTable = new QToolButton(layoutWidget);
        CreateTable->setObjectName(QStringLiteral("CreateTable"));
        CreateTable->setCheckable(false);

        horizontalLayout->addWidget(CreateTable);

        DeleteTable = new QToolButton(layoutWidget);
        DeleteTable->setObjectName(QStringLiteral("DeleteTable"));

        horizontalLayout->addWidget(DeleteTable);

        splitter->addWidget(layoutWidget);
        layoutWidget1 = new QWidget(splitter);
        layoutWidget1->setObjectName(QStringLiteral("layoutWidget1"));
        horizontalLayout_2 = new QHBoxLayout(layoutWidget1);
        horizontalLayout_2->setSpacing(6);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(layoutWidget1);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_2->addWidget(label);

        comboBox = new QComboBox(layoutWidget1);
        comboBox->setObjectName(QStringLiteral("comboBox"));

        horizontalLayout_2->addWidget(comboBox);

        horizontalSpacer_2 = new QSpacerItem(128, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(horizontalSpacer_2);

        pushButtonNewRecord = new QPushButton(layoutWidget1);
        pushButtonNewRecord->setObjectName(QStringLiteral("pushButtonNewRecord"));

        horizontalLayout_2->addWidget(pushButtonNewRecord);

        pushButtonDeleteRecord = new QPushButton(layoutWidget1);
        pushButtonDeleteRecord->setObjectName(QStringLiteral("pushButtonDeleteRecord"));

        horizontalLayout_2->addWidget(pushButtonDeleteRecord);

        splitter->addWidget(layoutWidget1);
        tableView = new QTableView(splitter);
        tableView->setObjectName(QStringLiteral("tableView"));
        splitter->addWidget(tableView);
        splitter_2->addWidget(splitter);
        layoutWidget2 = new QWidget(splitter_2);
        layoutWidget2->setObjectName(QStringLiteral("layoutWidget2"));
        formLayout = new QFormLayout(layoutWidget2);
        formLayout->setSpacing(6);
        formLayout->setContentsMargins(11, 11, 11, 11);
        formLayout->setObjectName(QStringLiteral("formLayout"));
        formLayout->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(layoutWidget2);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setEnabled(true);

        formLayout->setWidget(0, QFormLayout::LabelRole, label_2);

        status = new QLabel(layoutWidget2);
        status->setObjectName(QStringLiteral("status"));

        formLayout->setWidget(0, QFormLayout::FieldRole, status);

        splitter_2->addWidget(layoutWidget2);

        gridLayout->addWidget(splitter_2, 0, 0, 1, 1);

        MainWindow->setCentralWidget(centralWidget);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0));
        NewDatabase->setText(QApplication::translate("MainWindow", "New Database...", 0));
        OpenDatabase->setText(QApplication::translate("MainWindow", "Open Database...", 0));
        CreateTable->setText(QApplication::translate("MainWindow", "Create Table...", 0));
        DeleteTable->setText(QApplication::translate("MainWindow", "Delete Tabel...", 0));
        label->setText(QApplication::translate("MainWindow", "Table :", 0));
        pushButtonNewRecord->setText(QApplication::translate("MainWindow", "New Record", 0));
        pushButtonDeleteRecord->setText(QApplication::translate("MainWindow", "Delete Record", 0));
        label_2->setText(QApplication::translate("MainWindow", "Status: ", 0));
        status->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
