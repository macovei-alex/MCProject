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

QT_BEGIN_NAMESPACE

class Ui_CanvasWindow
{
public:
    QPushButton *resetCanvas;
    QPushButton *drawButton;
    QPushButton *eraseButton;
    QPushButton *undoButton;
    QLabel *roomLabel;
    QPushButton *startGameButton;
    QLabel *timerLabel;
    QLabel *playerScore;
    QLabel *chosedWord;
    QLineEdit *messageBox;
    QPushButton *messageButton;
    QLabel *gameChatLabel;

    void setupUi(QDialog *CanvasWindow)
    {
        if (CanvasWindow->objectName().isEmpty())
            CanvasWindow->setObjectName("CanvasWindow");
        CanvasWindow->resize(1888, 967);
        resetCanvas = new QPushButton(CanvasWindow);
        resetCanvas->setObjectName("resetCanvas");
        resetCanvas->setGeometry(QRect(1200, 720, 80, 24));
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
        playerScore->setGeometry(QRect(950, 10, 211, 111));
        chosedWord = new QLabel(CanvasWindow);
        chosedWord->setObjectName("chosedWord");
        chosedWord->setGeometry(QRect(760, 40, 171, 41));
        messageBox = new QLineEdit(CanvasWindow);
        messageBox->setObjectName("messageBox");
        messageBox->setGeometry(QRect(1200, 650, 181, 24));
        messageButton = new QPushButton(CanvasWindow);
        messageButton->setObjectName("messageButton");
        messageButton->setGeometry(QRect(1400, 650, 80, 24));
        gameChatLabel = new QLabel(CanvasWindow);
        gameChatLabel->setObjectName("gameChatLabel");
        gameChatLabel->setGeometry(QRect(1200, 60, 271, 571));

        retranslateUi(CanvasWindow);

        QMetaObject::connectSlotsByName(CanvasWindow);
    } // setupUi

    void retranslateUi(QDialog *CanvasWindow)
    {
        CanvasWindow->setWindowTitle(QCoreApplication::translate("CanvasWindow", "Canvas", nullptr));
        resetCanvas->setText(QCoreApplication::translate("CanvasWindow", "ResetCanvas", nullptr));
        drawButton->setText(QCoreApplication::translate("CanvasWindow", "Draw", nullptr));
        eraseButton->setText(QCoreApplication::translate("CanvasWindow", "Erase", nullptr));
        undoButton->setText(QCoreApplication::translate("CanvasWindow", "Undo", nullptr));
        roomLabel->setText(QCoreApplication::translate("CanvasWindow", "Room:", nullptr));
        startGameButton->setText(QCoreApplication::translate("CanvasWindow", "Start Game", nullptr));
        timerLabel->setText(QString());
        playerScore->setText(QCoreApplication::translate("CanvasWindow", "Your Score:", nullptr));
        chosedWord->setText(QCoreApplication::translate("CanvasWindow", "word: ", nullptr));
        messageButton->setText(QCoreApplication::translate("CanvasWindow", "Send", nullptr));
        gameChatLabel->setText(QString());
    } // retranslateUi

};

namespace Ui {
    class CanvasWindow: public Ui_CanvasWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASWINDOW_H
