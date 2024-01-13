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
    QPushButton *resetCanvas;
    QWidget *gameChat;
    QLabel *gameChatLabel;
    QWidget *chatMessages;
    QPushButton *messageButton;
    QLineEdit *messageBox;
    QPushButton *drawButton;
    QPushButton *eraseButton;
    QPushButton *undoButton;
    QLabel *roomLabel;
    QPushButton *startGameButton;
    QLabel *timerLabel;

    void setupUi(QDialog *CanvasPaint)
    {
        if (CanvasPaint->objectName().isEmpty())
            CanvasPaint->setObjectName("CanvasPaint");
        CanvasPaint->resize(1852, 967);
        resetCanvas = new QPushButton(CanvasPaint);
        resetCanvas->setObjectName("resetCanvas");
        resetCanvas->setGeometry(QRect(1200, 720, 80, 24));
        gameChat = new QWidget(CanvasPaint);
        gameChat->setObjectName("gameChat");
        gameChat->setGeometry(QRect(1200, 30, 301, 641));
        gameChatLabel = new QLabel(gameChat);
        gameChatLabel->setObjectName("gameChatLabel");
        gameChatLabel->setGeometry(QRect(110, 10, 81, 21));
        chatMessages = new QWidget(gameChat);
        chatMessages->setObjectName("chatMessages");
        chatMessages->setGeometry(QRect(-20, -130, 381, 781));
        messageButton = new QPushButton(chatMessages);
        messageButton->setObjectName("messageButton");
        messageButton->setGeometry(QRect(220, 730, 80, 24));
        messageBox = new QLineEdit(chatMessages);
        messageBox->setObjectName("messageBox");
        messageBox->setGeometry(QRect(40, 730, 171, 24));
        messageBox->raise();
        messageButton->raise();
        drawButton = new QPushButton(CanvasPaint);
        drawButton->setObjectName("drawButton");
        drawButton->setGeometry(QRect(1200, 690, 80, 24));
        eraseButton = new QPushButton(CanvasPaint);
        eraseButton->setObjectName("eraseButton");
        eraseButton->setGeometry(QRect(1300, 690, 80, 24));
        undoButton = new QPushButton(CanvasPaint);
        undoButton->setObjectName("undoButton");
        undoButton->setGeometry(QRect(1400, 690, 80, 24));
        roomLabel = new QLabel(CanvasPaint);
        roomLabel->setObjectName("roomLabel");
        roomLabel->setGeometry(QRect(1320, 750, 49, 21));
        startGameButton = new QPushButton(CanvasPaint);
        startGameButton->setObjectName("startGameButton");
        startGameButton->setGeometry(QRect(1400, 720, 75, 24));
        timerLabel = new QLabel(CanvasPaint);
        timerLabel->setObjectName("timerLabel");
        timerLabel->setGeometry(QRect(1220, 750, 49, 16));

        retranslateUi(CanvasPaint);

        QMetaObject::connectSlotsByName(CanvasPaint);
    } // setupUi

    void retranslateUi(QDialog *CanvasPaint)
    {
        CanvasPaint->setWindowTitle(QCoreApplication::translate("CanvasPaint", "Canvas", nullptr));
        resetCanvas->setText(QCoreApplication::translate("CanvasPaint", "ResetCanvas", nullptr));
        gameChatLabel->setText(QCoreApplication::translate("CanvasPaint", "Game Chat", nullptr));
        messageButton->setText(QCoreApplication::translate("CanvasPaint", "Send", nullptr));
        drawButton->setText(QCoreApplication::translate("CanvasPaint", "Draw", nullptr));
        eraseButton->setText(QCoreApplication::translate("CanvasPaint", "Erase", nullptr));
        undoButton->setText(QCoreApplication::translate("CanvasPaint", "Undo", nullptr));
        roomLabel->setText(QCoreApplication::translate("CanvasPaint", "Room:", nullptr));
        startGameButton->setText(QCoreApplication::translate("CanvasPaint", "Start Game", nullptr));
        timerLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CanvasPaint: public Ui_CanvasPaint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASPAINT_H
