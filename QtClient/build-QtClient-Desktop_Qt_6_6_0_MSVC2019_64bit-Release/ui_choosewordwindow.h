/********************************************************************************
** Form generated from reading UI file 'choosewordwindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CHOOSEWORDWINDOW_H
#define UI_CHOOSEWORDWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_choosewordwindow
{
public:
    QPushButton *firstWordButton;
    QPushButton *secondWordButton;
    QPushButton *thirdWordButton;

    void setupUi(QDialog *choosewordwindow)
    {
        if (choosewordwindow->objectName().isEmpty())
            choosewordwindow->setObjectName("choosewordwindow");
        choosewordwindow->resize(400, 400);
        firstWordButton = new QPushButton(choosewordwindow);
        firstWordButton->setObjectName("firstWordButton");
        firstWordButton->setGeometry(QRect(100, 130, 191, 31));
        secondWordButton = new QPushButton(choosewordwindow);
        secondWordButton->setObjectName("secondWordButton");
        secondWordButton->setGeometry(QRect(100, 180, 191, 31));
        thirdWordButton = new QPushButton(choosewordwindow);
        thirdWordButton->setObjectName("thirdWordButton");
        thirdWordButton->setGeometry(QRect(100, 230, 191, 31));

        retranslateUi(choosewordwindow);

        QMetaObject::connectSlotsByName(choosewordwindow);
    } // setupUi

    void retranslateUi(QDialog *choosewordwindow)
    {
        choosewordwindow->setWindowTitle(QCoreApplication::translate("choosewordwindow", "Dialog", nullptr));
        firstWordButton->setText(QCoreApplication::translate("choosewordwindow", "Word1", nullptr));
        secondWordButton->setText(QCoreApplication::translate("choosewordwindow", "Word2", nullptr));
        thirdWordButton->setText(QCoreApplication::translate("choosewordwindow", "Word3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class choosewordwindow: public Ui_choosewordwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSEWORDWINDOW_H
