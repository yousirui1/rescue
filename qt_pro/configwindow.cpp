#include "configwindow.h"
#include "ui_configwindow.h"
#include <QDebug>
#include <QKeyEvent>
#include "global.h"

bool isIPvalidate(const QString &strIP)
{
    QRegExp rx2("\\b(?:(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\.){3}(?:25[0-5]|2[0-4][0-9]|[01]?[0-9][0-9]?)\\b");
    if(!rx2.exactMatch(strIP))
        return false;
    else
        return true;
}

ConfigWindow::ConfigWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::ConfigWindow)
{
    ui->setupUi(this);
    Global *global = Global::getGlobal();
    if(!global->window_flag)
    {
        setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
        this->showFullScreen();
        this->setAutoFillBackground(true);
    }

    date_label = ui->dataLabel;

    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->dhcp_radio->setPalette(pe);
    ui->static_radio->setPalette(pe);
    ui->auto_in_cloud_radio->setPalette(pe);

    /* ip 格式 */
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$"), this);
    ui->ip_edit->setValidator(validator);
    ui->dns_edit->setValidator(validator);
    ui->dns2_edit->setValidator(validator);
    ui->netmask_edit->setValidator(validator);
    ui->gateway_edit->setValidator(validator);
    ui->server_ip_edit->setValidator(validator);

    /* 数字格式 */
    QRegExpValidator *num_validator = new QRegExpValidator(QRegExp("^[1-9][0-9]{1,3}"), this);
    ui->num_edit->setValidator(num_validator);

    /* 终端名称 */
    QRegExpValidator *major_name_validator = new QRegExpValidator(QRegExp("^[a-zA-Z0-9]{1,3}"), this);
    QRegExpValidator *minor_name_validator = new QRegExpValidator(QRegExp("^[a-zA-Z0-9]{1,10}"), this);
    ui->prefix_edit->setValidator(major_name_validator);
    ui->hostname_edit->setValidator(minor_name_validator);

    ui->ip_result_label->hide();
    ui->dns_result_label->hide();
    ui->dns2_result_label->hide();
    ui->netmask_result_label->hide();
    ui->gateway_result_label->hide();
    ui->server_ip_result_label->hide();

    /* 默认dhcp 启动 */

    /* 自动进入云桌面 */
   // ui->auto_in_cloud_radio->setChecked(true);
    group_btn = new QButtonGroup(this);

    group_btn->addButton(ui->dhcp_radio, 0);
    group_btn->addButton(ui->static_radio, 1);
    group_btn->addButton(ui->auto_in_cloud_radio, 2);

    connect(ui->dhcp_radio, SIGNAL(clicked(bool)),
             this, SLOT(slots_dhcpable()));
    connect(ui->static_radio,SIGNAL(clicked(bool)),
             this,SLOT(slots_dhcpable()));
    connect(ui->auto_in_cloud_radio,SIGNAL(clicked(bool)),
             this,SLOT(slots_dhcpable()));



    ui->deskconfig_widget->hide();
    ui->netconfig_widget->show();

    dialog_ui = new DialogWindow();
    dialog_ui->hide();

    /* table index */
    ui->dhcp_radio->setFocus();
    setTabOrder(ui->dhcp_radio, ui->static_radio);
    setTabOrder(ui->static_radio, ui->ip_edit);
    setTabOrder(ui->ip_edit, ui->netmask_edit);
    setTabOrder(ui->netmask_edit,ui->gateway_edit);
    setTabOrder(ui->gateway_edit,ui->dns_edit);
    setTabOrder(ui->dns_edit,ui->dns2_edit);
    setTabOrder(ui->dns2_edit,ui->server_ip_edit);


    setTabOrder(ui->num_edit,ui->prefix_edit);
    setTabOrder(ui->prefix_edit,ui->hostname_edit);
    setTabOrder(ui->hostname_edit,ui->auto_in_cloud_radio);
    setTabOrder(ui->auto_in_cloud_radio,ui->laststepButton);
    setTabOrder(ui->laststepButton,ui->formatButton);

    connect(dialog_ui->install_cacel_btn, SIGNAL(clicked()), this, SLOT(install_cacenl_clicked()));
    connect(dialog_ui->install_confirm_btn, SIGNAL(clicked()), this, SLOT(install_confirm_btn()));

    ui->ip_edit->setText(global->conf.netcard.ip);
    ui->netmask_edit->setText(global->conf.netcard.netmask);
    ui->gateway_edit->setText(global->conf.netcard.gateway);
	ui->dns_edit->setText(global->conf.netcard.dns1);
	ui->dns2_edit->setText(global->conf.netcard.dns2);


    ui->server_ip_edit->setText(global->conf.server.ip);

    ui->hostname_edit->setText((char *)global->conf.terminal.name);
    ui->num_edit->setText(QString("%1").arg(global->conf.terminal.id));

    ui->hostname_result_label->hide();
    ui->num_result_label->hide();


    QLabel *logo_label = new QLabel(this);
    logo_label->setText("");
    logo_label->setStyleSheet("QLabel{"
                               "border-image: url(:/images/logo.png);"
                               "}");
    logo_label->setGeometry(30, 30, 164,47);
    logo_label->show();


    if(global->conf.netcard.is_dhcp)
    {
          ui->dhcp_radio->setChecked(true);
          //ui->static_radio->setChecked(false);
          ui->ip_edit->setEnabled(false);
          ui->dns_edit->setEnabled(false);
          ui->dns2_edit->setEnabled(false);
          ui->netmask_edit->setEnabled(false);
          ui->gateway_edit->setEnabled(false);
    }
    else
    {
         //ui->dhcp_radio->setChecked(false);
         //ui->static_radio->setChecked(true);
         ui->static_radio->setChecked(true);
         ui->ip_edit->setEnabled(true);
         ui->dns_edit->setEnabled(true);
         ui->dns2_edit->setEnabled(true);
         ui->netmask_edit->setEnabled(true);
         ui->gateway_edit->setEnabled(true);
    }
    auto_cloud_flag = 0;
    DEBUG("global->conf.netcard.is_dhcp %d", global->conf.netcard.is_dhcp);
}

ConfigWindow::~ConfigWindow()
{
    delete ui;
}

void ConfigWindow::install_cacenl_clicked()
{
    emit display(0);
}

void ConfigWindow::install_confirm_btn()
{
    emit display(2);

	Global *global = Global::getGlobal();
    char head[HEAD_LEN] = {0};
    global->pipe->send_pipe(head, INSTALL_PIPE, 0);
    global->setDownLoadFlag(1);
}

void ConfigWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F8)
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, REBOOT_PIPE, 0);
    }
#if 0
    if (event->key() == Qt::Key_F1)
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, EXIT_PROGRESS_PIPE, 0);
        qApp->exit();
    }
#endif
    if (event->key() == Qt::Key_Enter)
    {
        DEBUG("Entry");
    }
    if(event->key() == Qt::Key_Up)
    {
         this->focusNextPrevChild(FALSE);//按A时焦点切换至上一部件
    }
    else if(event->key() == Qt::Key_Down)
    {
         this->focusNextPrevChild(TRUE);//按D时焦点切换至下一部件
    }
}

void ConfigWindow::showEvent(QShowEvent *e)
{
    ui->deskconfig_widget->hide();
    ui->netconfig_widget->show();
    Global *global = Global::getGlobal();
    if(global->conf.netcard.is_dhcp)
    {
           ui->dhcp_radio->setChecked(true);
          //ui->static_radio->setChecked(false);
          ui->ip_edit->setEnabled(false);
          ui->dns_edit->setEnabled(false);
          ui->dns2_edit->setEnabled(false);
          ui->netmask_edit->setEnabled(false);
          ui->gateway_edit->setEnabled(false);
    }
    else
    {
         //ui->dhcp_radio->setChecked(false);
         //ui->static_radio->setChecked(true);
         ui->static_radio->setChecked(true);
         ui->ip_edit->setEnabled(true);
         ui->dns_edit->setEnabled(true);
         ui->dns2_edit->setEnabled(true);
         ui->netmask_edit->setEnabled(true);
         ui->gateway_edit->setEnabled(true);
    }
}

void ConfigWindow::on_laststepButton_clicked()
{
    ui->deskconfig_widget->hide();
    ui->netconfig_widget->show();
    Global *global = Global::getGlobal();
    if(global->conf.netcard.is_dhcp)
    {
           ui->dhcp_radio->setChecked(true);
          //ui->static_radio->setChecked(false);
          ui->ip_edit->setEnabled(false);
          ui->dns_edit->setEnabled(false);
          ui->dns2_edit->setEnabled(false);
          ui->netmask_edit->setEnabled(false);
          ui->gateway_edit->setEnabled(false);
    }
    else
    {
         //ui->dhcp_radio->setChecked(false);
         //ui->static_radio->setChecked(true);
         ui->static_radio->setChecked(true);
         ui->ip_edit->setEnabled(true);
         ui->dns_edit->setEnabled(true);
         ui->dns2_edit->setEnabled(true);
         ui->netmask_edit->setEnabled(true);
         ui->gateway_edit->setEnabled(true);
    }

}

void ConfigWindow::on_nextButton_clicked()
{
    int ret = 0;
    Global *global = Global::getGlobal();
    if(!isIPvalidate(ui->ip_edit->text()))
    {
        ui->ip_result_label->show();
        ret = 1;
    }
    else
        ui->ip_result_label->hide();

    if(!isIPvalidate(ui->dns_edit->text()))
    {
        ui->dns_result_label->show();
        ret = 1;
    }
    else
        ui->dns_result_label->hide();

    if(!isIPvalidate(ui->dns2_edit->text()))
    {
        ui->dns2_result_label->show();
        ret = 1;
    }
    else
        ui->dns2_result_label->hide();

    if(!isIPvalidate(ui->netmask_edit->text()))
    {
        ui->netmask_result_label->show();
        ret = 1;
    }
    else
        ui->netmask_result_label->hide();

    if(!isIPvalidate(ui->gateway_edit->text()))
    {
        ui->gateway_result_label->show();
        ret = 1;
    }
    else
        ui->gateway_result_label->hide();

    if(global->conf.netcard.is_dhcp)
    {
        ui->ip_result_label->hide();
        ui->gateway_result_label->hide();
        ui->netmask_result_label->hide();
        ui->dns2_result_label->hide();
        ui->dns_result_label->hide();
    }

    if(!isIPvalidate(ui->server_ip_edit->text()))
    {
        ui->server_ip_result_label->show();
        return;
    }
    else
        ui->server_ip_result_label->hide();



    if(!ret || global->conf.netcard.is_dhcp)
    {
        ui->netconfig_widget->hide();
        ui->deskconfig_widget->show();
        ui->num_edit->setFocus();
        if(global->conf.terminal.auto_desktop)
        {
             ui->auto_in_cloud_radio->setChecked(true);
             //auto_cloud_flag = true;
        }
        else
        {
            ui->auto_in_cloud_radio->setChecked(false);
            //auto_cloud_flag = false;
        }
    }
}

/* 保存配置并弹出dialog 确认格式化磁盘 */
void ConfigWindow::on_formatButton_clicked()
{
    if(ui->num_edit->text().isEmpty())
    {
         ui->num_result_label->show();
         return;
    }

    if(ui->hostname_edit->text().isEmpty())
    {
        ui->hostname_result_label->show();
        return;
    }


    if(ui->prefix_edit->text().isEmpty())
    {
         ui->hostname_result_label->show();
         return;
    }

    QByteArray ba_ip = ui->ip_edit->text().toLatin1();
    QByteArray ba_gateway = ui->gateway_edit->text().toLatin1();
    QByteArray ba_netmask = ui->netmask_edit->text().toLatin1();
    QByteArray ba_nds1 = ui->dns_edit->text().toLatin1();
    QByteArray ba_nds2 = ui->dns2_edit->text().toLatin1();
    QByteArray ba_server_ip = ui->server_ip_edit->text().toLatin1();
    QByteArray ba_terminal_name = ui->hostname_edit->text().toLatin1();
    //QByteArray ba_terminal_name = ui->hostname_edit->text().toLatin1();

    Global *global = Global::getGlobal();
    //
    struct config *conf = &(global->conf);


    memset(conf->netcard.ip, 0, sizeof(conf->netcard.ip));
    memcpy(conf->netcard.ip, ba_ip.data(), ba_ip.length());

    memset(conf->netcard.gateway, 0, sizeof(conf->netcard.gateway));
    memcpy(conf->netcard.gateway, ba_gateway.data(), ba_gateway.length());

    memset(conf->netcard.netmask, 0, sizeof(conf->netcard.netmask));
    memcpy(conf->netcard.netmask, ba_netmask.data(), ba_netmask.length());

    memset(conf->netcard.dns1, 0, sizeof(conf->netcard.dns1));
    memcpy(conf->netcard.dns1, ba_nds1.data(), ba_nds1.length());

    memset(conf->netcard.dns2, 0, sizeof(conf->netcard.dns2));
    memcpy(conf->netcard.dns2, ba_nds2.data(), ba_nds2.length());

	memset(conf->server.ip, 0, sizeof(conf->server.ip));
    memcpy(conf->server.ip, ba_server_ip.data(), ba_server_ip.length());

	memset(conf->terminal.name, 0, sizeof(conf->terminal.name));
    memcpy(conf->terminal.name, ba_terminal_name.data(), ba_terminal_name.length());

    global->conf.terminal.id = ui->num_edit->text().toInt();

    DEBUG("----------dhcp_flag %d---------", global->conf.netcard.is_dhcp);
    DEBUG("conf->netcard.ip %s", conf->netcard.ip);
    DEBUG("conf->netcard.gateway %s", conf->netcard.ip);
    DEBUG("conf->netcard.netmask %s", conf->netcard.ip);
    DEBUG("conf->netcard.dns1 %s", conf->netcard.ip);
    DEBUG("conf->netcard.dns2 %s", conf->netcard.ip);
    DEBUG("conf->server.ip %s", conf->netcard.ip);
    DEBUG("global->conf.terminal.id %d", global->conf.terminal.id);
    DEBUG("global->conf.terminal.auto_desktop %d", global->conf.terminal.auto_desktop);

    char *buf = (char *)malloc(sizeof(struct config) + HEAD_LEN +1);

    if(!buf)
          return;
    char *data = &buf[HEAD_LEN];
    memcpy(data, conf, sizeof(struct config));

    global->pipe->send_pipe(buf, UPDATE_CONFIG_PIPE, sizeof(struct config));
    free(buf);
    dialog_ui->setType(DIALOG_INSTALL);
    dialog_ui->show();

}

void ConfigWindow::slots_dhcpable()
{
    Global *global = Global::getGlobal();
        switch(group_btn->checkedId())
        {
           case 0:          //dhcp
              DEBUG("set network dhcp ip model");
              ui->ip_edit->setEnabled(false);
              ui->dns_edit->setEnabled(false);
              ui->dns2_edit->setEnabled(false);
              ui->netmask_edit->setEnabled(false);
              ui->gateway_edit->setEnabled(false);
              //ui->server_ip_edit->setEnabled(false);
        	  ui->ip_edit->setText("");
        	  ui->dns_edit->setText("");
        	  ui->dns2_edit->setText("");
        	  ui->netmask_edit->setText("");
        	  ui->gateway_edit->setText("");

              global->conf.netcard.is_dhcp = 1;
              break;
           case 1:          //static
              DEBUG("set network static ip model");
              ui->ip_edit->setEnabled(true);
              ui->dns_edit->setEnabled(true);
              ui->dns2_edit->setEnabled(true);
              ui->netmask_edit->setEnabled(true);
              ui->gateway_edit->setEnabled(true);
              //ui->server_ip_edit->setEnabled(true);

              global->conf.netcard.is_dhcp = 0;
              break;
           case 2:
                global->conf.terminal.auto_desktop = !global->conf.terminal.auto_desktop;
                DEBUG("global->conf.terminal.auto_desktop %d", global->conf.terminal.auto_desktop);
                ui->dhcp_radio->setChecked(!global->conf.terminal.auto_desktop);
                return;
        }
}

void ConfigWindow::on_num_edit_textChanged(const QString &arg1)
{
    ui->hostname_edit->setText(arg1);
}
