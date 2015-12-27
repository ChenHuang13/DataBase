/********************************************************************************
** Form generated from reading UI file 'createtable.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CREATETABLE_H
#define UI_CREATETABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CreateTable
{
public:
    QWidget *widget;
    QGridLayout *gridLayout;
    QHBoxLayout *horizontalLayout;
    QLabel *tableName_2;
    QLineEdit *tableName;
    QVBoxLayout *verticalLayout;
    QLabel *label1;
    QTextEdit *columns;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *CreateTable)
    {
        if (CreateTable->objectName().isEmpty())
            CreateTable->setObjectName(QStringLiteral("CreateTable"));
        CreateTable->resize(280, 306);
        CreateTable->setModal(true);
        widget = new QWidget(CreateTable);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(10, 10, 260, 287));
        gridLayout = new QGridLayout(widget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        tableName_2 = new QLabel(widget);
        tableName_2->setObjectName(QStringLiteral("tableName_2"));

        horizontalLayout->addWidget(tableName_2);

        tableName = new QLineEdit(widget);
        tableName->setObjectName(QStringLiteral("tableName"));

        horizontalLayout->addWidget(tableName);


        gridLayout->addLayout(horizontalLayout, 0, 0, 1, 1);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label1 = new QLabel(widget);
        label1->setObjectName(QStringLiteral("label1"));

        verticalLayout->addWidget(label1);

        columns = new QTextEdit(widget);
        columns->setObjectName(QStringLiteral("columns"));

        verticalLayout->addWidget(columns);


        gridLayout->addLayout(verticalLayout, 1, 0, 1, 1);

        buttonBox = new QDialogButtonBox(widget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 2, 0, 1, 1);


        retranslateUi(CreateTable);
        QObject::connect(buttonBox, SIGNAL(accepted()), CreateTable, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), CreateTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(CreateTable);
    } // setupUi

    void retranslateUi(QDialog *CreateTable)
    {
        CreateTable->setWindowTitle(QApplication::translate("CreateTable", "Dialog", 0));
        tableName_2->setText(QApplication::translate("CreateTable", "Table Name:", 0));
        label1->setText(QApplication::translate("CreateTable", "Columns:", 0));
    } // retranslateUi

};

namespace Ui {
    class CreateTable: public Ui_CreateTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CREATETABLE_H
