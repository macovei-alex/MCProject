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
#include <QtWidgets/QLineEdit>
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
    QPushButton *messageButton;
    QLineEdit *messageBox;
    QPushButton *DrawButton;
    QPushButton *EraseButton;
    QPushButton *UndoButton;

    void setupUi(QDialog *CanvasPaint)
    {
        if (CanvasPaint->objectName().isEmpty())
            CanvasPaint->setObjectName("CanvasPaint");
        CanvasPaint->resize(1854, 967);
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
        messageButton = new QPushButton(ChatMessages);
        messageButton->setObjectName("messageButton");
        messageButton->setGeometry(QRect(220, 730, 80, 24));
        messageBox = new QLineEdit(ChatMessages);
        messageBox->setObjectName("messageBox");
        messageBox->setGeometry(QRect(40, 730, 171, 24));
        DrawButton = new QPushButton(CanvasPaint);
        DrawButton->setObjectName("DrawButton");
        DrawButton->setGeometry(QRect(1200, 690, 80, 24));
        EraseButton = new QPushButton(CanvasPaint);
        EraseButton->setObjectName("EraseButton");
        EraseButton->setGeometry(QRect(1290, 690, 80, 24));
        UndoButton = new QPushButton(CanvasPaint);
        UndoButton->setObjectName("UndoButton");
        UndoButton->setGeometry(QRect(1380, 690, 80, 24));

        retranslateUi(CanvasPaint);

        QMetaObject::connectSlotsByName(CanvasPaint);
    } // setupUi

    void retranslateUi(QDialog *CanvasPaint)
    {
        CanvasPaint->setWindowTitle(QCoreApplication::translate("CanvasPaint", "Canvas", nullptr));
        ResetCanvas->setText(QCoreApplication::translate("CanvasPaint", "ResetCanvas", nullptr));
        LeaveServerButton->setText(QCoreApplication::translate("CanvasPaint", "Leave Server", nullptr));
        GameChatLabel->setText(QCoreApplication::translate("CanvasPaint", "Game Chat", nullptr));
        messageButton->setText(QCoreApplication::translate("CanvasPaint", "Send", nullptr));
        DrawButton->setText(QCoreApplication::translate("CanvasPaint", "Draw", nullptr));
        EraseButton->setText(QCoreApplication::translate("CanvasPaint", "Erase", nullptr));
        UndoButton->setText(QCoreApplication::translate("CanvasPaint", "Undo", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanvasPaint: public Ui_CanvasPaint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASPAINT_H
