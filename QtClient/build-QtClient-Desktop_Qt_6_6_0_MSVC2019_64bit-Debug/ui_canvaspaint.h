/********************************************************************************
** Form generated from reading UI file 'canvaspaint.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANVASPAINT_H
#define UI_CANVASPAINT_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanvasPaint
{
public:
    QWidget *GameChat;
    QPushButton *LeaveServerButton;
    QLabel *GameChatLabel;
    QWidget *ChatMessages;

    void setupUi(QDialog *CanvasPaint)
    {
        if (CanvasPaint->objectName().isEmpty())
            CanvasPaint->setObjectName("CanvasPaint");
        CanvasPaint->resize(600, 371);
        GameChat = new QWidget(CanvasPaint);
        GameChat->setObjectName("GameChat");
        GameChat->setGeometry(QRect(440, 0, 161, 371));
        LeaveServerButton = new QPushButton(GameChat);
        LeaveServerButton->setObjectName("LeaveServerButton");
        LeaveServerButton->setGeometry(QRect(40, 340, 80, 24));
        GameChatLabel = new QLabel(GameChat);
        GameChatLabel->setObjectName("GameChatLabel");
        GameChatLabel->setGeometry(QRect(50, 10, 81, 20));
        ChatMessages = new QWidget(GameChat);
        ChatMessages->setObjectName("ChatMessages");
        ChatMessages->setGeometry(QRect(-1, 40, 161, 291));

        retranslateUi(CanvasPaint);

        QMetaObject::connectSlotsByName(CanvasPaint);
    } // setupUi

    void retranslateUi(QDialog *CanvasPaint)
    {
        CanvasPaint->setWindowTitle(QCoreApplication::translate("CanvasPaint", "Canvas", nullptr));
        LeaveServerButton->setText(QCoreApplication::translate("CanvasPaint", "Leave Server", nullptr));
        GameChatLabel->setText(QCoreApplication::translate("CanvasPaint", "Game Chat", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanvasPaint: public Ui_CanvasPaint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASPAINT_H
