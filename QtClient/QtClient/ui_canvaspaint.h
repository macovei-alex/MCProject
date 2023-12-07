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

QT_BEGIN_NAMESPACE

class Ui_CanvasPaint
{
public:

    void setupUi(QDialog *CanvasPaint)
    {
        if (CanvasPaint->objectName().isEmpty())
            CanvasPaint->setObjectName("CanvasPaint");
        CanvasPaint->resize(400, 300);

        retranslateUi(CanvasPaint);

        QMetaObject::connectSlotsByName(CanvasPaint);
    } // setupUi

    void retranslateUi(QDialog *CanvasPaint)
    {
        CanvasPaint->setWindowTitle(QCoreApplication::translate("CanvasPaint", "Canvas", nullptr));
    } // retranslateUi

};

namespace Ui {
    class CanvasPaint: public Ui_CanvasPaint {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CANVASPAINT_H
