/********************************************************************************
** Form generated from reading ui file 'configwindow.ui'
**
** Created: Wed Nov 11 22:10:35 2020
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_CONFIGWINDOW_H
#define UI_CONFIGWINDOW_H

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

class Ui_ConfigWindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *backgroudWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *hrightSpace;
    QLabel *detailWidget;
    QWidget *deskconfig_widget;
    QLabel *label_67;
    QWidget *num_widget;
    QLabel *num_label;
    QLineEdit *num_edit;
    QLabel *num_result_label;
    QButton2 *laststepButton;
    QRadioButton *auto_in_cloud_radio;
    QLabel *linue1_label_2;
    QButton2 *formatButton;
    QLabel *label_66;
    QWidget *hostname_widget;
    QLabel *hostname_label;
    QLineEdit *prefix_edit;
    QLineEdit *hostname_edit;
    QLabel *linue1_label_3;
    QLabel *hostname_result_label;
    QWidget *netconfig_widget;
    QRadioButton *dhcp_radio;
    QWidget *dns_widget;
    QLabel *dns_label;
    QLabel *dns_result_label;
    QLineEdit *dns_edit;
    QWidget *gateway_widget;
    QLabel *gateway_label;
    QLabel *gateway_result_label;
    QLineEdit *gateway_edit;
    QLabel *label_26;
    QRadioButton *static_radio;
    QWidget *dns2_widget;
    QLabel *dns2_label;
    QLabel *dns2_result_label;
    QLineEdit *dns2_edit;
    QWidget *netmask_widget;
    QLabel *netmask_label;
    QLabel *netmask_result_label;
    QLineEdit *netmask_edit;
    QLabel *lineLabel;
    QWidget *ip_widget;
    QLabel *ip_label;
    QLabel *ip_result_label;
    QLineEdit *ip_edit;
    QLabel *label_100;
    QWidget *server_ip_widget;
    QLabel *server_ip_label;
    QLabel *server_ip_result_label;
    QLineEdit *server_ip_edit;
    QButton2 *nextButton;
    QSpacerItem *horizontalSpacer;
    QLabel *lineLabel_2;
    QLabel *bottomWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *f2Label;
    QLabel *arrowLabel;
    QLabel *enterLabel;
    QSpacerItem *hcenSpace;
    QLabel *dataLabel;

    void setupUi(QWidget *ConfigWindow)
    {
        if (ConfigWindow->objectName().isEmpty())
            ConfigWindow->setObjectName(QString::fromUtf8("ConfigWindow"));
        ConfigWindow->resize(1600, 900);
        ConfigWindow->setMinimumSize(QSize(800, 600));
        ConfigWindow->setMaximumSize(QSize(1920, 1080));
        ConfigWindow->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(ConfigWindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        backgroudWidget = new QLabel(ConfigWindow);
        backgroudWidget->setObjectName(QString::fromUtf8("backgroudWidget"));
        backgroudWidget->setMinimumSize(QSize(800, 600));
        backgroudWidget->setMaximumSize(QSize(1920, 1080));
        backgroudWidget->setStyleSheet(QString::fromUtf8("QWidget#backgroudWidget{border-image: url(:/images/backgroud.png);}"));
        horizontalLayout = new QHBoxLayout(backgroudWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        hrightSpace = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(hrightSpace);

        detailWidget = new QLabel(backgroudWidget);
        detailWidget->setObjectName(QString::fromUtf8("detailWidget"));
        detailWidget->setMinimumSize(QSize(576, 549));
        detailWidget->setMaximumSize(QSize(576, 549));
        detailWidget->setStyleSheet(QString::fromUtf8("QWidget#detailWidget{\n"
"border-image:url(:/images/nice_light.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;}\n"
""));
        deskconfig_widget = new QWidget(detailWidget);
        deskconfig_widget->setObjectName(QString::fromUtf8("deskconfig_widget"));
        deskconfig_widget->setGeometry(QRect(0, 0, 576, 549));
        deskconfig_widget->setMinimumSize(QSize(576, 549));
        deskconfig_widget->setMaximumSize(QSize(576, 549));
        label_67 = new QLabel(deskconfig_widget);
        label_67->setObjectName(QString::fromUtf8("label_67"));
        label_67->setGeometry(QRect(80, 170, 131, 31));
        label_67->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        num_widget = new QWidget(deskconfig_widget);
        num_widget->setObjectName(QString::fromUtf8("num_widget"));
        num_widget->setGeometry(QRect(120, 50, 411, 45));
        num_label = new QLabel(num_widget);
        num_label->setObjectName(QString::fromUtf8("num_label"));
        num_label->setGeometry(QRect(20, 10, 71, 21));
        num_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        num_edit = new QLineEdit(num_widget);
        num_edit->setObjectName(QString::fromUtf8("num_edit"));
        num_edit->setGeometry(QRect(100, 5, 201, 33));
        num_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        num_result_label = new QLabel(num_widget);
        num_result_label->setObjectName(QString::fromUtf8("num_result_label"));
        num_result_label->setGeometry(QRect(320, 10, 71, 21));
        num_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        laststepButton = new QButton2(deskconfig_widget);
        laststepButton->setObjectName(QString::fromUtf8("laststepButton"));
        laststepButton->setGeometry(QRect(140, 500, 91, 33));
        laststepButton->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        auto_in_cloud_radio = new QRadioButton(deskconfig_widget);
        auto_in_cloud_radio->setObjectName(QString::fromUtf8("auto_in_cloud_radio"));
        auto_in_cloud_radio->setGeometry(QRect(230, 170, 101, 31));
        auto_in_cloud_radio->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));
        auto_in_cloud_radio->setChecked(true);
        linue1_label_2 = new QLabel(deskconfig_widget);
        linue1_label_2->setObjectName(QString::fromUtf8("linue1_label_2"));
        linue1_label_2->setGeometry(QRect(0, 480, 576, 1));
        linue1_label_2->setStyleSheet(QString::fromUtf8("border-image: url(:/images/line.png);"));
        formatButton = new QButton2(deskconfig_widget);
        formatButton->setObjectName(QString::fromUtf8("formatButton"));
        formatButton->setGeometry(QRect(340, 500, 101, 33));
        formatButton->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        label_66 = new QLabel(deskconfig_widget);
        label_66->setObjectName(QString::fromUtf8("label_66"));
        label_66->setGeometry(QRect(30, 20, 81, 31));
        label_66->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        hostname_widget = new QWidget(deskconfig_widget);
        hostname_widget->setObjectName(QString::fromUtf8("hostname_widget"));
        hostname_widget->setGeometry(QRect(120, 110, 411, 45));
        hostname_label = new QLabel(hostname_widget);
        hostname_label->setObjectName(QString::fromUtf8("hostname_label"));
        hostname_label->setGeometry(QRect(20, 10, 71, 21));
        hostname_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        prefix_edit = new QLineEdit(hostname_widget);
        prefix_edit->setObjectName(QString::fromUtf8("prefix_edit"));
        prefix_edit->setGeometry(QRect(100, 3, 81, 33));
        prefix_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        hostname_edit = new QLineEdit(hostname_widget);
        hostname_edit->setObjectName(QString::fromUtf8("hostname_edit"));
        hostname_edit->setGeometry(QRect(210, 3, 101, 33));
        hostname_edit->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        linue1_label_3 = new QLabel(hostname_widget);
        linue1_label_3->setObjectName(QString::fromUtf8("linue1_label_3"));
        linue1_label_3->setGeometry(QRect(185, 20, 20, 1));
        linue1_label_3->setStyleSheet(QString::fromUtf8("border-image: url(:/images/line.png);"));
        hostname_result_label = new QLabel(hostname_widget);
        hostname_result_label->setObjectName(QString::fromUtf8("hostname_result_label"));
        hostname_result_label->setGeometry(QRect(320, 10, 71, 21));
        hostname_result_label->setStyleSheet(QString::fromUtf8("color: #ff3636;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        netconfig_widget = new QWidget(detailWidget);
        netconfig_widget->setObjectName(QString::fromUtf8("netconfig_widget"));
        netconfig_widget->setGeometry(QRect(0, 0, 576, 549));
        netconfig_widget->setMinimumSize(QSize(576, 549));
        netconfig_widget->setMaximumSize(QSize(576, 549));
        dhcp_radio = new QRadioButton(netconfig_widget);
        dhcp_radio->setObjectName(QString::fromUtf8("dhcp_radio"));
        dhcp_radio->setGeometry(QRect(120, 40, 161, 31));
        dhcp_radio->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));
        dhcp_radio->setCheckable(true);
        dhcp_radio->setChecked(true);
        dns_widget = new QWidget(netconfig_widget);
        dns_widget->setObjectName(QString::fromUtf8("dns_widget"));
        dns_widget->setGeometry(QRect(120, 260, 411, 41));
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
        gateway_widget = new QWidget(netconfig_widget);
        gateway_widget->setObjectName(QString::fromUtf8("gateway_widget"));
        gateway_widget->setGeometry(QRect(150, 210, 381, 45));
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
        label_26 = new QLabel(netconfig_widget);
        label_26->setObjectName(QString::fromUtf8("label_26"));
        label_26->setGeometry(QRect(30, 20, 81, 31));
        label_26->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));
        static_radio = new QRadioButton(netconfig_widget);
        static_radio->setObjectName(QString::fromUtf8("static_radio"));
        static_radio->setGeometry(QRect(120, 80, 161, 21));
        static_radio->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";"));
        dns2_widget = new QWidget(netconfig_widget);
        dns2_widget->setObjectName(QString::fromUtf8("dns2_widget"));
        dns2_widget->setGeometry(QRect(120, 310, 411, 41));
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
        netmask_widget = new QWidget(netconfig_widget);
        netmask_widget->setObjectName(QString::fromUtf8("netmask_widget"));
        netmask_widget->setGeometry(QRect(150, 160, 381, 45));
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
        lineLabel = new QLabel(netconfig_widget);
        lineLabel->setObjectName(QString::fromUtf8("lineLabel"));
        lineLabel->setGeometry(QRect(0, 480, 576, 1));
        lineLabel->setStyleSheet(QString::fromUtf8("border-image: url(:/images/line.png);"));
        ip_widget = new QWidget(netconfig_widget);
        ip_widget->setObjectName(QString::fromUtf8("ip_widget"));
        ip_widget->setGeometry(QRect(180, 110, 351, 45));
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
        label_100 = new QLabel(netconfig_widget);
        label_100->setObjectName(QString::fromUtf8("label_100"));
        label_100->setGeometry(QRect(20, 390, 141, 21));
        label_100->setStyleSheet(QString::fromUtf8("font: 14pt \"\351\273\221\344\275\223\";"));
        server_ip_widget = new QWidget(netconfig_widget);
        server_ip_widget->setObjectName(QString::fromUtf8("server_ip_widget"));
        server_ip_widget->setGeometry(QRect(150, 420, 381, 45));
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
        nextButton = new QButton2(netconfig_widget);
        nextButton->setObjectName(QString::fromUtf8("nextButton"));
        nextButton->setGeometry(QRect(250, 500, 91, 33));
        nextButton->setStyleSheet(QString::fromUtf8("border-image:url(:/images/textEdit.png) 10 10 10 10;\n"
"border-top: 10px transparent;\n"
"border-right: 10px transparent;\n"
"border-bottom: 10px transparent;\n"
"border-left: 10px transparent;\n"
"color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        server_ip_widget->raise();
        dns2_widget->raise();
        dns_widget->raise();
        gateway_widget->raise();
        netmask_widget->raise();
        ip_widget->raise();
        dhcp_radio->raise();
        label_26->raise();
        static_radio->raise();
        lineLabel->raise();
        label_100->raise();
        nextButton->raise();

        horizontalLayout->addWidget(detailWidget);

        horizontalSpacer = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 1);

        verticalLayout->addWidget(backgroudWidget);

        lineLabel_2 = new QLabel(ConfigWindow);
        lineLabel_2->setObjectName(QString::fromUtf8("lineLabel_2"));
        lineLabel_2->setMinimumSize(QSize(9999, 1));
        lineLabel_2->setMaximumSize(QSize(9999, 1));
        lineLabel_2->setStyleSheet(QString::fromUtf8("\n"
"background-image: url(:/images/line.png);"));

        verticalLayout->addWidget(lineLabel_2);

        bottomWidget = new QLabel(ConfigWindow);
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
        f2Label->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(f2Label);

        arrowLabel = new QLabel(bottomWidget);
        arrowLabel->setObjectName(QString::fromUtf8("arrowLabel"));
        arrowLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(arrowLabel);

        enterLabel = new QLabel(bottomWidget);
        enterLabel->setObjectName(QString::fromUtf8("enterLabel"));
        enterLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(enterLabel);

        hcenSpace = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout_2->addItem(hcenSpace);

        dataLabel = new QLabel(bottomWidget);
        dataLabel->setObjectName(QString::fromUtf8("dataLabel"));
        dataLabel->setStyleSheet(QString::fromUtf8("font: 12pt \"\351\273\221\344\275\223\";\n"
"color: #FFFFFF;"));

        horizontalLayout_2->addWidget(dataLabel);


        verticalLayout->addWidget(bottomWidget);

        QWidget::setTabOrder(num_edit, prefix_edit);
        QWidget::setTabOrder(prefix_edit, hostname_edit);
        QWidget::setTabOrder(hostname_edit, auto_in_cloud_radio);
        QWidget::setTabOrder(auto_in_cloud_radio, laststepButton);
        QWidget::setTabOrder(laststepButton, formatButton);
        QWidget::setTabOrder(formatButton, static_radio);
        QWidget::setTabOrder(static_radio, dhcp_radio);
        QWidget::setTabOrder(dhcp_radio, nextButton);
        QWidget::setTabOrder(nextButton, dns_edit);
        QWidget::setTabOrder(dns_edit, gateway_edit);
        QWidget::setTabOrder(gateway_edit, dns2_edit);
        QWidget::setTabOrder(dns2_edit, netmask_edit);
        QWidget::setTabOrder(netmask_edit, ip_edit);
        QWidget::setTabOrder(ip_edit, server_ip_edit);

        retranslateUi(ConfigWindow);

        QMetaObject::connectSlotsByName(ConfigWindow);
    } // setupUi

    void retranslateUi(QWidget *ConfigWindow)
    {
        ConfigWindow->setWindowTitle(QApplication::translate("ConfigWindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        label_67->setText(QApplication::translate("ConfigWindow", "\350\207\252\345\212\250\350\277\233\345\205\245\351\273\230\350\256\244\346\241\214\351\235\242", 0, QApplication::UnicodeUTF8));
        num_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p align=\"right\">\345\272\217\345\217\267:</p></body></html>", 0, QApplication::UnicodeUTF8));
        num_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        laststepButton->setText(QApplication::translate("ConfigWindow", "\344\270\212\344\270\200\346\255\245", 0, QApplication::UnicodeUTF8));
#ifndef QT_NO_WHATSTHIS
        auto_in_cloud_radio->setWhatsThis(QApplication::translate("ConfigWindow", "<html><head/><body><p>color: #FFFFFF;</p><p>font: 14pt &quot;\351\273\221\344\275\223&quot;;</p></body></html>", 0, QApplication::UnicodeUTF8));
#endif // QT_NO_WHATSTHIS
        auto_in_cloud_radio->setText(QApplication::translate("ConfigWindow", "\345\220\257\345\212\250", 0, QApplication::UnicodeUTF8));
        linue1_label_2->setText(QString());
        formatButton->setText(QApplication::translate("ConfigWindow", "\345\274\200\345\247\213\345\256\211\350\243\205", 0, QApplication::UnicodeUTF8));
        label_66->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p><span style=\" font-size:14pt;\">\345\217\202\346\225\260\351\205\215\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        hostname_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\347\273\210\347\253\257\345\220\215\347\247\260\357\274\232</p></body></html>", 0, QApplication::UnicodeUTF8));
        prefix_edit->setText(QApplication::translate("ConfigWindow", "YZY", 0, QApplication::UnicodeUTF8));
        linue1_label_3->setText(QString());
        hostname_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        dhcp_radio->setText(QApplication::translate("ConfigWindow", "\350\207\252\345\212\250\350\216\267\345\217\226IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        dns_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\351\246\226\351\200\211DNS\346\234\215\345\212\241\345\231\250:</p></body></html>", 0, QApplication::UnicodeUTF8));
        dns_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        gateway_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\351\273\230\350\256\244\347\275\221\345\205\263:</p></body></html>", 0, QApplication::UnicodeUTF8));
        gateway_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_26->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p><span style=\" font-size:14pt; color:#ffffff;\">\347\275\221\347\273\234\351\205\215\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        static_radio->setText(QApplication::translate("ConfigWindow", "\344\275\277\347\224\250\344\270\213\346\226\271IP\345\234\260\345\235\200", 0, QApplication::UnicodeUTF8));
        dns2_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\345\244\207\351\200\211DNS\346\234\215\345\212\241\345\231\250:</p></body></html>", 0, QApplication::UnicodeUTF8));
        dns2_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        netmask_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\345\255\220\347\275\221\346\216\251\347\240\201:</p></body></html>", 0, QApplication::UnicodeUTF8));
        netmask_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        lineLabel->setText(QString());
        ip_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>IP\345\234\260\345\235\200:</p></body></html>", 0, QApplication::UnicodeUTF8));
        ip_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        label_100->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p><span style=\" font-size:12pt; color:#ffffff;\">\346\234\215\345\212\241\345\231\250\350\277\236\346\216\245\351\205\215\347\275\256</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        server_ip_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\346\234\215\345\212\241\345\231\250IP\357\274\232</p></body></html>", 0, QApplication::UnicodeUTF8));
        server_ip_result_label->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p>\350\276\223\345\205\245\351\224\231\350\257\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        nextButton->setText(QApplication::translate("ConfigWindow", "\344\270\213\344\270\200\346\255\245", 0, QApplication::UnicodeUTF8));
        lineLabel_2->setText(QString());
        f2Label->setText(QApplication::translate("ConfigWindow", "[F8]\351\207\215\345\220\257", 0, QApplication::UnicodeUTF8));
        arrowLabel->setText(QApplication::translate("ConfigWindow", "<html><head/><body><p><span style=\" color:#ffffff;\">[\342\206\221]/[\342\206\223]/[\342\206\220]/[\342\206\222]\351\200\211\346\213\251</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        enterLabel->setText(QApplication::translate("ConfigWindow", "[Enter]\351\200\211\346\213\251\347\241\256\350\256\244", 0, QApplication::UnicodeUTF8));
        dataLabel->setText(QApplication::translate("ConfigWindow", "  00:00\n"
"2000/01/01", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(ConfigWindow);
    } // retranslateUi

};

namespace Ui {
    class ConfigWindow: public Ui_ConfigWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_CONFIGWINDOW_H
