/********************************************************************************
** Form generated from reading UI file 'MainWindow.ui'
**
** Created by: Qt User Interface Compiler version 6.6.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QLabel>
#include <QtWidgets/QLineEdit>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSplitter>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QWidget *centralwidget;
    QLabel *humanFigureLabel;
    QGroupBox *SignIn_GroupBox;
    QVBoxLayout *verticalLayout;
    QSplitter *usernameSplitter;
    QLabel *usernameLabel;
    QLineEdit *usernameLineEdit;
    QSplitter *passwordSplitter;
    QLabel *passwordLabel;
    QLineEdit *passwordLineEdit;
    QPushButton *loginButton;
    QLabel *backgroundLabel;
    QPushButton *joinRoomButton;
    QPushButton *createRoomButton;
    QLineEdit *joinRoomLineEdit;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(737, 808);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        humanFigureLabel = new QLabel(centralwidget);
        humanFigureLabel->setObjectName("humanFigureLabel");
        humanFigureLabel->setGeometry(QRect(590, 550, 121, 121));
        SignIn_GroupBox = new QGroupBox(centralwidget);
        SignIn_GroupBox->setObjectName("SignIn_GroupBox");
        SignIn_GroupBox->setGeometry(QRect(240, 220, 271, 231));
        verticalLayout = new QVBoxLayout(SignIn_GroupBox);
        verticalLayout->setObjectName("verticalLayout");
        usernameSplitter = new QSplitter(SignIn_GroupBox);
        usernameSplitter->setObjectName("usernameSplitter");
        usernameSplitter->setOrientation(Qt::Horizontal);
        usernameLabel = new QLabel(usernameSplitter);
        usernameLabel->setObjectName("usernameLabel");
        usernameSplitter->addWidget(usernameLabel);
        usernameLineEdit = new QLineEdit(usernameSplitter);
        usernameLineEdit->setObjectName("usernameLineEdit");
        usernameSplitter->addWidget(usernameLineEdit);

        verticalLayout->addWidget(usernameSplitter);

        passwordSplitter = new QSplitter(SignIn_GroupBox);
        passwordSplitter->setObjectName("passwordSplitter");
        passwordSplitter->setOrientation(Qt::Horizontal);
        passwordLabel = new QLabel(passwordSplitter);
        passwordLabel->setObjectName("passwordLabel");
        passwordSplitter->addWidget(passwordLabel);
        passwordLineEdit = new QLineEdit(passwordSplitter);
        passwordLineEdit->setObjectName("passwordLineEdit");
        passwordLineEdit->setEchoMode(QLineEdit::Password);
        passwordSplitter->addWidget(passwordLineEdit);

        verticalLayout->addWidget(passwordSplitter);

        loginButton = new QPushButton(SignIn_GroupBox);
        loginButton->setObjectName("loginButton");

        verticalLayout->addWidget(loginButton);

        backgroundLabel = new QLabel(centralwidget);
        backgroundLabel->setObjectName("backgroundLabel");
        backgroundLabel->setGeometry(QRect(340, 150, 16, 16));
        joinRoomButton = new QPushButton(centralwidget);
        joinRoomButton->setObjectName("joinRoomButton");
        joinRoomButton->setGeometry(QRect(390, 490, 80, 24));
        createRoomButton = new QPushButton(centralwidget);
        createRoomButton->setObjectName("createRoomButton");
        createRoomButton->setGeometry(QRect(340, 550, 80, 24));
        joinRoomLineEdit = new QLineEdit(centralwidget);
        joinRoomLineEdit->setObjectName("joinRoomLineEdit");
        joinRoomLineEdit->setGeometry(QRect(270, 490, 113, 24));
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 737, 22));
        MainWindow->setMenuBar(menubar);
        statusbar = new QStatusBar(MainWindow);
        statusbar->setObjectName("statusbar");
        MainWindow->setStatusBar(statusbar);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QCoreApplication::translate("MainWindow", "MainWindow", nullptr));
        humanFigureLabel->setText(QString());
        SignIn_GroupBox->setTitle(QCoreApplication::translate("MainWindow", "Sign In", nullptr));
        usernameLabel->setText(QCoreApplication::translate("MainWindow", "Username", nullptr));
        passwordLabel->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        loginButton->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
        backgroundLabel->setText(QString());
        joinRoomButton->setText(QCoreApplication::translate("MainWindow", "Join Room", nullptr));
        createRoomButton->setText(QCoreApplication::translate("MainWindow", "Create Room", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
