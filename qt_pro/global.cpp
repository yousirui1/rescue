#include "global.h"

#include <QDateTime>


QStringList err_msg_list;


Global::Global()
{
    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(onUpdateTime()));
    current_index = 0;
    net_state = NET_UP;
    download_flag = 1;
    memset(&template_i, 0, sizeof(struct template_info));
    memset(&conf, 0, sizeof(struct config));
    err_msg_list<<QString::fromUtf8("安装失败")<<QString::fromUtf8("没有找到可用的磁盘")<<QString::fromUtf8("未检测到安装U盘")<<QString::fromUtf8("格式化硬盘失败")<<QString::fromUtf8("P2V 上传系统失败,\n请关闭Window硬盘快速启动")
               <<QString::fromUtf8("P2V 磁盘错误,上传失败")<<QString::fromUtf8("P2V 模板名已存在, 上传失败")<<QString::fromUtf8("P2V 失败")<<QString::fromUtf8("硬盘容量不足下载失败");

    QStringListIterator strIterator((err_msg_list));
}

Global::~Global()
{

}

void Global::startTimeOut()
{
    timer->start(1000);

}

void Global::setMainWindow(MainWindow *w)
{
    main_ui = w;
}

void Global::setUploadWindow(Uploadwindow *w)
{
    upload_ui = w;
    connect(this, SIGNAL(setValue(struct progress_info *)), upload_ui, SLOT(setProgressValue(struct progress_info *)));
}

void Global::setDownloadWindow(DownloadWindow *w)
{
    download_ui = w;
    connect(this, SIGNAL(setValue(struct progress_info *)), download_ui, SLOT(setProgressValue(struct progress_info *)));
}


void Global::setConfigWindow(ConfigWindow *w)
{
    config_ui = w;
}

void Global::setOSWindow(OSWindow *w)
{
    os_ui = w;
}

void Global::setDialogWindow(DialogWindow *w)
{
    dialog_ui = w;
}

void Global::setErrorMsg(int msg)
{
    DEBUG("------%d-------", msg);
    switch(current_index)
    {
        case MAINWINDOW_PAGE:         //MainWindow
            main_ui->dialog_ui->setErrorText(err_msg_list[msg]);
            break;
        case CONFIGWINDOW_PAGE:         //ConfigWindow
             config_ui->dialog_ui->setErrorText(err_msg_list[msg]);
             break;
        case UPLOADWINDOW_PAGE:         //UploadWindow
             upload_ui->dialog_ui->setErrorText(err_msg_list[msg]);
            break;
        case OSWINDOW_PAGE:         //OSWindow
            os_ui->dialog_ui->setErrorText(err_msg_list[msg]);
            break;
    }
}

void Global::setConfig(struct config *info)
{
    memcpy(&conf, info, sizeof(struct config));
    if(current_index == UPLOADWINDOW_PAGE)
    {
        upload_ui->upConfig(info);
    }
}

void Global::setDesktopRect(QRect rect)
{
    this->desktop_rect = rect;
}

void Global::setPipe(int fd[2])
{
    pipe = new PipeThread(fd);
    pipe->start();
}

void Global::setCurrentIndex(int index)
{
    current_index = index;
    onUpdateTime();
}

void Global::setWindowFlag(int flag)
{
    window_flag = flag;
}

void Global::setInstallFlag(int flag)
{
	this->install_flag = flag;	
}

void Global::setDownLoadFlag(int flag)
{
    this->download_flag = flag;
    DEBUG("setDownLoadFlag %d", this->download_flag);
}

void Global::setProgressValue(struct progress_info *info)
{
    emit setValue(info);
}

void Global::setDisplay(int index)
{
    emit display(index);
}

void Global::setTemplate(struct template_info *info)
{
    memcpy(&template_i, info, sizeof(struct template_info));
}

void Global::FormatDisk()
{
    //int PipeThread::send_pipe(char *buf, short cmd, int size)
    //pipe->send_pipe("");
}


void Global::onUpdateTime()
{
    QString ip_state;
    QImage icon_img;
    QImage point_img;
    switch (net_state)
    {
        case NET_UP:
            ip_state= tr("离线");
            icon_img.load(":/images/reconnect.png");
               point_img.load(":/images/offline-point.png");
            break;
        case NET_ONLINE:
            ip_state= tr("在线");
            icon_img.load(":/images/online.png");
            point_img.load(":/images/online-point.png");
            break;
        case NET_DOWN:
            ip_state= tr("断开");
            icon_img.load(":/images/net_down.png");
               point_img.load(":/images/offline-point.png");
            break;
        case NET_OFFLINE:
            ip_state= tr("离线");
            icon_img.load(":/images/off-online.png");
            point_img.load(":/images/offline-point.png");
            break;
        case NET_EXCEPTION:
            ip_state= tr("异常");
            icon_img.load(":/images/net_exception.png");
            point_img.load(":/images/offline-point.png");
            break;
        default:
            ip_state= tr("离线");
            icon_img.load(":/images/off-online.png");
            break;
    }

    switch(current_index)
    {
        case MAINWINDOW_PAGE:         //MainWindow
            main_ui->date_label->setText(QDateTime::currentDateTime().toString(" hh:mm:ss\nyyyy/MM/dd"));
            main_ui->ip_label->setText(conf.netcard.ip);
            main_ui->netstate_label->setText(ip_state);
            main_ui->neticon_label->setPixmap(QPixmap::fromImage(icon_img));
            main_ui->netpoint_label->setPixmap(QPixmap::fromImage(point_img));
            break;
        case CONFIGWINDOW_PAGE:         //ConfigWindow
            config_ui->date_label->setText(QDateTime::currentDateTime().toString(" hh:mm:ss\nyyyy/MM/dd"));
            //config_ui->date_label->setText(QDateTime::currentDateTime().toString(" hh:mm:ss\nyyyy/MM/dd"));
             break;
        case UPLOADWINDOW_PAGE:         //UploadWindow
            upload_ui->date_label->setText(QDateTime::currentDateTime().toString(" hh:mm:ss\nyyyy/MM/dd"));
            upload_ui->ip_label->setText(conf.netcard.ip);
            upload_ui->netstate_label->setText(ip_state);
            upload_ui->neticon_label->setPixmap(QPixmap::fromImage(icon_img));
            upload_ui->netpoint_label->setPixmap(QPixmap::fromImage(point_img));
            break;
        case OSWINDOW_PAGE:         //OSWindow
            os_ui->date_label->setText(QDateTime::currentDateTime().toString(" hh:mm:ss\nyyyy/MM/dd"));
            os_ui->date_label->setText(QDateTime::currentDateTime().toString(" hh:mm:ss\nyyyy/MM/dd"));
            os_ui->ip_label->setText(conf.netcard.ip);
            os_ui->netstate_label->setText(ip_state);
            os_ui->neticon_label->setPixmap(QPixmap::fromImage(icon_img));
            os_ui->netpoint_label->setPixmap(QPixmap::fromImage(point_img));
            break;
        case DIALOGWINDOW_PAGE:         //DialogWindow
            //main_ui->date_label->setText(QDateTime::currentDateTime().toString(" hh:mm:ss\nyyyy/MM/dd"));
            break;
    }
}

void Global::setNetState(NET_STATE state)
{
    net_state = state;
}


void Global::closePipe()
{
    pipe->run_flag = 0;
}

Global *Global::mGlobal = NULL;

Global *Global::getGlobal()
{
    if(mGlobal == NULL)
    {
        mGlobal = new Global();
    }
    return mGlobal;
}

#if 0
QString Global::AutoFeed(QString text)
{
    QString strText = text;
    int AutoIndex = 1;
    if(!strText.isEmpty())
    {
        for(int i = 1;)
    }
}
#endif

