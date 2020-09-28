/********************************************************************************
** Form generated from reading ui file 'mainwindow.ui'
**
** Created: Sun Sep 27 20:19:43 2020
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "mpushbutton.h"

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *centerWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *hrightSpace;
    QWidget *detailWidget;
    QLabel *modeLabel;
    QLabel *desLabel;
    QLabel *modeWidget;
    QVBoxLayout *verticalLayout_2;
    mPushButton *installButton;
    mPushButton *uploadButton;
    mPushButton *rebootButton;
    QSpacerItem *hleftSpace;
    QLabel *lineLabel;
    QLabel *bottomWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *f2Label;
    QLabel *arrowLabel;
    QLabel *enterLabel;
    QSpacerItem *hcenSpace;
    QWidget *netWidget;
    QHBoxLayout *horizontalLayout_3;
    QLabel *netpoint_label;
    QLabel *netstate_label;
    QWidget *ipWidget;
    QHBoxLayout *horizontalLayout_4;
    QLabel *neticon_label;
    QLabel *ip_label;
    QLabel *dataLabel;

    void setupUi(QWidget *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1600, 900);
        MainWindow->setMinimumSize(QSize(800, 600));
        MainWindow->setMaximumSize(QSize(1920, 1080));
        MainWindow->setStyleSheet(QString::fromUtf8("QWidget#MainWindow{border-image: url(:/images/backgroud.png);}"));
        verticalLayout = new QVBoxLayout(MainWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        centerWidget = new QLabel(MainWindow);
        centerWidget->setObjectName(QString::fromUtf8("centerWidget"));
        centerWidget->setStyleSheet(QString::fromUtf8("QWidget#centerWidget{border-image: url(:/images/backgroud.png);}"));
        horizontalLayout = new QHBoxLayout(centerWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        hrightSpace = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(hrightSpace);

        detailWidget = new QWidget(centerWidget);
        detailWidget->setObjectName(QString::fromUtf8("detailWidget"));
        detailWidget->setMinimumSize(QSize(576, 440));
        detailWidget->setMaximumSize(QSize(576, 440));
        modeLabel = new QLabel(detailWidget);
        modeLabel->setObjectName(QString::fromUtf8("modeLabel"));
        modeLabel->setGeometry(QRect(200, 10, 161, 41));
        modeLabel->setStyleSheet(QString::fromUtf8("font: 16pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));
        desLabel = new QLabel(detailWidget);
        desLabel->setObjectName(QString::fromUtf8("desLabel"));
        desLabel->setGeometry(QRect(10, 90, 311, 31));
        desLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));
        modeWidget = new QLabel(detailWidget);
        modeWidget->setObjectName(QString::fromUtf8("modeWidget"));
        modeWidget->setGeometry(QRect(0, 140, 576, 211));
        modeWidget->setMinimumSize(QSize(576, 0));
        modeWidget->setMaximumSize(QSize(576, 16777215));
        modeWidget->setStyleSheet(QString::fromUtf8("QWidget#modeWidget\n"
"{\n"
"border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"\n"
"border-bottom: 10px transparent;\n"
"\n"
"}\n"
""));
        verticalLayout_2 = new QVBoxLayout(modeWidget);
        verticalLayout_2->setSpacing(0);
        verticalLayout_2->setMargin(11);
        verticalLayout_2->setObjectName(QString::fromUtf8("verticalLayout_2"));
        verticalLayout_2->setContentsMargins(0, 0, 10, 0);
        installButton = new mPushButton(modeWidget);
        installButton->setObjectName(QString::fromUtf8("installButton"));
        installButton->setMinimumSize(QSize(576, 41));
        installButton->setMaximumSize(QSize(576, 41));
        installButton->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"border-image:url(:/images/nice_black.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"\n"
"QPushButton#installButton:pressed\n"
"{\n"
"        border-image:url(:/images/nice_black.png) 10 10 10 10;\n"
"        border-top: 10px transparent;\n"
"        border-right: 10px transparent;\n"
"        border-bottom: 10px transparent;\n"
"        border-left: 10px transparent;\n"
"}\n"
""));

        verticalLayout_2->addWidget(installButton);

        uploadButton = new mPushButton(modeWidget);
        uploadButton->setObjectName(QString::fromUtf8("uploadButton"));
        uploadButton->setMinimumSize(QSize(576, 41));
        uploadButton->setMaximumSize(QSize(576, 41));
        uploadButton->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"border-image:url(:/images/nice_light.pngg) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;"));

        verticalLayout_2->addWidget(uploadButton);

        rebootButton = new mPushButton(modeWidget);
        rebootButton->setObjectName(QString::fromUtf8("rebootButton"));
        rebootButton->setMinimumSize(QSize(576, 41));
        rebootButton->setMaximumSize(QSize(576, 41));
        rebootButton->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"border-image:url(:/images/nice_light.pngg) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;"));

        verticalLayout_2->addWidget(rebootButton);


        horizontalLayout->addWidget(detailWidget);

        hleftSpace = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(hleftSpace);


        verticalLayout->addWidget(centerWidget);

        lineLabel = new QLabel(MainWindow);
        lineLabel->setObjectName(QString::fromUtf8("lineLabel"));
        lineLabel->setMinimumSize(QSize(800, 1));
        lineLabel->setMaximumSize(QSize(1920, 1));
        lineLabel->setStyleSheet(QString::fromUtf8("background-image: url(:/images/line.png);"));

        verticalLayout->addWidget(lineLabel);

        bottomWidget = new QLabel(MainWindow);
        bottomWidget->setObjectName(QString::fromUtf8("bottomWidget"));
        bottomWidget->setMinimumSize(QSize(800, 60));
        bottomWidget->setMaximumSize(QSize(1920, 60));
        bottomWidget->setStyleSheet(QString::fromUtf8("QWidget#bottomWidget{border-image: url(:/images/backgroud.png);}"));
        horizontalLayout_2 = new QHBoxLayout(bottomWidget);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setMargin(11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(20, 0, 10, 0);
        f2Label = new QLabel(bottomWidget);
        f2Label->setObjectName(QString::fromUtf8("f2Label"));
        f2Label->setMinimumSize(QSize(90, 0));
        f2Label->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(f2Label);

        arrowLabel = new QLabel(bottomWidget);
        arrowLabel->setObjectName(QString::fromUtf8("arrowLabel"));
        arrowLabel->setMinimumSize(QSize(110, 0));
        arrowLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(arrowLabel);

        enterLabel = new QLabel(bottomWidget);
        enterLabel->setObjectName(QString::fromUtf8("enterLabel"));
        enterLabel->setMinimumSize(QSize(120, 0));
        enterLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(enterLabel);

        hcenSpace = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(hcenSpace);

        netWidget = new QWidget(bottomWidget);
        netWidget->setObjectName(QString::fromUtf8("netWidget"));
        netWidget->setMinimumSize(QSize(80, 60));
        netWidget->setMaximumSize(QSize(80, 60));
        horizontalLayout_3 = new QHBoxLayout(netWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setMargin(11);
        horizontalLayout_3->setObjectName(QString::fromUtf8("horizontalLayout_3"));
        netpoint_label = new QLabel(netWidget);
        netpoint_label->setObjectName(QString::fromUtf8("netpoint_label"));
        netpoint_label->setMinimumSize(QSize(8, 8));
        netpoint_label->setMaximumSize(QSize(8, 8));
        netpoint_label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_3->addWidget(netpoint_label);

        netstate_label = new QLabel(netWidget);
        netstate_label->setObjectName(QString::fromUtf8("netstate_label"));
        netstate_label->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_3->addWidget(netstate_label);


        horizontalLayout_2->addWidget(netWidget);

        ipWidget = new QWidget(bottomWidget);
        ipWidget->setObjectName(QString::fromUtf8("ipWidget"));
        ipWidget->setMinimumSize(QSize(185, 60));
        ipWidget->setMaximumSize(QSize(185, 60));
        horizontalLayout_4 = new QHBoxLayout(ipWidget);
        horizontalLayout_4->setSpacing(6);
        horizontalLayout_4->setMargin(11);
        horizontalLayout_4->setObjectName(QString::fromUtf8("horizontalLayout_4"));
        neticon_label = new QLabel(ipWidget);
        neticon_label->setObjectName(QString::fromUtf8("neticon_label"));
        neticon_label->setMinimumSize(QSize(23, 19));
        neticon_label->setMaximumSize(QSize(23, 19));
        neticon_label->setStyleSheet(QString::fromUtf8(""));

        horizontalLayout_4->addWidget(neticon_label);

        ip_label = new QLabel(ipWidget);
        ip_label->setObjectName(QString::fromUtf8("ip_label"));
        ip_label->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_4->addWidget(ip_label);


        horizontalLayout_2->addWidget(ipWidget);

        dataLabel = new QLabel(bottomWidget);
        dataLabel->setObjectName(QString::fromUtf8("dataLabel"));
        dataLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(dataLabel);


        verticalLayout->addWidget(bottomWidget);


        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QWidget *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        modeLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\351\203\250\347\275\262\346\250\241\345\274\217</p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p>\346\202\250\345\267\262\347\273\217\350\277\233\345\205\245\351\203\250\347\275\262\346\250\241\345\274\217\357\274\214\345\217\257\344\273\245\346\211\247\350\241\214\344\273\245\344\270\213\346\223\215\344\275\234:</p></body></html>", 0, QApplication::UnicodeUTF8));
        installButton->setText(QApplication::translate("MainWindow", "\345\256\211\350\243\205\345\256\242\346\210\267\347\253\257(\346\255\244\346\223\215\344\275\234\345\260\206\346\240\274\345\274\217\345\214\226\347\273\210\347\253\257\347\241\254\347\233\230)", 0, QApplication::UnicodeUTF8));
        uploadButton->setText(QApplication::translate("MainWindow", "\345\260\206\346\234\254\345\234\260\347\263\273\347\273\237\344\275\234\344\270\272\346\250\241\346\235\277\344\270\212\344\274\240", 0, QApplication::UnicodeUTF8));
        rebootButton->setText(QApplication::translate("MainWindow", "\345\256\211\345\205\250\345\274\271\345\207\272U\347\233\230\345\271\266\351\207\215\345\220\257", 0, QApplication::UnicodeUTF8));
        lineLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        f2Label->setText(QApplication::translate("MainWindow", "[F2]\347\273\210\347\253\257\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        arrowLabel->setText(QApplication::translate("MainWindow", "<html><head/><body><p><span style=\" color:#ffffff;\">[\342\206\221][\342\206\223]\351\200\211\346\213\251\351\200\211\351\241\271</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        enterLabel->setText(QApplication::translate("MainWindow", "[Space]\351\200\211\346\213\251\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        netpoint_label->setText(QString());
        netstate_label->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">\345\234\250\347\272\277</p></body></html>", 0, QApplication::UnicodeUTF8));
        neticon_label->setText(QString());
        ip_label->setText(QApplication::translate("MainWindow", "<html><head/><body><p align=\"center\">192.168.255.255</p></body></html>", 0, QApplication::UnicodeUTF8));
        dataLabel->setText(QApplication::translate("MainWindow", "  00:00\n"
"2000/01/01", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(MainWindow);
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
