/********************************************************************************
** Form generated from reading ui file 'uploadwindow.ui'
**
** Created: Sun Sep 27 20:19:43 2020
**      by: Qt User Interface Compiler version 4.5.2
**
** WARNING! All changes made in this file will be lost when recompiling ui file!
********************************************************************************/

#ifndef UI_UPLOADWINDOW_H
#define UI_UPLOADWINDOW_H

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

QT_BEGIN_NAMESPACE

class Ui_Uploadwindow
{
public:
    QVBoxLayout *verticalLayout;
    QLabel *lineLabel;
    QLabel *backgroudWidget;
    QHBoxLayout *horizontalLayout;
    QSpacerItem *hright_space;
    QWidget *upload_widget;
    QWidget *install_widget;
    QLabel *i_progress_t_label;
    QLabel *i_title_label;
    QWidget *i_rangbar;
    QLabel *label;
    QLabel *i_remain_time_label;
    QLabel *i_status_label;
    QLabel *i_remain_time_t_label;
    QLabel *i_progress_label;
    QLabel *i_termal_info_label;
    QLabel *i_num_t_label;
    QLabel *i_disk_info_t_label;
    QLabel *i_num_label;
    QLabel *i_memory_t_label;
    QLabel *i_cpu_t_label;
    QLabel *i_disk_info_label;
    QLabel *i_memory_label;
    QLabel *i_cpu_label;
    QLabel *i_ip_t_label;
    QLabel *i_mac_label;
    QLabel *i_mac_t_label;
    QLabel *i_ip_label;
    QLabel *i_netcard_label;
    QLabel *i_netcard_t_label;
    QLabel *i_download_rate_t_label;
    QLabel *i_download_rate_label;
    QLabel *i_upload_rate_label;
    QLabel *i_upload_rate_t_label;
    QWidget *up_widget;
    QLabel *desLabel_35;
    QLabel *desLabel_36;
    QLabel *desLabel_37;
    QLabel *desLabel_38;
    QLabel *desLabel_39;
    QLabel *desLabel_40;
    QLabel *desLabel_41;
    QLabel *desLabel_42;
    QWidget *widget_5;
    QLabel *desLabel_43;
    QLabel *desLabel_44;
    QLabel *desLabel_45;
    QLabel *desLabel_46;
    QLabel *desLabel_47;
    QLabel *desLabel_48;
    QLabel *desLabel_49;
    QLabel *desLabel_50;
    QLabel *desLabel_51;
    QLabel *desLabel_52;
    QLabel *desLabel_53;
    QLabel *desLabel_54;
    QLabel *desLabel_55;
    QLabel *desLabel_56;
    QLabel *desLabel_57;
    QLabel *desLabel_58;
    QLabel *desLabel_59;
    QLabel *desLabel_60;
    QLabel *desLabel_61;
    QLabel *desLabel_62;
    QLabel *desLabel_63;
    QLabel *desLabel_64;
    QLabel *desLabel_65;
    QWidget *rangbar_2;
    QLabel *desLabel_66;
    QLabel *desLabel_67;
    QSpacerItem *hleft_space;
    QLabel *bottomWidget;
    QHBoxLayout *horizontalLayout_2;
    QLabel *f2Label;
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

    void setupUi(QWidget *Uploadwindow)
    {
        if (Uploadwindow->objectName().isEmpty())
            Uploadwindow->setObjectName(QString::fromUtf8("Uploadwindow"));
        Uploadwindow->resize(1600, 900);
        Uploadwindow->setMinimumSize(QSize(800, 600));
        Uploadwindow->setMaximumSize(QSize(1920, 1080));
        Uploadwindow->setStyleSheet(QString::fromUtf8(""));
        verticalLayout = new QVBoxLayout(Uploadwindow);
        verticalLayout->setSpacing(0);
        verticalLayout->setMargin(11);
        verticalLayout->setObjectName(QString::fromUtf8("verticalLayout"));
        verticalLayout->setContentsMargins(0, 0, 0, 0);
        lineLabel = new QLabel(Uploadwindow);
        lineLabel->setObjectName(QString::fromUtf8("lineLabel"));
        lineLabel->setMinimumSize(QSize(800, 1));
        lineLabel->setMaximumSize(QSize(1920, 1));
        lineLabel->setStyleSheet(QString::fromUtf8("background-image: url(:/images/line.png);"));

        verticalLayout->addWidget(lineLabel);

        backgroudWidget = new QLabel(Uploadwindow);
        backgroudWidget->setObjectName(QString::fromUtf8("backgroudWidget"));
        backgroudWidget->setMinimumSize(QSize(800, 600));
        backgroudWidget->setMaximumSize(QSize(1920, 1080));
        backgroudWidget->setStyleSheet(QString::fromUtf8("QWidget#backgroudWidget{border-image: url(:/images/backgroud.png);}"));
        horizontalLayout = new QHBoxLayout(backgroudWidget);
        horizontalLayout->setSpacing(6);
        horizontalLayout->setMargin(11);
        horizontalLayout->setObjectName(QString::fromUtf8("horizontalLayout"));
        hright_space = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(hright_space);

        upload_widget = new QWidget(backgroudWidget);
        upload_widget->setObjectName(QString::fromUtf8("upload_widget"));
        upload_widget->setMinimumSize(QSize(1052, 568));
        upload_widget->setMaximumSize(QSize(1116, 900));
        install_widget = new QWidget(upload_widget);
        install_widget->setObjectName(QString::fromUtf8("install_widget"));
        install_widget->setGeometry(QRect(20, 90, 1052, 601));
        install_widget->setMinimumSize(QSize(1052, 568));
        install_widget->setMaximumSize(QSize(1116, 900));
        install_widget->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        i_progress_t_label = new QLabel(install_widget);
        i_progress_t_label->setObjectName(QString::fromUtf8("i_progress_t_label"));
        i_progress_t_label->setGeometry(QRect(120, 270, 111, 21));
        i_progress_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_progress_t_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        i_title_label = new QLabel(install_widget);
        i_title_label->setObjectName(QString::fromUtf8("i_title_label"));
        i_title_label->setGeometry(QRect(430, 0, 100, 31));
        i_title_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        i_title_label->setAlignment(Qt::AlignCenter);
        i_rangbar = new QWidget(install_widget);
        i_rangbar->setObjectName(QString::fromUtf8("i_rangbar"));
        i_rangbar->setGeometry(QRect(400, 50, 150, 150));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(i_rangbar->sizePolicy().hasHeightForWidth());
        i_rangbar->setSizePolicy(sizePolicy);
        i_rangbar->setMinimumSize(QSize(150, 150));
        i_rangbar->setMaximumSize(QSize(150, 150));
        i_rangbar->setStyleSheet(QString::fromUtf8(""));
        label = new QLabel(i_rangbar);
        label->setObjectName(QString::fromUtf8("label"));
        label->setGeometry(QRect(0, 0, 150, 150));
        label->setMinimumSize(QSize(150, 150));
        label->setMaximumSize(QSize(150, 150));
        label->setStyleSheet(QString::fromUtf8("border-image: url(:/images/rircle.png);\n"
""));
        i_remain_time_label = new QLabel(install_widget);
        i_remain_time_label->setObjectName(QString::fromUtf8("i_remain_time_label"));
        i_remain_time_label->setGeometry(QRect(420, 270, 111, 21));
        i_remain_time_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_status_label = new QLabel(install_widget);
        i_status_label->setObjectName(QString::fromUtf8("i_status_label"));
        i_status_label->setGeometry(QRect(218, 220, 521, 31));
        i_status_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_status_label->setAlignment(Qt::AlignCenter);
        i_remain_time_t_label = new QLabel(install_widget);
        i_remain_time_t_label->setObjectName(QString::fromUtf8("i_remain_time_t_label"));
        i_remain_time_t_label->setGeometry(QRect(340, 270, 71, 21));
        i_remain_time_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_progress_label = new QLabel(install_widget);
        i_progress_label->setObjectName(QString::fromUtf8("i_progress_label"));
        i_progress_label->setGeometry(QRect(250, 270, 61, 21));
        i_progress_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_termal_info_label = new QLabel(install_widget);
        i_termal_info_label->setObjectName(QString::fromUtf8("i_termal_info_label"));
        i_termal_info_label->setGeometry(QRect(110, 320, 81, 31));
        i_termal_info_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        i_num_t_label = new QLabel(install_widget);
        i_num_t_label->setObjectName(QString::fromUtf8("i_num_t_label"));
        i_num_t_label->setGeometry(QRect(168, 380, 61, 21));
        i_num_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_num_t_label->setAlignment(Qt::AlignRight|Qt::AlignTrailing|Qt::AlignVCenter);
        i_disk_info_t_label = new QLabel(install_widget);
        i_disk_info_t_label->setObjectName(QString::fromUtf8("i_disk_info_t_label"));
        i_disk_info_t_label->setGeometry(QRect(130, 420, 91, 21));
        i_disk_info_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_num_label = new QLabel(install_widget);
        i_num_label->setObjectName(QString::fromUtf8("i_num_label"));
        i_num_label->setGeometry(QRect(270, 380, 151, 21));
        i_num_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_memory_t_label = new QLabel(install_widget);
        i_memory_t_label->setObjectName(QString::fromUtf8("i_memory_t_label"));
        i_memory_t_label->setGeometry(QRect(160, 460, 61, 21));
        i_memory_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_cpu_t_label = new QLabel(install_widget);
        i_cpu_t_label->setObjectName(QString::fromUtf8("i_cpu_t_label"));
        i_cpu_t_label->setGeometry(QRect(160, 500, 61, 21));
        i_cpu_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_disk_info_label = new QLabel(install_widget);
        i_disk_info_label->setObjectName(QString::fromUtf8("i_disk_info_label"));
        i_disk_info_label->setGeometry(QRect(270, 420, 151, 21));
        i_disk_info_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_memory_label = new QLabel(install_widget);
        i_memory_label->setObjectName(QString::fromUtf8("i_memory_label"));
        i_memory_label->setGeometry(QRect(270, 460, 151, 21));
        i_memory_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_cpu_label = new QLabel(install_widget);
        i_cpu_label->setObjectName(QString::fromUtf8("i_cpu_label"));
        i_cpu_label->setGeometry(QRect(270, 500, 351, 41));
        i_cpu_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_cpu_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        i_cpu_label->setWordWrap(false);
        i_ip_t_label = new QLabel(install_widget);
        i_ip_t_label->setObjectName(QString::fromUtf8("i_ip_t_label"));
        i_ip_t_label->setGeometry(QRect(598, 380, 61, 21));
        i_ip_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_mac_label = new QLabel(install_widget);
        i_mac_label->setObjectName(QString::fromUtf8("i_mac_label"));
        i_mac_label->setGeometry(QRect(708, 420, 251, 21));
        i_mac_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_mac_t_label = new QLabel(install_widget);
        i_mac_t_label->setObjectName(QString::fromUtf8("i_mac_t_label"));
        i_mac_t_label->setGeometry(QRect(578, 420, 81, 21));
        i_mac_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_ip_label = new QLabel(install_widget);
        i_ip_label->setObjectName(QString::fromUtf8("i_ip_label"));
        i_ip_label->setGeometry(QRect(708, 380, 251, 21));
        i_ip_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_netcard_label = new QLabel(install_widget);
        i_netcard_label->setObjectName(QString::fromUtf8("i_netcard_label"));
        i_netcard_label->setGeometry(QRect(708, 460, 321, 41));
        i_netcard_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_netcard_label->setAlignment(Qt::AlignLeading|Qt::AlignLeft|Qt::AlignTop);
        i_netcard_t_label = new QLabel(install_widget);
        i_netcard_t_label->setObjectName(QString::fromUtf8("i_netcard_t_label"));
        i_netcard_t_label->setGeometry(QRect(598, 460, 61, 21));
        i_netcard_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_download_rate_t_label = new QLabel(install_widget);
        i_download_rate_t_label->setObjectName(QString::fromUtf8("i_download_rate_t_label"));
        i_download_rate_t_label->setGeometry(QRect(510, 270, 71, 21));
        i_download_rate_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_download_rate_label = new QLabel(install_widget);
        i_download_rate_label->setObjectName(QString::fromUtf8("i_download_rate_label"));
        i_download_rate_label->setGeometry(QRect(590, 270, 111, 21));
        i_download_rate_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_upload_rate_label = new QLabel(install_widget);
        i_upload_rate_label->setObjectName(QString::fromUtf8("i_upload_rate_label"));
        i_upload_rate_label->setGeometry(QRect(770, 270, 111, 21));
        i_upload_rate_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        i_upload_rate_t_label = new QLabel(install_widget);
        i_upload_rate_t_label->setObjectName(QString::fromUtf8("i_upload_rate_t_label"));
        i_upload_rate_t_label->setGeometry(QRect(680, 270, 71, 21));
        i_upload_rate_t_label->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        up_widget = new QWidget(upload_widget);
        up_widget->setObjectName(QString::fromUtf8("up_widget"));
        up_widget->setGeometry(QRect(1000, 1000, 1052, 600));
        up_widget->setMinimumSize(QSize(1052, 568));
        up_widget->setMaximumSize(QSize(1116, 900));
        desLabel_35 = new QLabel(up_widget);
        desLabel_35->setObjectName(QString::fromUtf8("desLabel_35"));
        desLabel_35->setGeometry(QRect(880, 280, 101, 21));
        desLabel_35->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_36 = new QLabel(up_widget);
        desLabel_36->setObjectName(QString::fromUtf8("desLabel_36"));
        desLabel_36->setGeometry(QRect(240, 0, 311, 31));
        desLabel_36->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        desLabel_37 = new QLabel(up_widget);
        desLabel_37->setObjectName(QString::fromUtf8("desLabel_37"));
        desLabel_37->setGeometry(QRect(380, 280, 91, 21));
        desLabel_37->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_38 = new QLabel(up_widget);
        desLabel_38->setObjectName(QString::fromUtf8("desLabel_38"));
        desLabel_38->setGeometry(QRect(710, 280, 81, 21));
        desLabel_38->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_39 = new QLabel(up_widget);
        desLabel_39->setObjectName(QString::fromUtf8("desLabel_39"));
        desLabel_39->setGeometry(QRect(100, 280, 81, 21));
        desLabel_39->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_40 = new QLabel(up_widget);
        desLabel_40->setObjectName(QString::fromUtf8("desLabel_40"));
        desLabel_40->setGeometry(QRect(210, 280, 71, 21));
        desLabel_40->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_41 = new QLabel(up_widget);
        desLabel_41->setObjectName(QString::fromUtf8("desLabel_41"));
        desLabel_41->setGeometry(QRect(470, 280, 61, 21));
        desLabel_41->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_42 = new QLabel(up_widget);
        desLabel_42->setObjectName(QString::fromUtf8("desLabel_42"));
        desLabel_42->setGeometry(QRect(250, 220, 311, 31));
        desLabel_42->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        widget_5 = new QWidget(up_widget);
        widget_5->setObjectName(QString::fromUtf8("widget_5"));
        widget_5->setGeometry(QRect(0, 330, 761, 251));
        desLabel_43 = new QLabel(widget_5);
        desLabel_43->setObjectName(QString::fromUtf8("desLabel_43"));
        desLabel_43->setGeometry(QRect(40, 50, 61, 21));
        desLabel_43->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_44 = new QLabel(widget_5);
        desLabel_44->setObjectName(QString::fromUtf8("desLabel_44"));
        desLabel_44->setGeometry(QRect(140, 50, 151, 21));
        desLabel_44->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_45 = new QLabel(widget_5);
        desLabel_45->setObjectName(QString::fromUtf8("desLabel_45"));
        desLabel_45->setGeometry(QRect(40, 90, 61, 21));
        desLabel_45->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_46 = new QLabel(widget_5);
        desLabel_46->setObjectName(QString::fromUtf8("desLabel_46"));
        desLabel_46->setGeometry(QRect(140, 90, 171, 21));
        desLabel_46->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_47 = new QLabel(widget_5);
        desLabel_47->setObjectName(QString::fromUtf8("desLabel_47"));
        desLabel_47->setGeometry(QRect(20, 130, 81, 21));
        desLabel_47->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_48 = new QLabel(widget_5);
        desLabel_48->setObjectName(QString::fromUtf8("desLabel_48"));
        desLabel_48->setGeometry(QRect(140, 130, 171, 21));
        desLabel_48->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_49 = new QLabel(widget_5);
        desLabel_49->setObjectName(QString::fromUtf8("desLabel_49"));
        desLabel_49->setGeometry(QRect(10, 170, 91, 21));
        desLabel_49->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_50 = new QLabel(widget_5);
        desLabel_50->setObjectName(QString::fromUtf8("desLabel_50"));
        desLabel_50->setGeometry(QRect(140, 170, 171, 21));
        desLabel_50->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_51 = new QLabel(widget_5);
        desLabel_51->setObjectName(QString::fromUtf8("desLabel_51"));
        desLabel_51->setGeometry(QRect(0, 210, 101, 21));
        desLabel_51->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_52 = new QLabel(widget_5);
        desLabel_52->setObjectName(QString::fromUtf8("desLabel_52"));
        desLabel_52->setGeometry(QRect(140, 210, 171, 21));
        desLabel_52->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_53 = new QLabel(widget_5);
        desLabel_53->setObjectName(QString::fromUtf8("desLabel_53"));
        desLabel_53->setGeometry(QRect(490, 170, 91, 21));
        desLabel_53->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_54 = new QLabel(widget_5);
        desLabel_54->setObjectName(QString::fromUtf8("desLabel_54"));
        desLabel_54->setGeometry(QRect(480, 210, 101, 21));
        desLabel_54->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_55 = new QLabel(widget_5);
        desLabel_55->setObjectName(QString::fromUtf8("desLabel_55"));
        desLabel_55->setGeometry(QRect(590, 50, 171, 21));
        desLabel_55->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_56 = new QLabel(widget_5);
        desLabel_56->setObjectName(QString::fromUtf8("desLabel_56"));
        desLabel_56->setGeometry(QRect(590, 170, 171, 21));
        desLabel_56->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_57 = new QLabel(widget_5);
        desLabel_57->setObjectName(QString::fromUtf8("desLabel_57"));
        desLabel_57->setGeometry(QRect(590, 130, 171, 21));
        desLabel_57->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_58 = new QLabel(widget_5);
        desLabel_58->setObjectName(QString::fromUtf8("desLabel_58"));
        desLabel_58->setGeometry(QRect(510, 90, 61, 21));
        desLabel_58->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_59 = new QLabel(widget_5);
        desLabel_59->setObjectName(QString::fromUtf8("desLabel_59"));
        desLabel_59->setGeometry(QRect(590, 210, 171, 21));
        desLabel_59->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_60 = new QLabel(widget_5);
        desLabel_60->setObjectName(QString::fromUtf8("desLabel_60"));
        desLabel_60->setGeometry(QRect(590, 90, 171, 21));
        desLabel_60->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_61 = new QLabel(widget_5);
        desLabel_61->setObjectName(QString::fromUtf8("desLabel_61"));
        desLabel_61->setGeometry(QRect(510, 50, 61, 21));
        desLabel_61->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_62 = new QLabel(widget_5);
        desLabel_62->setObjectName(QString::fromUtf8("desLabel_62"));
        desLabel_62->setGeometry(QRect(500, 130, 81, 21));
        desLabel_62->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_63 = new QLabel(widget_5);
        desLabel_63->setObjectName(QString::fromUtf8("desLabel_63"));
        desLabel_63->setGeometry(QRect(10, 10, 81, 31));
        desLabel_63->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 14pt \"\351\273\221\344\275\223\";"));
        desLabel_64 = new QLabel(up_widget);
        desLabel_64->setObjectName(QString::fromUtf8("desLabel_64"));
        desLabel_64->setGeometry(QRect(630, 280, 61, 21));
        desLabel_64->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_65 = new QLabel(up_widget);
        desLabel_65->setObjectName(QString::fromUtf8("desLabel_65"));
        desLabel_65->setGeometry(QRect(560, 280, 81, 21));
        desLabel_65->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        rangbar_2 = new QWidget(up_widget);
        rangbar_2->setObjectName(QString::fromUtf8("rangbar_2"));
        rangbar_2->setGeometry(QRect(300, 60, 211, 131));
        desLabel_66 = new QLabel(up_widget);
        desLabel_66->setObjectName(QString::fromUtf8("desLabel_66"));
        desLabel_66->setGeometry(QRect(290, 280, 61, 21));
        desLabel_66->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));
        desLabel_67 = new QLabel(up_widget);
        desLabel_67->setObjectName(QString::fromUtf8("desLabel_67"));
        desLabel_67->setGeometry(QRect(800, 280, 61, 21));
        desLabel_67->setStyleSheet(QString::fromUtf8("color: #FFFFFF;\n"
"font: 12pt \"\351\273\221\344\275\223\";"));

        horizontalLayout->addWidget(upload_widget);

        hleft_space = new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(hleft_space);

        horizontalLayout->setStretch(0, 1);
        horizontalLayout->setStretch(1, 2);
        horizontalLayout->setStretch(2, 1);

        verticalLayout->addWidget(backgroudWidget);

        bottomWidget = new QLabel(Uploadwindow);
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
        ip_label->setMinimumSize(QSize(150, 42));
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


        retranslateUi(Uploadwindow);

        QMetaObject::connectSlotsByName(Uploadwindow);
    } // setupUi

    void retranslateUi(QWidget *Uploadwindow)
    {
        Uploadwindow->setWindowTitle(QApplication::translate("Uploadwindow", "MainWindow", 0, QApplication::UnicodeUTF8));
        lineLabel->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><br/></p></body></html>", 0, QApplication::UnicodeUTF8));
        i_progress_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\344\270\213\350\275\275\350\277\233\345\272\246:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_title_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\">\351\203\250\347\275\262\346\250\241\345\274\217</p></body></html>", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        i_remain_time_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>-\347\247\222</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_status_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\">\346\255\243\345\234\250\345\256\211\350\243\205:\345\256\242\346\210\267\347\253\257</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_remain_time_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\345\211\251\344\275\231\346\227\266\351\227\264:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_progress_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>10%</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_termal_info_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">\347\273\210\347\253\257\344\277\241\346\201\257</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        i_num_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\272\217\345\217\267\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        i_disk_info_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\347\241\254\347\233\230\345\211\251\344\275\231:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_num_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>Desktop-MC08</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_memory_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\345\206\205\345\255\230:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_cpu_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">CPU:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_disk_info_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>380GB\357\274\210400GB)</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_memory_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>1928MB</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_cpu_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>Intel(R)1111111111111111111111111111111111111111111111111111111111</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_ip_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">IP\345\234\260\345\235\200:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_mac_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>00-0C-29-29-D6-68</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_mac_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">MAC\345\234\260\345\235\200:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_ip_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>192.168.125.125\357\274\210255.255.255.0\357\274\211</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_netcard_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>Intel</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_netcard_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\347\275\221\345\215\241:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_download_rate_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\344\270\213\350\241\214:</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_download_rate_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>0kb/s</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_upload_rate_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p>0kb/s</p></body></html>", 0, QApplication::UnicodeUTF8));
        i_upload_rate_t_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\344\270\212\344\274\240:</p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_35->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_36->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\">\347\273\264\346\212\244\346\250\241\345\274\217</p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_37->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_38->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_39->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\">\344\270\213\350\275\275\350\277\233\345\272\246:</p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_40->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_41->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_42->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">window_1964_\346\225\231\345\255\246\344\270\213\350\275\275\345\256\214\346\210\220\357\274\201</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_43->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\272\217\345\217\267\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_44->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">01</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_45->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\347\273\210\347\253\257\345\220\215\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_46->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_47->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\347\243\201\347\233\230\345\211\251\344\275\231:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_48->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_49->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\267\262\345\210\206\346\241\214\351\235\242\346\225\260:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_50->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_51->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\267\262\344\270\213\346\241\214\351\235\242\346\225\260:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_52->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_53->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\267\262\345\210\206\346\241\214\351\235\242\346\225\260:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_54->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\267\262\344\270\213\346\241\214\351\235\242\346\225\260:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_55->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">01</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_56->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_57->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_58->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\347\273\210\347\253\257\345\220\215:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_59->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_60->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_61->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\272\217\345\217\267\357\274\232</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_62->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\347\243\201\347\233\230\345\211\251\344\275\231:</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_63->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\"><span style=\" color:#ffffff;\">\347\273\210\347\253\257\344\277\241\346\201\257</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_64->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_65->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_66->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        desLabel_67->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"right\"><span style=\" color:#ffffff;\">\345\272\217\345\217\267</span></p></body></html>", 0, QApplication::UnicodeUTF8));
        f2Label->setText(QApplication::translate("Uploadwindow", "[F2]\347\273\210\347\253\257\351\205\215\347\275\256", 0, QApplication::UnicodeUTF8));
        netpoint_label->setText(QString());
        netstate_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\">\345\234\250\347\272\277</p></body></html>", 0, QApplication::UnicodeUTF8));
        neticon_label->setText(QString());
        ip_label->setText(QApplication::translate("Uploadwindow", "<html><head/><body><p align=\"center\">192.168.255.255</p></body></html>", 0, QApplication::UnicodeUTF8));
        dataLabel->setText(QApplication::translate("Uploadwindow", "  00:00\n"
"2000/01/01", 0, QApplication::UnicodeUTF8));
        Q_UNUSED(Uploadwindow);
    } // retranslateUi

};

namespace Ui {
    class Uploadwindow: public Ui_Uploadwindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_UPLOADWINDOW_H
