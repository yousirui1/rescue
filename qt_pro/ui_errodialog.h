/********************************************************************************
** Form generated from reading ui file 'errodialog.ui'
**
** Created: Sun Jul 19 00:00:48 2020
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_ERRODIALOG_H
#define UI_ERRODIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDialog>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>

QT_BEGIN_NAMESPACE

class Ui_ErroDialog
{
public:
    QLabel *label_81;
    QPushButton *error_confirm_btn_2;
    QLabel *label_82;
    QPushButton *error_cacel_btn_2;
    QLabel *backgroud_init_4;

    void setupUi(QDialog *ErroDialog)
    {
        if (ErroDialog->objectName().isEmpty())
            ErroDialog->setObjectName(QString::fromUtf8("ErroDialog"));
        ErroDialog->resize(497, 273);
        label_81 = new QLabel(ErroDialog);
        label_81->setObjectName(QString::fromUtf8("label_81"));
        label_81->setGeometry(QRect(200, 30, 101, 31));
        label_81->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        error_confirm_btn_2 = new QPushButton(ErroDialog);
        error_confirm_btn_2->setObjectName(QString::fromUtf8("error_confirm_btn_2"));
        error_confirm_btn_2->setGeometry(QRect(280, 190, 91, 31));
        error_confirm_btn_2->setStyleSheet(QString::fromUtf8("border-image: url(:/images/bluebutton.png);\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        label_82 = new QLabel(ErroDialog);
        label_82->setObjectName(QString::fromUtf8("label_82"));
        label_82->setGeometry(QRect(90, 110, 171, 31));
        label_82->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        error_cacel_btn_2 = new QPushButton(ErroDialog);
        error_cacel_btn_2->setObjectName(QString::fromUtf8("error_cacel_btn_2"));
        error_cacel_btn_2->setGeometry(QRect(150, 190, 91, 31));
        error_cacel_btn_2->setStyleSheet(QString::fromUtf8("border-image: url(:/images/bluebutton.png);\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        backgroud_init_4 = new QLabel(ErroDialog);
        backgroud_init_4->setObjectName(QString::fromUtf8("backgroud_init_4"));
        backgroud_init_4->setGeometry(QRect(0, 0, 497, 273));
        backgroud_init_4->setMaximumSize(QSize(497, 273));
        backgroud_init_4->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
""));

        retranslateUi(ErroDialog);

        QMetaObject::connectSlotsByName(ErroDialog);
    } // setupUi

    void retranslateUi(QDialog *ErroDialog)
    {
        ErroDialog->setWindowTitle(QApplication::translate("ErroDialog", "Dialog", 0, QApplication::UnicodeUTF8));
        label_81->setText(QApplication::translate("ErroDialog", "<html><head/><body><p align=\"center\">\346\217\220\347\244\272</p></body></html>", 0, QApplication::UnicodeUTF8));
        error_confirm_btn_2->setText(QApplication::translate("ErroDialog", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        label_82->setText(QApplication::translate("ErroDialog", "\346\230\257\345\220\246\347\241\256\350\256\244\345\215\270\350\275\275\345\256\242\346\210\267\347\253\257?", 0, QApplication::UnicodeUTF8));
        error_cacel_btn_2->setText(QApplication::translate("ErroDialog", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        backgroud_init_4->setText(QString());
        Q_UNUSED(ErroDialog);
    } // retranslateUi

};

namespace Ui {
    class ErroDialog: public Ui_ErroDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_ERRODIALOG_H
