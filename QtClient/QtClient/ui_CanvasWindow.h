/********************************************************************************
** Form generated from reading UI file 'CanvasWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_CANVASWINDOW_H
#define UI_CANVASWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanvasWindow
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
    QLabel *playerScore;
    QLabel *chosedWord;

    void setupUi(QDialog *CanvasWindow)
    {
        if (CanvasWindow->objectName().isEmpty())
            CanvasWindow->setObjectName("CanvasWindow");
        CanvasWindow->resize(1888, 967);
        resetCanvas = new QPushButton(CanvasWindow);
        resetCanvas->setObjectName("resetCanvas");
        resetCanvas->setGeometry(QRect(1200, 720, 80, 24));
        gameChat = new QWidget(CanvasWindow);
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
        drawButton = new QPushButton(CanvasWindow);
        drawButton->setObjectName("drawButton");
        drawButton->setGeometry(QRect(1200, 690, 80, 24));
        eraseButton = new QPushButton(CanvasWindow);
        eraseButton->setObjectName("eraseButton");
        eraseButton->setGeometry(QRect(1300, 690, 80, 24));
        undoButton = new QPushButton(CanvasWindow);
        undoButton->setObjectName("undoButton");
        undoButton->setGeometry(QRect(1400, 690, 80, 24));
        roomLabel = new QLabel(CanvasWindow);
        roomLabel->setObjectName("roomLabel");
        roomLabel->setGeometry(QRect(1300, 720, 61, 21));
        startGameButton = new QPushButton(CanvasWindow);
        startGameButton->setObjectName("startGameButton");
        startGameButton->setGeometry(QRect(1400, 720, 75, 24));
        timerLabel = new QLabel(CanvasWindow);
        timerLabel->setObjectName("timerLabel");
        timerLabel->setGeometry(QRect(1210, 750, 51, 16));
        playerScore = new QLabel(CanvasWindow);
        playerScore->setObjectName("playerScore");
        playerScore->setGeometry(QRect(1020, 29, 151, 21));
        chosedWord = new QLabel(CanvasWindow);
        chosedWord->setObjectName("chosedWord");
        chosedWord->setGeometry(QRect(1020, 60, 151, 16));

        retranslateUi(CanvasWindow);

        QMetaObject::connectSlotsByName(CanvasWindow);
    } // setupUi

    void retranslateUi(QDialog *CanvasWindow)
    {
        CanvasWindow->setWindowTitle(QCoreApplication::translate("CanvasWindow", "Canvas", nullptr));
        resetCanvas->setText(QCoreApplication::translate("CanvasWindow", "ResetCanvas", nullptr));
        gameChatLabel->setText(QCoreApplication::translate("CanvasWindow", "Game Chat", nullptr));
        messageButton->setText(QCoreApplication::translate("CanvasWindow", "Send", nullptr));
        drawButton->setText(QCoreApplication::translate("CanvasWindow", "Draw", nullptr));
        eraseButton->setText(QCoreApplication::translate("CanvasWindow", "Erase", nullptr));
        undoButton->setText(QCoreApplication::translate("CanvasWindow", "Undo", nullptr));
        roomLabel->setText(QCoreApplication::translate("CanvasWindow", "Room:", nullptr));
        startGameButton->setText(QCoreApplication::translate("CanvasWindow", "Start Game", nullptr));
        timerLabel->setText(QString());
        playerScore->setText(QCoreApplication::translate("CanvasWindow", "Your Score:", nullptr));
        chosedWord->setText(QCoreApplication::translate("CanvasWindow", "word: ", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanvasWindow: public Ui_CanvasWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASWINDOW_H
