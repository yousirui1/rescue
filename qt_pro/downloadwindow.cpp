#include "downloadwindow.h"
#include "global.h"
#include "ui_downloadwindow.h"

#include <QKeyEvent>
#if 0
DownloadWindow::DownloadWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DownloadWindow)
{
    //ui->setupUi(this);

    Global *global = Global::getGlobal();
    if(!global->window_flag)
    {
        //setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
        //this->showFullScreen();

        this->setGeometry(global->desktop_rect);
        //this->setAutoFillBackground(true);
    }

    //date_label = ui->dataLabel;

    QString ip = "127.0.0.1";
    QString ip_state;
    QImage icon_img;
    QImage point_img;

    ip_state= tr("离线");
    icon_img.load(":/images/off-online.png");
    point_img.load(":/images/offline-point.png");

#if 0
    date_label = ui->dataLabel;
    ip_label = ui->ip_label;
    netstate_label = ui->netstate_label;
    neticon_label = ui->neticon_label;
    netpoint_label = ui->netpoint_label;
#endif

    dialog_ui = new DialogWindow();
    dialog_ui->hide();

    this->ip_label->setText(ip);
    this->netstate_label->setText(tr("离线"));
    this->neticon_label->setPixmap(QPixmap::fromImage(icon_img));
    this->netpoint_label->setPixmap(QPixmap::fromImage(point_img));

    progress = new QRoundProgressBar(ui->i_rangbar);
    progress->setValue(0);
    progress->setFixedSize(150,150);
    progress->setInnerOuterRate(0.87);
    progress->setBarStyle(QRoundProgressBar::StyleLine);
    progress->setOutlinePenWidth(7);
    //progress->setDataColors(QColor(3,71,168,0));
    progress->setDataPenWidth(9);
    progress->setStyleSheet("QWidget{background-color: rgba(0,0,0,1);}");
#if 0
    ui->i_progress_label->setText("0%");

    ui->i_netcard_label->setText(global->conf.terminal.netcard);
    ui->i_cpu_label->setText(global->conf.terminal.cpu);
    ui->i_num_label->setText("YZY-" + QString::number(global->conf.terminal.id));
    ui->i_memory_label->setText(QString::number(global->conf.terminal.memory / 100) + "MB");
    ui->i_ip_label->setText(global->conf.netcard.ip);
    ui->i_mac_label->setText(global->conf.netcard.mac);
    ui->i_disk_info_label->setText(QString::number(global->conf.terminal.disk_size / 1024 / 1024) + "GB");
#endif
    qWarning("global->conf.terminal.memory %lld global->conf.netcard.ip %s global->conf.netcard.mac %s", global->conf.terminal.memory, global->conf.netcard.ip,
             global->conf.netcard.mac);
}

DownloadWindow::~DownloadWindow()
{
    delete ui;
}

void DownloadWindow::keyPressEvent(QKeyEvent *event)
{
    if (event->key() == Qt::Key_F2)
    {
       dialog_ui->setType(DIALOG_NET);
       dialog_ui->show();
    }

    if (event->key() == Qt::Key_F10)
    {
        qApp->exit();
    }

}

void DownloadWindow::setProgressValue(struct progress_info *info)
{
    //int index = str.indexOf("]");
    //QStringList sections = str.split(QRegExp("^(-?\d+)(\.\d+)?$" "^[A-Za-z]+$"));
    //qWarning()<<str;
    //ui->i_status_label->setText(str);
    //progress->setValue(value);
    //progress->setValue(i);
    //info->progress

    //ui->i_progress_label->setText(QString("%1%").arg(info->progress));
    //ui->i_remain_time_label->setText(QString("%1秒").arg(info->progress));
}

#endif
