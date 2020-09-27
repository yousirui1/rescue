#include "oswindow.h"
#include "ui_oswindow.h"
#include "global.h"

OSWindow::OSWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::OSWindow)
{
    ui->setupUi(this);

    Global *global = Global::getGlobal();
    if(!global->window_flag)
    {
        setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
        this->showFullScreen();
        this->setAutoFillBackground(true);
    }

    QString ip = "127.0.0.1";
    QString ip_state;
    QImage icon_img;
    QImage point_img;

    ip_state= tr("离线");
    icon_img.load(":/images/off-online.png");
    point_img.load(":/images/offline-point.png");

    date_label = ui->dataLabel;
    ip_label = ui->ip_label;
    netstate_label = ui->netstate_label;
    neticon_label = ui->neticon_label;
    netpoint_label = ui->netpoint_label;

    dialog_ui = new DialogWindow();

    this->ip_label->setText(ip);
    this->netstate_label->setText(ip_state);
    this->neticon_label->setPixmap(QPixmap::fromImage(icon_img));
    this->netpoint_label->setPixmap(QPixmap::fromImage(point_img));

    date_label = ui->dataLabel;
    QRegExpValidator *size_validator = new QRegExpValidator(QRegExp("^[0-9]{1,10}[.][0-9]{1}"), this);
    ui->template_size_edit->setValidator(size_validator);
    ui->template_name_result_label->hide();
    //ui->template_size_result_label->hide();

    ui->template_size_edit->setText(QString("%1G").arg((global->conf.terminal.disk_size / 2/ 1024 / 1024)));
    ui->template_size_edit->setFocusPolicy(Qt::NoFocus);

    QLabel *logo_label = new QLabel(this);
    logo_label->setText("");
    logo_label->setStyleSheet("QLabel{"
                               "border-image: url(:/images/logo.png);"
                               "}");
    logo_label->setGeometry(30, 30, 164,47);
    logo_label->show();
    dialog_ui->hide();
}

OSWindow::~OSWindow()
{
    delete ui;
}


void OSWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F8)
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, REBOOT_PIPE, 0);
    }
	if((event->modifiers() == Qt::AltModifier) &&  (event->key() == Qt::Key_F1))
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, EXIT_PROGRESS_PIPE, 0);
        qApp->exit();
    }
}

void OSWindow::on_pushButton_12_clicked()
{
#if 1
    if(ui->template_name_edit->text().isEmpty())
    {
        ui->template_name_result_label->show();
        return;
    }
#endif


    Global *global = Global::getGlobal();
#if 1
    if(global->net_state != NET_ONLINE)
    {
        dialog_ui->setType(DIALOG_NET);
        dialog_ui->show();
        return;
    }
#endif
    global->setDownLoadFlag(0);


    QByteArray ba_template_name = ui->template_name_edit->text().toLatin1();
    QByteArray ba_dst_edit = ui->dst_edit->toPlainText().toLatin1();

    struct template_info info;
    memset(&info, 0, sizeof(struct template_info));
    memcpy(info.template_name, ba_template_name.data(), ba_template_name.length());
    memcpy(info.template_desc,  ba_dst_edit.data(),  ba_dst_edit.length());

    DEBUG("%s", info.template_name);
    DEBUG("%s", info.template_desc);

    global->setTemplate(&info);
    char *buf = (char *)malloc(sizeof(struct template_info) + HEAD_LEN +1);
    if(!buf)
          return;
    char *data = &buf[HEAD_LEN];
    memcpy(data, &info, sizeof(struct template_info));

    global->pipe->send_pipe(buf, P2V_OS_PIPE, sizeof(struct template_info));

    free(buf);

    emit display(2);
}

void OSWindow::on_pushButton_11_clicked()
{
    emit display(0);
}

void OSWindow::on_template_size_edit_textChanged(const QString &arg1)
{
    //if(ui->template_name_edit->text().end() != (QChar *)('G'))
    ui->template_size_edit->setText(arg1);
}
