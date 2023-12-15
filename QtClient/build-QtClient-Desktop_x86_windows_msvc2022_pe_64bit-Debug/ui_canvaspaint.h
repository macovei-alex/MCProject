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
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_CanvasPaint
{
public:
    QWidget *GameChat;
    QLabel *GameChatLabel;
    QWidget *ChatMessages;
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QPushButton *LeaveServerButton;
    QPushButton *ResetCanvas;

    void setupUi(QDialog *CanvasPaint)
    {
        if (CanvasPaint->objectName().isEmpty())
            CanvasPaint->setObjectName("CanvasPaint");
        CanvasPaint->resize(600, 371);
        GameChat = new QWidget(CanvasPaint);
        GameChat->setObjectName("GameChat");
        GameChat->setGeometry(QRect(440, 0, 161, 371));
        GameChatLabel = new QLabel(GameChat);
        GameChatLabel->setObjectName("GameChatLabel");
        GameChatLabel->setGeometry(QRect(50, 10, 81, 20));
        ChatMessages = new QWidget(GameChat);
        ChatMessages->setObjectName("ChatMessages");
        ChatMessages->setGeometry(QRect(0, 60, 161, 291));
        widget = new QWidget(GameChat);
        widget->setObjectName("widget");
        widget->setGeometry(QRect(10, 340, 168, 26));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName("horizontalLayout");
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        LeaveServerButton = new QPushButton(widget);
        LeaveServerButton->setObjectName("LeaveServerButton");

        horizontalLayout->addWidget(LeaveServerButton);

        ResetCanvas = new QPushButton(CanvasPaint);
        ResetCanvas->setObjectName("ResetCanvas");
        ResetCanvas->setGeometry(QRect(0, 340, 80, 24));

        retranslateUi(CanvasPaint);

        QMetaObject::connectSlotsByName(CanvasPaint);
    } // setupUi

    void retranslateUi(QDialog *CanvasPaint)
    {
        CanvasPaint->setWindowTitle(QCoreApplication::translate("CanvasPaint", "Canvas", nullptr));
        GameChatLabel->setText(QCoreApplication::translate("CanvasPaint", "Game Chat", nullptr));
        LeaveServerButton->setText(QCoreApplication::translate("CanvasPaint", "Leave Server", nullptr));
        ResetCanvas->setText(QCoreApplication::translate("CanvasPaint", "PushButton", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanvasPaint: public Ui_CanvasPaint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASPAINT_H
