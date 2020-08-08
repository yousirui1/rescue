#include "dialogwindow.h"
#include "global.h"
#include "ui_dialogwindow.h"

bool isIPvalidate(const QString &strIP);


DialogWindow::DialogWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogWindow)
{
    ui->setupUi(this);

    Global *global = Global::getGlobal();
    if(!global->window_flag)
    {
        setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
        this->showFullScreen();
        this->setAutoFillBackground(true);
    }

    install_cacel_btn = ui->install_cacel_btn;
    install_confirm_btn = ui->install_confirm_btn;

    /* ip 格式 */
    QRegExpValidator *validator = new QRegExpValidator(QRegExp("^((2[0-4]\\d|25[0-5]|[01]?\\d\\d?)\\.){3}(2[0-4]\\d|25[0-5]|[01]?\\d\\d?)$"), this);
    ui->ip_edit->setValidator(validator);
    ui->dns_edit->setValidator(validator);
    ui->dns2_edit->setValidator(validator);
    ui->netmask_edit->setValidator(validator);
    ui->gateway_edit->setValidator(validator);
    ui->server_ip_edit->setValidator(validator);

    ui->ip_result_label->hide();
    ui->dns_result_label->hide();
    ui->dns2_result_label->hide();
    ui->netmask_result_label->hide();
    ui->gateway_result_label->hide();
    ui->server_ip_result_label->hide();

    /* 默认dhcp 启动 */
    group_btn = new QButtonGroup(this);

    group_btn->addButton(ui->dhcp_radio, 0);
    group_btn->addButton(ui->static_radio, 1);

    connect(ui->dhcp_radio, SIGNAL(clicked(bool)),
             this, SLOT(slots_dhcpable()));
    connect(ui->static_radio,SIGNAL(clicked(bool)),
             this,SLOT(slots_dhcpable()));


    ui->ip_edit->setEnabled(false);
    ui->dns_edit->setEnabled(false);
    ui->dns2_edit->setEnabled(false);
    ui->netmask_edit->setEnabled(false);
    ui->gateway_edit->setEnabled(false);
    //ui->server_ip_edit->setEnabled(false);
    //dhcp_flag = 1;
    //auto_cloud_flag = true;

    //ui->deskconfig_widget->hide();
    //ui->netconfig_widget->show();

    ui->ip_edit->setText(global->conf.netcard.ip);
    ui->netmask_edit->setText(global->conf.netcard.netmask);
    ui->gateway_edit->setText(global->conf.netcard.gateway);

    /* table index */
    ui->dhcp_radio->setFocus();
    setTabOrder(ui->dhcp_radio, ui->static_radio);
    setTabOrder(ui->static_radio, ui->ip_edit);
    setTabOrder(ui->ip_edit, ui->netmask_edit);
    setTabOrder(ui->netmask_edit,ui->gateway_edit);
    setTabOrder(ui->gateway_edit,ui->dns_edit);
    setTabOrder(ui->dns_edit,ui->dns2_edit);
    setTabOrder(ui->dns2_edit,ui->server_ip_edit);

    QLabel *logo_label = new QLabel(this);
    logo_label->setText("");
    logo_label->setStyleSheet("QLabel{"
                               "border-image: url(:/images/logo.png);"
                               "}");
    logo_label->setGeometry(30, 30, 164,47);
    logo_label->show();


    QPalette pe;
    pe.setColor(QPalette::WindowText,Qt::white);
    ui->dhcp_radio->setPalette(pe);
    ui->static_radio->setPalette(pe);
}


void DialogWindow::showEvent(QShowEvent *e)
{
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
          dhcp_flag = 1;
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
         dhcp_flag = 0;
    }
    ui->ip_edit->setText(global->conf.netcard.ip);
    ui->netmask_edit->setText(global->conf.netcard.netmask);
    ui->gateway_edit->setText(global->conf.netcard.gateway);
}

DialogWindow::~DialogWindow()
{
    delete ui;
}

void DialogWindow::hideAll()
{
    ui->net_dialog->hide();
    ui->install_dialog->hide();
    ui->error_dialog->hide();
}

void DialogWindow::setType(int type)
{
    hideAll();
    switch(type)
    {
        case DIALOG_NET:
             ui->net_dialog->show();
            break;
        case DIALOG_INSTALL:
            ui->install_dialog->show();
            break;
        case DIALOG_UNINSTALL:
            ui->install_dialog->show();
            break;
        case DIALOG_ERROR:
            ui->error_dialog->show();
             break;
    }
}

void DialogWindow::setErrorText(const char *msg)
{
    this->show();
    hideAll();
    ui->error_dialog->show();
    ui->error_msg_label->setText(msg);
}

void DialogWindow::on_cacel_btn_clicked()
{
    this->hide();
}

void DialogWindow::on_confirm_btn_clicked()
{
    int ret = 0;
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

    if(dhcp_flag)
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


    if(ret && !dhcp_flag)
    {
        return;
    }


    Global *global = Global::getGlobal();

    QByteArray ba_ip = ui->ip_edit->text().toLatin1();
    QByteArray ba_gateway = ui->gateway_edit->text().toLatin1();
    QByteArray ba_netmask = ui->netmask_edit->text().toLatin1();
    QByteArray ba_nds1 = ui->dns_edit->text().toLatin1();
    QByteArray ba_nds2 = ui->dns2_edit->text().toLatin1();
    QByteArray ba_server_ip = ui->server_ip_edit->text().toLatin1();

    //
    struct config *conf = &(global->conf);

    conf->netcard.is_dhcp = dhcp_flag;
    memcpy(conf->netcard.ip, ba_ip.data(), ba_ip.length());
    memcpy(conf->netcard.gateway, ba_gateway.data(), ba_gateway.length());
    //memcpy(conf->netcard.boardcast_addr, ui->->text().data(), ui->ip_edit->text().length());
    memcpy(conf->netcard.netmask, ba_netmask.data(), ba_netmask.length());
    memcpy(conf->netcard.dns1, ba_nds1.data(), ba_nds1.length());
    memcpy(conf->netcard.dns2, ba_nds2.data(), ba_nds2.length());

    memcpy(conf->server.ip, ba_server_ip.data(), ba_server_ip.length());

    char *buf = (char *)malloc(sizeof(struct config) + HEAD_LEN +1);

    if(!buf)
          return;
    char *data = &buf[HEAD_LEN];
    memcpy(data, conf, sizeof(struct config));

    global->pipe->send_pipe(buf, UPDATE_CONFIG_PIPE, sizeof(struct config));
    free(buf);

    this->hide();
}

void DialogWindow::on_install_cacel_btn_clicked()
{
    this->hide();
}

void DialogWindow::on_install_confirm_btn_clicked()
{
    this->hide();
}


void DialogWindow::slots_dhcpable()
{
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
              dhcp_flag = 1;
              break;
           case 1:          //static
              DEBUG("set network static ip model");
              ui->ip_edit->setEnabled(true);
              ui->dns_edit->setEnabled(true);
              ui->dns2_edit->setEnabled(true);
              ui->netmask_edit->setEnabled(true);
              ui->gateway_edit->setEnabled(true);
              //ui->server_ip_edit->setEnabled(true);
              dhcp_flag = 0;
              break;
        default:
                return;
        }
        ui->ip_edit->setText("");
        ui->ip_edit->setText("");
        ui->dns_edit->setText("");
        ui->dns2_edit->setText("");
        ui->netmask_edit->setText("");
        ui->gateway_edit->setText("");
        //ui->server_ip_edit->setText("");
}

void DialogWindow::on_error_confirm_btn_clicked()
{
    Global *global = Global::getGlobal();
    global->setDisplay(0);
    this->hide();
}
