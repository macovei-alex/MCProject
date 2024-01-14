/********************************************************************************
** Form generated from reading UI file 'ChooseWordWindow.ui'
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

class Ui_ChooseWordWindow
{
public:
    QPushButton *firstWordButton;
    QPushButton *secondWordButton;
    QPushButton *thirdWordButton;

    void setupUi(QDialog *ChooseWordWindow)
    {
        if (ChooseWordWindow->objectName().isEmpty())
            ChooseWordWindow->setObjectName("ChooseWordWindow");
        ChooseWordWindow->resize(400, 400);
        firstWordButton = new QPushButton(ChooseWordWindow);
        firstWordButton->setObjectName("firstWordButton");
        firstWordButton->setGeometry(QRect(100, 130, 191, 31));
        secondWordButton = new QPushButton(ChooseWordWindow);
        secondWordButton->setObjectName("secondWordButton");
        secondWordButton->setGeometry(QRect(100, 180, 191, 31));
        thirdWordButton = new QPushButton(ChooseWordWindow);
        thirdWordButton->setObjectName("thirdWordButton");
        thirdWordButton->setGeometry(QRect(100, 230, 191, 31));

        retranslateUi(ChooseWordWindow);

        QMetaObject::connectSlotsByName(ChooseWordWindow);
    } // setupUi

    void retranslateUi(QDialog *ChooseWordWindow)
    {
        ChooseWordWindow->setWindowTitle(QCoreApplication::translate("ChooseWordWindow", "Dialog", nullptr));
        firstWordButton->setText(QCoreApplication::translate("ChooseWordWindow", "Word1", nullptr));
        secondWordButton->setText(QCoreApplication::translate("ChooseWordWindow", "Word2", nullptr));
        thirdWordButton->setText(QCoreApplication::translate("ChooseWordWindow", "Word3", nullptr));
    } // retranslateUi

};

namespace Ui {
    class ChooseWordWindow: public Ui_ChooseWordWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CHOOSEWORDWINDOW_H
