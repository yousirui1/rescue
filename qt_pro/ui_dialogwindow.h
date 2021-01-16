/********************************************************************************
** Form generated from reading ui file 'dialogwindow.ui'
**
** Created: Thu Jan 14 15:04:31 2021
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_DIALOGWINDOW_H
#define UI_DIALOGWINDOW_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QHBoxLayout>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QRadioButton>
#include <QtGui/QSpacerItem>
#include <QtGui/QVBoxLayout>
#include <QtGui/QWidget>
#include "qbutton2.h"

QT_BEGIN_NAMESPACE

class Ui_DialogWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *backgroudWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *horizontalSpacer_2;
    QWidget *dialog_widget;
    QWidget *error_dialog;
    QLabel *label_79;
    QLabel *error_msg_label;
    QLabel *backgroud_init_3;
    QButton2 *error_confirm_btn;
    QWidget *net_dialog;
    QButton2 *cacel_btn;
    QButton2 *confirm_btn;
    QLabel *label_15;
    QLabel *label_16;
    QLabel *label_17;
    QLabel *networkWigte_2;
    QLabel *label_3;
    QLabel *label_18;
    QLabel *lineLabel;
    QWidget *netmask_widget;
    QLabel *netmask_label;
    QLabel *netmask_result_label;
    QLineEdit *netmask_edit;
    QWidget *server_ip_widget;
    QLabel *server_ip_label;
    QLabel *server_ip_result_label;
    QLineEdit *server_ip_edit;
    QWidget *dns_widget;
    QLabel *dns_label;
    QLabel *dns_result_label;
    QLineEdit *dns_edit;
    QRadioButton *static_radio;
    QWidget *gateway_widget;
    QLabel *gateway_label;
    QLabel *gateway_result_label;
    QLineEdit *gateway_edit;
    QWidget *ip_widget;
    QLabel *ip_label;
    QLabel *ip_result_label;
    QLineEdit *ip_edit;
    QRadioButton *dhcp_radio;
    QWidget *dns2_widget;
    QLabel *dns2_label;
    QLabel *dns2_result_label;
    QLineEdit *dns2_edit;
    QLabel *label_100;
    QWidget *install_dialog;
    QLabel *label_83;
    QLabel *label_84;
    QButton2 *install_cacel_btn;
    QLabel *backgroud_init_5;
    QButton2 *install_confirm_btn;
    QSpacerItem *horizontalSpacer;

    void setupUi(QWidget *DialogWindow)
    {
        if (DialogWindow->objectName().isEmpty())
            DialogWindow->setObjectName(QString::fromUtf8("DialogWindow"));
        DialogWindow->resize(1600, 900);
        DialogWindow->setMinimumSize(QSize(800, 600));
        DialogWindow->setMaximumSize(QSize(1920, 1080));
        DialogWindow->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(DialogWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        backgroudWidget = new QLabel(DialogWindow);
        backgroudWidget->setObjectName(QString::fromUtf8("backgroudWidget"));
        backgroudWidget->setMinimumSize(QSize(800, 600));
        backgroudWidget->setMaximumSize(QSize(1920, 1080));
        backgroudWidget->setStyleSheet(QString::fromUtf8("QLabel#backgroudWidget{border-image: url(:/images/backgroud.png);}"));
        horizontalLayout = new QHBoxLayout(backgroudWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        horizontalSpacer_2 = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer_2);

        dialog_widget = new QWidget(backgroudWidget);
        dialog_widget->setObjectName(QString::fromUtf8("dialog_widget"));
        dialog_widget->setMinimumSize(QSize(800, 600));
        dialog_widget->setMaximumSize(QSize(800, 600));
        error_dialog = new QWidget(dialog_widget);
        error_dialog->setObjectName(QString::fromUtf8("error_dialog"));
        error_dialog->setGeometry(QRect(150, 100, 497, 273));
        label_79 = new QLabel(error_dialog);
        label_79->setObjectName(QString::fromUtf8("label_79"));
        label_79->setGeometry(QRect(200, 30, 101, 31));
        label_79->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        error_msg_label = new QLabel(error_dialog);
        error_msg_label->setObjectName(QString::fromUtf8("error_msg_label"));
        error_msg_label->setGeometry(QRect(90, 70, 321, 101));
        error_msg_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        backgroud_init_3 = new QLabel(error_dialog);
        backgroud_init_3->setObjectName(QString::fromUtf8("backgroud_init_3"));
        backgroud_init_3->setGeometry(QRect(0, 0, 497, 273));
        backgroud_init_3->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
""));
        error_confirm_btn = new QButton2(error_dialog);
        error_confirm_btn->setObjectName(QString::fromUtf8("error_confirm_btn"));
        error_confirm_btn->setGeometry(QRect(200, 200, 91, 31));
        error_confirm_btn->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        backgroud_init_3->raise();
        label_79->raise();
        error_msg_label->raise();
        error_confirm_btn->raise();
        net_dialog = new QWidget(dialog_widget);
        net_dialog->setObjectName(QString::fromUtf8("net_dialog"));
        net_dialog->setGeometry(QRect(0, 0, 1000, 900));
        cacel_btn = new QButton2(net_dialog);
        cacel_btn->setObjectName(QString::fromUtf8("cacel_btn"));
        cacel_btn->setGeometry(QRect(430, 550, 102, 37));
        cacel_btn->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        confirm_btn = new QButton2(net_dialog);
        confirm_btn->setObjectName(QString::fromUtf8("confirm_btn"));
        confirm_btn->setGeometry(QRect(620, 550, 102, 37));
        confirm_btn->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        label_15 = new QLabel(net_dialog);
        label_15->setObjectName(QString::fromUtf8("label_15"));
        label_15->setGeometry(QRect(0, 63, 192, 551));
        label_15->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
""));
        label_16 = new QLabel(net_dialog);
        label_16->setObjectName(QString::fromUtf8("label_16"));
        label_16->setGeometry(QRect(31, 18, 17, 14));
        label_16->setStyleSheet(QString::fromUtf8("image: url(:/images/network.png);"));
        label_17 = new QLabel(net_dialog);
        label_17->setObjectName(QString::fromUtf8("label_17"));
        label_17->setGeometry(QRect(237, 26, 81, 21));
        label_17->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        networkWigte_2 = new QLabel(net_dialog);
        networkWigte_2->setObjectName(QString::fromUtf8("networkWigte_2"));
        networkWigte_2->setGeometry(QRect(0, 0, 1005, 614));
        networkWigte_2->setStyleSheet(QString::fromUtf8(""));
        label_3 = new QLabel(net_dialog);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        label_3->setGeometry(QRect(0, 0, 1005, 614));
        label_3->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
""));
        label_18 = new QLabel(net_dialog);
        label_18->setObjectName(QString::fromUtf8("label_18"));
        label_18->setGeometry(QRect(60, 18, 71, 20));
        label_18->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        lineLabel = new QLabel(net_dialog);
        lineLabel->setObjectName(QString::fromUtf8("lineLabel"));
        lineLabel->setGeometry(QRect(200, 538, 600, 1));
        lineLabel->setStyleSheet(QString::fromUtf8("border-image: url(:/images/line.png);"));
        netmask_widget = new QWidget(net_dialog);
        netmask_widget->setObjectName(QString::fromUtf8("netmask_widget"));
        netmask_widget->setGeometry(QRect(340, 180, 381, 45));
        netmask_label = new QLabel(netmask_widget);
        netmask_label->setObjectName(QString::fromUtf8("netmask_label"));
        netmask_label->setGeometry(QRect(20, 10, 71, 21));
        netmask_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        netmask_result_label = new QLabel(netmask_widget);
        netmask_result_label->setObjectName(QString::fromUtf8("netmask_result_label"));
        netmask_result_label->setGeometry(QRect(310, 10, 71, 21));
        netmask_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        netmask_edit = new QLineEdit(netmask_widget);
        netmask_edit->setObjectName(QString::fromUtf8("netmask_edit"));
        netmask_edit->setGeometry(QRect(100, 5, 201, 33));
        netmask_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        server_ip_widget = new QWidget(net_dialog);
        server_ip_widget->setObjectName(QString::fromUtf8("server_ip_widget"));
        server_ip_widget->setGeometry(QRect(340, 460, 381, 45));
        server_ip_label = new QLabel(server_ip_widget);
        server_ip_label->setObjectName(QString::fromUtf8("server_ip_label"));
        server_ip_label->setGeometry(QRect(20, 10, 71, 21));
        server_ip_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        server_ip_result_label = new QLabel(server_ip_widget);
        server_ip_result_label->setObjectName(QString::fromUtf8("server_ip_result_label"));
        server_ip_result_label->setGeometry(QRect(310, 10, 71, 21));
        server_ip_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        server_ip_edit = new QLineEdit(server_ip_widget);
        server_ip_edit->setObjectName(QString::fromUtf8("server_ip_edit"));
        server_ip_edit->setGeometry(QRect(100, 5, 201, 33));
        server_ip_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns_widget = new QWidget(net_dialog);
        dns_widget->setObjectName(QString::fromUtf8("dns_widget"));
        dns_widget->setGeometry(QRect(310, 280, 411, 41));
        dns_label = new QLabel(dns_widget);
        dns_label->setObjectName(QString::fromUtf8("dns_label"));
        dns_label->setGeometry(QRect(10, 10, 111, 21));
        dns_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns_result_label = new QLabel(dns_widget);
        dns_result_label->setObjectName(QString::fromUtf8("dns_result_label"));
        dns_result_label->setGeometry(QRect(340, 10, 71, 21));
        dns_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns_edit = new QLineEdit(dns_widget);
        dns_edit->setObjectName(QString::fromUtf8("dns_edit"));
        dns_edit->setGeometry(QRect(130, 5, 201, 33));
        dns_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        static_radio = new QRadioButton(net_dialog);
        static_radio->setObjectName(QString::fromUtf8("static_radio"));
        static_radio->setGeometry(QRect(340, 100, 161, 21));
        static_radio->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));
        gateway_widget = new QWidget(net_dialog);
        gateway_widget->setObjectName(QString::fromUtf8("gateway_widget"));
        gateway_widget->setGeometry(QRect(340, 230, 381, 45));
        gateway_label = new QLabel(gateway_widget);
        gateway_label->setObjectName(QString::fromUtf8("gateway_label"));
        gateway_label->setGeometry(QRect(20, 10, 71, 21));
        gateway_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        gateway_result_label = new QLabel(gateway_widget);
        gateway_result_label->setObjectName(QString::fromUtf8("gateway_result_label"));
        gateway_result_label->setGeometry(QRect(310, 10, 71, 21));
        gateway_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        gateway_edit = new QLineEdit(gateway_widget);
        gateway_edit->setObjectName(QString::fromUtf8("gateway_edit"));
        gateway_edit->setGeometry(QRect(100, 5, 201, 33));
        gateway_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        ip_widget = new QWidget(net_dialog);
        ip_widget->setObjectName(QString::fromUtf8("ip_widget"));
        ip_widget->setGeometry(QRect(370, 130, 351, 45));
        ip_label = new QLabel(ip_widget);
        ip_label->setObjectName(QString::fromUtf8("ip_label"));
        ip_label->setGeometry(QRect(10, 15, 71, 21));
        ip_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        ip_result_label = new QLabel(ip_widget);
        ip_result_label->setObjectName(QString::fromUtf8("ip_result_label"));
        ip_result_label->setGeometry(QRect(280, 15, 81, 21));
        ip_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        ip_edit = new QLineEdit(ip_widget);
        ip_edit->setObjectName(QString::fromUtf8("ip_edit"));
        ip_edit->setGeometry(QRect(70, 7, 201, 33));
        ip_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dhcp_radio = new QRadioButton(net_dialog);
        dhcp_radio->setObjectName(QString::fromUtf8("dhcp_radio"));
        dhcp_radio->setGeometry(QRect(340, 60, 161, 31));
        dhcp_radio->setStyleSheet(QString::fromUtf8("\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dhcp_radio->setChecked(true);
        dns2_widget = new QWidget(net_dialog);
        dns2_widget->setObjectName(QString::fromUtf8("dns2_widget"));
        dns2_widget->setGeometry(QRect(310, 330, 411, 41));
        dns2_label = new QLabel(dns2_widget);
        dns2_label->setObjectName(QString::fromUtf8("dns2_label"));
        dns2_label->setGeometry(QRect(10, 10, 111, 21));
        dns2_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns2_result_label = new QLabel(dns2_widget);
        dns2_result_label->setObjectName(QString::fromUtf8("dns2_result_label"));
        dns2_result_label->setGeometry(QRect(340, 10, 71, 21));
        dns2_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        dns2_edit = new QLineEdit(dns2_widget);
        dns2_edit->setObjectName(QString::fromUtf8("dns2_edit"));
        dns2_edit->setGeometry(QRect(130, 5, 201, 33));
        dns2_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        label_100 = new QLabel(net_dialog);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(260, 430, 141, 21));
        label_100->setStyleSheet(QString::fromUtf8("font: 14pt \"\351\273\221\344\275\223\";"));
        networkWigte_2->raise();
        label_3->raise();
        cacel_btn->raise();
        confirm_btn->raise();
        label_15->raise();
        label_16->raise();
        label_17->raise();
        label_18->raise();
        lineLabel->raise();
        netmask_widget->raise();
        server_ip_widget->raise();
        dns_widget->raise();
        static_radio->raise();
        gateway_widget->raise();
        ip_widget->raise();
        dhcp_radio->raise();
        dns2_widget->raise();
        label_100->raise();
        install_dialog = new QWidget(dialog_widget);
        install_dialog->setObjectName(QString::fromUtf8("install_dialog"));
        install_dialog->setGeometry(QRect(150, 100, 497, 273));
        label_83 = new QLabel(install_dialog);
        label_83->setObjectName(QString::fromUtf8("label_83"));
        label_83->setGeometry(QRect(200, 30, 101, 31));
        label_83->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        label_84 = new QLabel(install_dialog);
        label_84->setObjectName(QString::fromUtf8("label_84"));
        label_84->setGeometry(QRect(90, 110, 171, 31));
        label_84->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        install_cacel_btn = new QButton2(install_dialog);
        install_cacel_btn->setObjectName(QString::fromUtf8("install_cacel_btn"));
        install_cacel_btn->setGeometry(QRect(150, 190, 91, 33));
        install_cacel_btn->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        backgroud_init_5 = new QLabel(install_dialog);
        backgroud_init_5->setObjectName(QString::fromUtf8("backgroud_init_5"));
        backgroud_init_5->setGeometry(QRect(0, 0, 497, 273));
        backgroud_init_5->setStyleSheet(QString::fromUtf8("border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
""));
        install_confirm_btn = new QButton2(install_dialog);
        install_confirm_btn->setObjectName(QString::fromUtf8("install_confirm_btn"));
        install_confirm_btn->setGeometry(QRect(280, 190, 91, 33));
        install_confirm_btn->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        backgroud_init_5->raise();
        label_83->raise();
        label_84->raise();
        install_cacel_btn->raise();
        install_confirm_btn->raise();

        horizontalLayout->addWidget(dialog_widget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(2, 1);

        verticalLayout->addWidget(backgroudWidget);

        QWidget::setTabOrder(dhcp_radio, static_radio);
        QWidget::setTabOrder(static_radio, ip_edit);
        QWidget::setTabOrder(ip_edit, netmask_edit);
        QWidget::setTabOrder(netmask_edit, gateway_edit);
        QWidget::setTabOrder(gateway_edit, dns_edit);
        QWidget::setTabOrder(dns_edit, dns2_edit);
        QWidget::setTabOrder(dns2_edit, server_ip_edit);
        QWidget::setTabOrder(server_ip_edit, cacel_btn);
        QWidget::setTabOrder(cacel_btn, confirm_btn);
        QWidget::setTabOrder(confirm_btn, error_confirm_btn);
        QWidget::setTabOrder(error_confirm_btn, install_cacel_btn);
        QWidget::setTabOrder(install_cacel_btn, install_confirm_btn);

        retranslateUi(DialogWindow);

        QMetaObject::connectSlotsByName(DialogWindow);
    } // setupUi

    void retranslateUi(QWidget *DialogWindow)
    {
        DialogWindow->setWindowTitle(QApplication::translate("DialogWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label_79->setText(QApplication::translate("DialogWindow", "<html><head/><body><p align=\"center\">\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        error_msg_label->setText(QApplication::translate("DialogWindow", "\346\230\257\345\220\246\347\241\256\350\256\244\345\215\270\350\275\275\345\256\242\346\210\267\347\253\257?", 0, QApplication::UnicodeUTF8));
        backgroud_init_3->setText(QString());
        error_confirm_btn->setText(QApplication::translate("DialogWindow", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        cacel_btn->setText(QApplication::translate("DialogWindow", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        confirm_btn->setText(QApplication::translate("DialogWindow", "\347\241\256\345\256\232", 0, QApplication::UnicodeUTF8));
        label_15->setText(QString());
        label_16->setText(QString());
        label_17->setText(QApplication::translate("DialogWindow", "<html><head/><body><p><span style=\" font-size:12pt; color:#ffffff;\">IP\345\234\260\345\235\200\351\205\215\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        networkWigte_2->setText(QString());
        label_3->setText(QString());
        label_18->setText(QApplication::translate("DialogWindow", "<html><head/><body><p><span style=\" font-size:12pt; color:#ffffff;\">\347\275\221\347\273\234\351\205\215\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        lineLabel->setText(QApplication::translate("DialogWindow", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        netmask_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\345\255\220\347\275\221\346\216\251\347\240\201:</p></body></html>", 0, QApplication::UnicodeUTF8));
        netmask_result_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        server_ip_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\346\234\215\345\212\241\345\231\250IP\357\274\232</p></body></html>", 0, QApplication::UnicodeUTF8));
        server_ip_result_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        dns_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\351\246\226\351\200\211DNS\346\234\215\345\212\241\345\231\250:</p></body></html>", 0, QApplication::UnicodeUTF8));
        dns_result_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        static_radio->setText(QApplication::translate("DialogWindow", "\344\275\277\347\224\250\344\270\213\346\226\271IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        gateway_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\351\273\230\350\256\244\347\275\221\345\205\263:</p></body></html>", 0, QApplication::UnicodeUTF8));
        gateway_result_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        ip_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>IP\345\234\260\345\235\200:</p></body></html>", 0, QApplication::UnicodeUTF8));
        ip_result_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        dhcp_radio->setText(QApplication::translate("DialogWindow", "\350\207\252\345\212\250\350\216\267\345\217\226IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        dns2_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\345\244\207\351\200\211DNS\346\234\215\345\212\241\345\231\250:</p></body></html>", 0, QApplication::UnicodeUTF8));
        dns2_result_label->setText(QApplication::translate("DialogWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_100->setText(QApplication::translate("DialogWindow", "<html><head/><body><p><span style=\" font-size:12pt; color:#ffffff;\">\346\234\215\345\212\241\345\231\250\350\277\236\346\216\245\351\205\215\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        label_83->setText(QApplication::translate("DialogWindow", "<html><head/><body><p align=\"center\">\346\217\220\347\244\272</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_84->setText(QApplication::translate("DialogWindow", "\346\230\257\345\220\246\347\241\256\350\256\244\345\256\211\350\243\205\345\256\242\346\210\267\347\253\257?", 0, QApplication::UnicodeUTF8));
        install_cacel_btn->setText(QApplication::translate("DialogWindow", "\345\217\226\346\266\210", 0, QApplication::UnicodeUTF8));
        backgroud_init_5->setText(QString());
        install_confirm_btn->setText(QApplication::translate("DialogWindow", "\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(DialogWindow);
    } // retranslateUi

};

namespace Ui {
    class DialogWindow: public Ui_DialogWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_DIALOGWINDOW_H
