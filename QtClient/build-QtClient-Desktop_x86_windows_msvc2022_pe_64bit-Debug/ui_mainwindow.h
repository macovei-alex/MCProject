/********************************************************************************
** Form generated from reading UI file 'mainwindow.ui'
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
    QLabel *label;
    QLabel *label_login;
    QGroupBox *SignIn_GroupBox;
    QVBoxLayout *verticalLayout;
    QSplitter *splitter;
    QLabel *label_2;
    QLineEdit *lineEdit;
    QSplitter *splitter_2;
    QLabel *label_3;
    QLineEdit *lineEdit_2;
    QPushButton *LoginButton;
    QMenuBar *menubar;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName("MainWindow");
        MainWindow->resize(845, 808);
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName("centralwidget");
        label = new QLabel(centralwidget);
        label->setObjectName("label");
        label->setGeometry(QRect(0, 150, 801, 571));
        label_login = new QLabel(centralwidget);
        label_login->setObjectName("label_login");
        label_login->setGeometry(QRect(590, 550, 121, 121));
        SignIn_GroupBox = new QGroupBox(centralwidget);
        SignIn_GroupBox->setObjectName("SignIn_GroupBox");
        SignIn_GroupBox->setGeometry(QRect(210, 350, 271, 231));
        verticalLayout = new QVBoxLayout(SignIn_GroupBox);
        verticalLayout->setObjectName("verticalLayout");
        splitter = new QSplitter(SignIn_GroupBox);
        splitter->setObjectName("splitter");
        splitter->setOrientation(Qt::Horizontal);
        label_2 = new QLabel(splitter);
        label_2->setObjectName("label_2");
        splitter->addWidget(label_2);
        lineEdit = new QLineEdit(splitter);
        lineEdit->setObjectName("lineEdit");
        splitter->addWidget(lineEdit);

        verticalLayout->addWidget(splitter);

        splitter_2 = new QSplitter(SignIn_GroupBox);
        splitter_2->setObjectName("splitter_2");
        splitter_2->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(splitter_2);
        label_3->setObjectName("label_3");
        splitter_2->addWidget(label_3);
        lineEdit_2 = new QLineEdit(splitter_2);
        lineEdit_2->setObjectName("lineEdit_2");
        lineEdit_2->setEchoMode(QLineEdit::Password);
        splitter_2->addWidget(lineEdit_2);

        verticalLayout->addWidget(splitter_2);

        LoginButton = new QPushButton(SignIn_GroupBox);
        LoginButton->setObjectName("LoginButton");

        verticalLayout->addWidget(LoginButton);

        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 845, 22));
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
        label->setText(QString());
        label_login->setText(QString());
        SignIn_GroupBox->setTitle(QCoreApplication::translate("MainWindow", "Sign In", nullptr));
        label_2->setText(QCoreApplication::translate("MainWindow", "Username", nullptr));
        label_3->setText(QCoreApplication::translate("MainWindow", "Password", nullptr));
        LoginButton->setText(QCoreApplication::translate("MainWindow", "Login", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
