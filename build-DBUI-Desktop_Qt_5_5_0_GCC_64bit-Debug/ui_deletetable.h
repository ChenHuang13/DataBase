/********************************************************************************
** Form generated from reading UI file 'deletetable.ui'
**
** Created by: Qt User Interface Compiler version 5.5.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_DELETETABLE_H
#define UI_DELETETABLE_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>

QT_BEGIN_NAMESPACE

class Ui_deleteTable
{
public:
    QDialogButtonBox *buttonBox;
    QLabel *label;

    void setupUi(QDialog *deleteTable)
    {
        if (deleteTable->objectName().isEmpty())
            deleteTable->setObjectName(QStringLiteral("deleteTable"));
        deleteTable->resize(361, 81);
        deleteTable->setModal(true);
        buttonBox = new QDialogButtonBox(deleteTable);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setGeometry(QRect(100, 40, 171, 32));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);
        label = new QLabel(deleteTable);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(20, 10, 331, 20));

        retranslateUi(deleteTable);
        QObject::connect(buttonBox, SIGNAL(accepted()), deleteTable, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), deleteTable, SLOT(reject()));

        QMetaObject::connectSlotsByName(deleteTable);
    } // setupUi

    void retranslateUi(QDialog *deleteTable)
    {
        deleteTable->setWindowTitle(QApplication::translate("deleteTable", "Dialog", 0));
        label->setText(QApplication::translate("deleteTable", "Are you shure you want to delete current table?", 0));
    } // retranslateUi

};

namespace Ui {
    class deleteTable: public Ui_deleteTable {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DELETETABLE_H
