/********************************************************************************
** Form generated from reading UI file 'oswindow.ui'
**
** Created by: Qt User Interface Compiler version 4.8.6
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_OSWINDOW_H
#define UI_OSWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QCheckBox>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QPushButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QTextEdit>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_OSWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *backgroudWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QWidget *widget;
    QPushButton *pushButton_11;
    QLineEdit *template_name_edit;
    QLabel *dns2_label_3;
    QLabel *dns2_label_5;
    QLineEdit *template_size_edit;
    QLabel *dns2_label_4;
    QTextEdit *dst_edit;
    QPushButton *pushButton_12;
    QWidget *buttonLayout_2;
    QLabel *buttonLabel11_2;
    QWidget *widget_3;
    QCheckBox *checkBox;
    QLabel *label_19;
    QLabel *label_21;
    QWidget *widget_4;
    QCheckBox *checkBox_3;
    QLabel *label_3;
    QLabel *label_18;
    QLabel *label_8;
    QLabel *dns2_label_6;
    QLabel *dns2_label_7;
    QLabel *template_name_result_label;
    QSpacerItem *horizontalSpacer;
    QLabel *lineLabel;
    QLabel *bottomWidget;
    QHBoxLayout *horizontalLayout_2;
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

    void setupUi(QWidget *OSWindow)
    {
        if (OSWindow->objectName().isEmpty())
            OSWindow->setObjectName(QString::fromUtf8("OSWindow"));
        OSWindow->resize(1600, 900);
        OSWindow->setMinimumSize(QSize(800, 600));
        OSWindow->setMaximumSize(QSize(1920, 1080));
        OSWindow->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(OSWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setContentsMargins(11, 11, 11, 11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        backgroudWidget = new QLabel(OSWindow);
        backgroudWidget->setObjectName(QString::fromUtf8("backgroudWidget"));
        backgroudWidget->setMinimumSize(QSize(800, 600));
        backgroudWidget->setMaximumSize(QSize(1920, 1080));
        backgroudWidget->setStyleSheet(QString::fromUtf8("QWidget#backgroudWidget{border-image: url(:/images/backgroud.png);}"));
        horizontalLayout = new QHBoxLayout(backgroudWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setContentsMargins(11, 11, 11, 11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        widget = new QWidget(backgroudWidget);
        widget->setObjectName(QString::fromUtf8("widget"));
        widget->setMinimumSize(QSize(800, 600));
        widget->setMaximumSize(QSize(800, 600));
        pushButton_11 = new QPushButton(widget);
        pushButton_11->setObjectName(QString::fromUtf8("pushButton_11"));
        pushButton_11->setGeometry(QRect(270, 560, 91, 31));
        pushButton_11->setStyleSheet(QString::fromUtf8("border-image: url(:/images/bluebutton.png);\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        template_name_edit = new QLineEdit(widget);
        template_name_edit->setObjectName(QString::fromUtf8("template_name_edit"));
        template_name_edit->setGeometry(QRect(110, 320, 531, 41));
        template_name_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns2_label_3 = new QLabel(widget);
        dns2_label_3->setObjectName(QString::fromUtf8("dns2_label_3"));
        dns2_label_3->setGeometry(QRect(110, 290, 71, 21));
        dns2_label_3->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns2_label_5 = new QLabel(widget);
        dns2_label_5->setObjectName(QString::fromUtf8("dns2_label_5"));
        dns2_label_5->setGeometry(QRect(110, 460, 71, 21));
        dns2_label_5->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        template_size_edit = new QLineEdit(widget);
        template_size_edit->setObjectName(QString::fromUtf8("template_size_edit"));
        template_size_edit->setGeometry(QRect(110, 410, 531, 41));
        template_size_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns2_label_4 = new QLabel(widget);
        dns2_label_4->setObjectName(QString::fromUtf8("dns2_label_4"));
        dns2_label_4->setGeometry(QRect(100, 380, 81, 21));
        dns2_label_4->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dst_edit = new QTextEdit(widget);
        dst_edit->setObjectName(QString::fromUtf8("dst_edit"));
        dst_edit->setGeometry(QRect(110, 490, 531, 61));
        dst_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"background-color: rgba(255, 255, 255, 0);\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        pushButton_12 = new QPushButton(widget);
        pushButton_12->setObjectName(QString::fromUtf8("pushButton_12"));
        pushButton_12->setGeometry(QRect(390, 560, 91, 31));
        pushButton_12->setStyleSheet(QString::fromUtf8("border-image: url(:/images/bluebutton.png);\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        buttonLayout_2 = new QWidget(widget);
        buttonLayout_2->setObjectName(QString::fromUtf8("buttonLayout_2"));
        buttonLayout_2->setGeometry(QRect(110, 100, 531, 171));
        buttonLayout_2->setStyleSheet(QString::fromUtf8(""));
        buttonLabel11_2 = new QLabel(buttonLayout_2);
        buttonLabel11_2->setObjectName(QString::fromUtf8("buttonLabel11_2"));
        buttonLabel11_2->setGeometry(QRect(0, 0, 1101, 171));
        buttonLabel11_2->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
""));
        widget_3 = new QWidget(buttonLayout_2);
        widget_3->setObjectName(QString::fromUtf8("widget_3"));
        widget_3->setGeometry(QRect(0, 60, 1111, 51));
        checkBox = new QCheckBox(widget_3);
        checkBox->setObjectName(QString::fromUtf8("checkBox"));
        checkBox->setGeometry(QRect(30, 20, 71, 16));
        label_19 = new QLabel(widget_3);
        label_19->setObjectName(QString::fromUtf8("label_19"));
        label_19->setGeometry(QRect(110, 10, 141, 21));
        label_19->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        label_21 = new QLabel(widget_3);
        label_21->setObjectName(QString::fromUtf8("label_21"));
        label_21->setGeometry(QRect(350, 10, 121, 21));
        label_21->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        widget_4 = new QWidget(buttonLayout_2);
        widget_4->setObjectName(QString::fromUtf8("widget_4"));
        widget_4->setGeometry(QRect(0, 0, 1111, 51));
        checkBox_3 = new QCheckBox(widget_4);
        checkBox_3->setObjectName(QString::fromUtf8("checkBox_3"));
        checkBox_3->setGeometry(QRect(30, 20, 71, 16));
        label_3 = new QLabel(widget_4);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(130, 20, 101, 20));
        label_3->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        label_18 = new QLabel(widget_4);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(0, 0, 1101, 51));
        label_18->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_black.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";\n"
""));
        label_8 = new QLabel(widget_4);
        label_8->setObjectName(QString::fromUtf8("label_8"));
        label_8->setGeometry(QRect(360, 20, 101, 20));
        label_8->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        label_18->raise();
        label_3->raise();
        checkBox_3->raise();
        label_8->raise();
        dns2_label_6 = new QLabel(widget);
        dns2_label_6->setObjectName(QString::fromUtf8("dns2_label_6"));
        dns2_label_6->setGeometry(QRect(320, 10, 81, 31));
        dns2_label_6->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns2_label_7 = new QLabel(widget);
        dns2_label_7->setObjectName(QString::fromUtf8("dns2_label_7"));
        dns2_label_7->setGeometry(QRect(110, 70, 101, 21));
        dns2_label_7->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        template_name_result_label = new QLabel(widget);
        template_name_result_label->setObjectName(QString::fromUtf8("template_name_result_label"));
        template_name_result_label->setGeometry(QRect(520, 290, 111, 21));
        template_name_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));

        horizontalLayout->addWidget(widget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 1);

        verticalLayout->addWidget(backgroudWidget);

        lineLabel = new QLabel(OSWindow);
        lineLabel->setObjectName(QString::fromUtf8("lineLabel"));
        lineLabel->setMinimumSize(QSize(800, 1));
        lineLabel->setMaximumSize(QSize(1920, 1));
        lineLabel->setStyleSheet(QString::fromUtf8("background-image: url(:/images/line.png);"));

        verticalLayout->addWidget(lineLabel);

        bottomWidget = new QLabel(OSWindow);
        bottomWidget->setObjectName(QString::fromUtf8("bottomWidget"));
        bottomWidget->setMinimumSize(QSize(800, 60));
        bottomWidget->setMaximumSize(QSize(1920, 60));
        bottomWidget->setStyleSheet(QString::fromUtf8("QWidget#bottomWidget{border-image: url(:/images/backgroud.png);}"));
        horizontalLayout_2 = new QHBoxLayout(bottomWidget);
        horizontalLayout_2->setSpacing(10);
        horizontalLayout_2->setContentsMargins(11, 11, 11, 11);
        horizontalLayout_2->setObjectName(QString::fromUtf8("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(20, 0, 10, 0);
        hcenSpace = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(hcenSpace);

        netWidget = new QWidget(bottomWidget);
        netWidget->setObjectName(QString::fromUtf8("netWidget"));
        netWidget->setMinimumSize(QSize(80, 60));
        netWidget->setMaximumSize(QSize(80, 60));
        horizontalLayout_3 = new QHBoxLayout(netWidget);
        horizontalLayout_3->setSpacing(6);
        horizontalLayout_3->setContentsMargins(11, 11, 11, 11);
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
        horizontalLayout_4->setContentsMargins(11, 11, 11, 11);
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


        retranslateUi(OSWindow);

        QMetaObject::connectSlotsByName(OSWindow);
    } // setupUi

    void retranslateUi(QWidget *OSWindow)
    {
        OSWindow->setWindowTitle(QApplication::translate("OSWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        pushButton_11->setText(QApplication::translate("OSWindow", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        template_name_edit->setText(QString());
        dns2_label_3->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"right\">\346\250\241\346\235\277\345\220\215\347\247\260:</p></body></html>", 0, QApplication::UnicodeUTF8));
        dns2_label_5->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"right\">\346\250\241\346\235\277\346\217\217\350\277\260:</p></body></html>", 0, QApplication::UnicodeUTF8));
        template_size_edit->setText(QString());
        dns2_label_4->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"right\">\346\250\241\346\235\277\345\244\247\345\260\217:</p></body></html>", 0, QApplication::UnicodeUTF8));
        dst_edit->setHtml(QApplication::translate("OSWindow", "<!DOCTYPE HTML PUBLIC \"-//W3C//DTD HTML 4.0//EN\" \"http://www.w3.org/TR/REC-html40/strict.dtd\">\n"
"<html><head><meta name=\"qrichtext\" content=\"1\" /><style type=\"text/css\">\n"
"p, li { white-space: pre-wrap; }\n"
"</style></head><body style=\" font-family:'\351\273\221\344\275\223'; font-size:12pt; font-weight:400; font-style:normal;\">\n"
"<p style=\"-qt-paragraph-type:empty; margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\"><br /></p></body></html>", 0, QApplication::UnicodeUTF8));
        pushButton_12->setText(QApplication::translate("OSWindow", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        buttonLabel11_2->setText(QString());
        checkBox->setText(QString());
        label_19->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"center\">01</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_21->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"center\">1714MB</p></body></html>", 0, QApplication::UnicodeUTF8));
        checkBox_3->setText(QString());
        label_3->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"center\">\347\263\273\347\273\237\345\272\217\345\217\267</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_18->setText(QString());
        label_8->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"center\">\346\225\260\346\215\256\345\244\247\345\260\217</p></body></html>", 0, QApplication::UnicodeUTF8));
        dns2_label_6->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"right\"><span style=\" font-size:14pt;\">\351\203\250\347\275\262\346\250\241\345\274\217</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        dns2_label_7->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"right\">\344\270\212\344\274\240\346\240\267\346\234\272\346\250\241\346\235\277</p></body></html>", 0, QApplication::UnicodeUTF8));
        template_name_result_label->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"right\">\350\257\267\350\276\223\345\205\245\346\250\241\346\235\277\345\220\215\347\247\260</p></body></html>", 0, QApplication::UnicodeUTF8));
        lineLabel->setText(QApplication::translate("OSWindow", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        netpoint_label->setText(QString());
        netstate_label->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"center\">\345\234\250\347\272\277</p></body></html>", 0, QApplication::UnicodeUTF8));
        neticon_label->setText(QString());
        ip_label->setText(QApplication::translate("OSWindow", "<html><head/><body><p align=\"center\">192.168.255.255</p></body></html>", 0, QApplication::UnicodeUTF8));
        dataLabel->setText(QApplication::translate("OSWindow", "  00:00\n"
"2000/01/01", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class OSWindow: public Ui_OSWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_OSWINDOW_H
