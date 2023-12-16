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
    QPushButton *ResetCanvas;
    QPushButton *LeaveServerButton;
    QWidget *GameChat;
    QLabel *GameChatLabel;
    QWidget *ChatMessages;

    void setupUi(QDialog *CanvasPaint)
    {
        if (CanvasPaint->objectName().isEmpty())
            CanvasPaint->setObjectName("CanvasPaint");
        CanvasPaint->resize(1568, 930);
        ResetCanvas = new QPushButton(CanvasPaint);
        ResetCanvas->setObjectName("ResetCanvas");
        ResetCanvas->setGeometry(QRect(1200, 720, 80, 24));
        LeaveServerButton = new QPushButton(CanvasPaint);
        LeaveServerButton->setObjectName("LeaveServerButton");
        LeaveServerButton->setGeometry(QRect(1290, 720, 91, 24));
        GameChat = new QWidget(CanvasPaint);
        GameChat->setObjectName("GameChat");
        GameChat->setGeometry(QRect(1200, 30, 301, 641));
        GameChatLabel = new QLabel(GameChat);
        GameChatLabel->setObjectName("GameChatLabel");
        GameChatLabel->setGeometry(QRect(110, 10, 81, 21));
        ChatMessages = new QWidget(GameChat);
        ChatMessages->setObjectName("ChatMessages");
        ChatMessages->setGeometry(QRect(-20, -130, 381, 781));

        retranslateUi(CanvasPaint);

        QMetaObject::connectSlotsByName(CanvasPaint);
    } // setupUi

    void retranslateUi(QDialog *CanvasPaint)
    {
        CanvasPaint->setWindowTitle(QCoreApplication::translate("CanvasPaint", "Canvas", nullptr));
        ResetCanvas->setText(QCoreApplication::translate("CanvasPaint", "ResetCanvas", nullptr));
        LeaveServerButton->setText(QCoreApplication::translate("CanvasPaint", "Leave Server", nullptr));
        GameChatLabel->setText(QCoreApplication::translate("CanvasPaint", "Game Chat", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanvasPaint: public Ui_CanvasPaint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASPAINT_H
