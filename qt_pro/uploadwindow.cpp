#include "uploadwindow.h"
#include "ui_uploadwindow.h"
#include "global.h"
#include "string.h"
#include <QDesktopWidget>
#include <QScreen>
#include <math.h>
#include <QKeyEvent>
#define STRPREFIX(a,b) (strncmp((a),(b),strlen((b))) == 0)


Uploadwindow::Uploadwindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Uploadwindow)
{
    ui->setupUi(this);

    Global *global = Global::getGlobal();
    if(!global->window_flag)
    {
        //setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
        //global->setDesktopRect(QApplication::desktop()->availableGeometry());
        this->setGeometry(QApplication::desktop()->availableGeometry());
    }

    date_label = ui->dataLabel;

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

    dialog_ui = global->main_ui->dialog_ui;

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
    ui->i_progress_label->setText("0%");
    //ui->i_netcard_label->adjustSize();
    ui->i_netcard_label->setWordWrap(true);
    ui->i_netcard_label->setAlignment(Qt::AlignTop);
    ui->i_netcard_label->setText(global->conf.terminal.netcard);
    //ui->i_cpu_label->adjustSize();

    ui->i_cpu_label->setWordWrap(true);
    ui->i_cpu_label->setAlignment(Qt::AlignTop);
    ui->i_cpu_label->setText(global->conf.terminal.cpu);
    ui->i_num_label->setText(QString::number(global->conf.terminal.id));
    ui->i_memory_label->setText(QString::number(global->conf.terminal.memory / 1024 / 1024) + "MB");
    ui->i_ip_label->setText(global->conf.netcard.ip);
    ui->i_mac_label->setText(global->conf.netcard.mac);
    ui->i_disk_info_label->setText(QString::number(global->conf.terminal.disk_size / 2/ 1024 / 1024) + "GB");
    DEBUG("global->conf.terminal.memory %lld global->conf.netcard.ip %s global->conf.netcard.mac %s", global->conf.terminal.memory, global->conf.netcard.ip,
             global->conf.netcard.mac);

    if(global->install_flag)
    {
        ui->i_title_label->setText(tr("维护模式"));
    }
    else
    {
        ui->i_title_label->setText(tr("部署模式"));
    }

    QLabel *logo_label = new QLabel(this);
    logo_label->setText("");
    logo_label->setStyleSheet("QLabel{"
                               "border-image: url(:/images/logo.png);"
                               "}");
    logo_label->setGeometry(30, 30, 164,47);
    logo_label->show();

    QLabel *ver_label = new QLabel(this);
    ver_label->setText(QString("v%1.%2.%3").arg(global->conf.major_ver).arg(0).arg(global->conf.minor_ver));
    ver_label->setStyleSheet("color: #FFFFFF;"
                             "font: 12pt '黑体';");
    ver_label->setGeometry(210, 45, 164, 47);
    ver_label->show();

    ui->i_progress_t_label->hide();
    ui->i_progress_label->hide();
    ui->i_remain_time_t_label->hide();
    ui->i_remain_time_label->hide();
    ui->i_upload_rate_t_label->hide();
    ui->i_upload_rate_label->hide();
    ui->i_download_rate_t_label->hide();
    ui->i_download_rate_label->hide();
    ui->i_diff_mode_label->hide();
    ui->i_diff_mode_t_label->hide();
}

Uploadwindow::~Uploadwindow()
{
    delete ui;
}

void Uploadwindow::showEvent(QShowEvent *e)
{
    Global *global = Global::getGlobal();
    if(global->download_flag)
    {
        ui->i_status_label->setText(tr("正在等待下载模板..."));
    }
    else
    {

        ui->i_status_label->setText(tr("正在等待上传模板..."));
    }
    ui->i_progress_label->setText("0%");
    ui->i_num_label->setText(QString::number(global->conf.terminal.id));
    ui->i_ip_label->setText(global->conf.netcard.ip);
    ui->i_mac_label->setText(global->conf.netcard.mac);
}
static int first_flag = 1;

void Uploadwindow::keyPressEvent(QKeyEvent *event)
{
#if 0
    if (event->key() == Qt::Key_F2)
    {
       dialog_ui->setType(DIALOG_NET);
       dialog_ui->show();
    }
#endif

    if (event->key() == Qt::Key_F8)
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, REBOOT_PIPE, 0);
    }

    if (event->key() == Qt::Key_F9)
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, SHUTDOWN_PIPE, 0);
    }

    if ((event->modifiers() == Qt::AltModifier) && event->key() == Qt::Key_F5)
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, INIT_PIPE, 0);
    }

	if((event->modifiers() == Qt::AltModifier) &&  (event->key() == Qt::Key_F1))	
    {
        char head[HEAD_LEN] = {0};
        Global *global = Global::getGlobal();
        global->pipe->send_pipe(head, EXIT_PROGRESS_PIPE, 0);
        qApp->exit();
    }

    if((event->modifiers() == Qt::AltModifier) &&  (event->key() == Qt::Key_F2))
    {
        if(first_flag)
        {
            char head[HEAD_LEN] = {0};
            Global *global = Global::getGlobal();
            DEBUG("RESET_PIPE ");
            global->pipe->send_pipe(head, RESET_PIPE, 0);
            first_flag = 0;
        }
    }

	if((event->modifiers() == Qt::AltModifier) &&  (event->key() == Qt::Key_F9))	
    {
        qApp->exit();
    }

    if(event->key() == Qt::Key_Enter || event->key() == Qt::Key_Return)
    {
        QWidget *currentitem = QApplication::focusWidget();
        if(currentitem->inherits("QPushButton"))
        {
            QPushButton *btn = qobject_cast<QPushButton *>(currentitem);
            btn->click();
        }
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

void Uploadwindow::showErrorDialog(char *err_msg)
{
    dialog_ui->setType(DIALOG_ERROR);
    dialog_ui->setErrorText(err_msg);
    dialog_ui->show();
}

QString add_suffix_float(double val, char const* suffix)
{
    if (val < 0.001)
    {
        QString ret;
        ret.resize(4 + 2);
        if (suffix) ret.resize(4 + 2 + strlen(suffix));
        return ret;
    }

    const char* prefix[] = {"kB", "MB", "GB", "TB", "PB"};
    const int num_prefix = sizeof(prefix) / sizeof(const char*);
    int i = 0;
    for (; i < num_prefix - 1; ++i)
    {
        val /= 1000.;
        if (fabs(val) < 1000.) break;
    }
    char ret[100];
    snprintf(ret, sizeof(ret), "%4.*f%s%s", val < 99 ? 1 : 0, val, prefix[i], suffix ? suffix : "");
    return ret;
}

void Uploadwindow::upConfig(struct config *conf)
{
    ui->i_num_label->setText(QString("%1").arg(conf->terminal.id));
    ui->i_ip_label->setText(conf->netcard.ip);
}

void Uploadwindow::Reboot()
{
    char head[HEAD_LEN] = {0};
    Global *global = Global::getGlobal();
    global->pipe->send_pipe(head, REBOOT_PIPE, 0);
}

void Uploadwindow::setProgressValue(struct progress_info *info)
{
    DEBUG("setProgressValue ");
#if 0
    if(!info)
        return ;
#endif
    DEBUG("info->progress %d", info->progress);
    if(info->type == 2) //安装或更新
    {
         ui->i_status_label->show();
         Global *global = Global::getGlobal();
         QString state;
         if(global->install_flag)
         {
             state = tr("更新客户端: ");
             if(info->progress == 100)
             {
                state = tr("更新客户端完成, 5秒后重启");
                QTimer::singleShot(5000, this, SLOT(Reboot()));
             }
         }
         else
         {
             state = tr("安装客户端: ");
             if(info->progress == 100)
             {
                state = tr("安装客户端完成,等待下载默认桌面");
                //QTimer::singleShot(5000, this, SLOT(Reboot()));
             }
         }
        if(info->progress != 100)
            state += QString(info->file_name);

        progress->setValue((double)info->progress);
        ui->i_status_label->setText(state);
    }
    else if(info->type == 1)
    {
        QString state;
        ui->i_status_label->show();
        if(STRPREFIX(info->state, "seeding"))
        {
            state = tr("上传模板中: ");
        }
        if(info->progress == 100)
        {
           state = tr("模板上传完成,5秒后重启");
           QTimer::singleShot(5000, this, SLOT(Reboot()));
        }
        progress->setValue((double)info->progress);
        state += QString(info->file_name);
        ui->i_status_label->setText(state);
    }
    else
    {
        ui->i_status_label->show();
        ui->i_progress_t_label->show();
        ui->i_progress_label->show();
        ui->i_remain_time_t_label->show();
        ui->i_remain_time_label->show();
        ui->i_upload_rate_t_label->show();
        ui->i_upload_rate_label->show();
        ui->i_download_rate_t_label->show();
        ui->i_download_rate_label->show();
        ui->i_diff_mode_t_label->show();
        ui->i_diff_mode_label->show();

        ui->i_upload_rate_label->setText("0kB/s");
        ui->i_download_rate_label->setText("0kB/s");

        int diff_mode = *(int *)&(info->storage[0]);

        DEBUG("diff_mode %d", diff_mode);
        if(diff_mode == 1)
        {
            ui->i_diff_mode_label->setText("覆盖");
        }
        else if(diff_mode == 2)
        {
            ui->i_diff_mode_label->setText("增量");
        }
        else
        {
            ui->i_diff_mode_label->setText("-");
        }

        QString state;
        unsigned long long remain_time = 0;
        if(STRPREFIX(info->state, "checking"))
        {
            state = tr("检测文件完整性: ");
        }
        else if(STRPREFIX(info->state, "downloading"))
        {
            state = tr("下载模板中: ");
            if(info->upload_rate > 0)
                ui->i_upload_rate_label->setText(add_suffix_float(info->upload_rate, "/s"));
            if(info->download_rate > 0)
                ui->i_download_rate_label->setText(add_suffix_float(info->download_rate, "/s"));
            ui->i_progress_label->setText(QString("%1%").arg(info->progress));
            if(info->download_rate != 0 && info->total_size != 0)
            {
                remain_time = (info->file_size - info->total_size)/info->download_rate;
                if(remain_time > 3600 * 24) //大于一天
                {
                    ui->i_remain_time_label->setText(tr("-秒"));
                }
                else if(remain_time < 3600 * 24 && remain_time > 3600 )
                {
                    ui->i_remain_time_label->setText(QString("%1小时").arg(remain_time / 3600));
                }
                else if(remain_time < 3600 && remain_time > 60 )
                {
                    ui->i_remain_time_label->setText(QString("%1分钟").arg(remain_time / 60 ));
                }
                else {
                    ui->i_remain_time_label->setText(QString("%1秒").arg(remain_time));
                }
            }
            else
                ui->i_remain_time_label->setText(tr("-秒"));
        }
        else if(STRPREFIX(info->state, "finished") || info->progress == 100)
        {
            if(info->progress == 100)
            {
                state = tr("完成模板下载：");
            }
            else {
                state = tr("下载模板中断：");
            }
            ui->i_progress_label->setText(QString("%1%").arg(info->progress));
            ui->i_upload_rate_label->setText("0kB/s");
            ui->i_download_rate_label->setText("0kB/s");
            ui->i_remain_time_label->setText(tr("-秒"));


            ui->i_progress_t_label->hide();
            ui->i_progress_label->hide();
            ui->i_remain_time_t_label->hide();
            ui->i_remain_time_label->hide();
            ui->i_upload_rate_t_label->hide();
            ui->i_upload_rate_label->hide();
            ui->i_download_rate_t_label->hide();
            ui->i_download_rate_label->hide();

            ui->i_diff_mode_label->hide();
            ui->i_diff_mode_t_label->hide();
        }
        else if(STRPREFIX(info->state, "seeding"))
        {
            state = tr("上传模板中: ");
        }
        progress->setValue((double)info->progress);
        state += QString(info->file_name);
        ui->i_status_label->setText(state);
    }
}





