#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "external.h"
#include <QFontDatabase>
#include "global.h"
#include <QDebug>
#include <QKeyEvent>
#include <QDesktopWidget>
#include <QScreen>
//#include <QFile

void loadFont(QString fontFileName)
{
    //QFont font_zh;
    int index = QFontDatabase::addApplicationFont(fontFileName);
    qDebug()<<"index ID:"<<index;
}

MainWindow::MainWindow(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Global *global = Global::getGlobal();
    if(!global->window_flag)
    {
        setWindowFlags(Qt::WindowStaysOnBottomHint | Qt::FramelessWindowHint);
        this->showFullScreen();
        this->setAutoFillBackground(true);
    }

    global->setDesktopRect(QApplication::desktop()->availableGeometry());
    global->setMainWindow(this);
    dialog_ui = new DialogWindow();

    loadFont("/opt/freetype2/font/pin.ttf");
    stackLayout = new QStackedLayout();
    upload_ui = new Uploadwindow();
    config_ui = new ConfigWindow();
    os_ui = new OSWindow();

    stackLayout->addWidget(this);       //0
    stackLayout->addWidget(config_ui);  //1
    stackLayout->addWidget(upload_ui);  //2
    stackLayout->addWidget(os_ui);      //3
    //stackLayout->addWidget(dialog_ui);  //4

    connect(this, SIGNAL(display(int)), stackLayout, SLOT(setCurrentIndex(int)));
    connect(upload_ui, SIGNAL(display(int)), stackLayout, SLOT(setCurrentIndex(int)));
    connect(config_ui, SIGNAL(display(int)), stackLayout, SLOT(setCurrentIndex(int)));
    connect(os_ui, SIGNAL(display(int)), stackLayout, SLOT(setCurrentIndex(int)));
    connect(global, SIGNAL(setError(QString)), dialog_ui, SLOT(setErrorText(QString)));

    global->setUploadWindow(upload_ui);
    global->setConfigWindow(config_ui);
    global->setOSWindow(os_ui);
    global->setDialogWindow(dialog_ui);

    connect(global, SIGNAL(display(int)), stackLayout, SLOT(setCurrentIndex(int)));
    connect(this, SIGNAL(display(int)), global, SLOT(setCurrentIndex(int)));
    connect(upload_ui, SIGNAL(display(int)), global, SLOT(setCurrentIndex(int)));
    connect(config_ui, SIGNAL(display(int)), global, SLOT(setCurrentIndex(int)));
    connect(os_ui, SIGNAL(display(int)), global, SLOT(setCurrentIndex(int)));
    //connect(dialog_ui, SIGNAL(display(int)), global, SLOT(setCurrentIndex(int)));

    //

    date_label = ui->dataLabel;
    ip_label = ui->ip_label;
    netstate_label = ui->netstate_label;
    neticon_label = ui->neticon_label;
    netpoint_label = ui->netpoint_label;

    QString ip = "127.0.0.1";
    QString ip_state;
    QImage icon_img;
    QImage point_img;

    ip_state= tr("离线");
    icon_img.load(":/images/off-online.png");
    point_img.load(":/images/offline-point.png");

    this->ip_label->setText(ip);
    this->netstate_label->setText(ip_state);
    this->neticon_label->setPixmap(QPixmap::fromImage(icon_img));
    this->netpoint_label->setPixmap(QPixmap::fromImage(point_img));


    mainLayout = new QVBoxLayout;
    mainLayout->addLayout(stackLayout);
    setLayout(mainLayout);
    dialog_ui->hide();
    global->startTimeOut();
    global->pipe->start();
	DEBUG("global->ip %s", global->conf.netcard.ip);
    if(global->install_flag)
    {
        emit display(2);
        this->hide();
    }
    else {
        if(strlen(global->conf.server.ip) != 0)
        {
            Global *global = Global::getGlobal();
            char head[HEAD_LEN] = {0};
            global->pipe->send_pipe(head, INSTALL_PIPE, 0);
            global->setDownLoadFlag(1);

            emit display(2);
            this->hide();
        }
        else {
             emit display(0);
        }

    }

}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::keyPressEvent(QKeyEvent *event)
{
    DEBUG("keyPressEvent");
    if (event->key() == Qt::Key_F2)
    {
        DEBUG("F2");
        //emit display(4);
        dialog_ui->setType(DIALOG_NET);
        dialog_ui->show();
    }
    if (event->key() == Qt::Key_Enter)
    {
        DEBUG("Entry");
    }
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

#if 0
void QWidget::focusOutEvent ( QFocusEvent * event )
{

}
#endif

void MainWindow::on_installButton_clicked()
{
#if 0
    Global *global = Global::getGlobal();
    char head[HEAD_LEN] = {0};
    global->pipe->send_pipe(head, INSTALL_PIPE, 0);
    emit display(2);
#endif
    emit display(1);
}

void MainWindow::on_uploadButton_clicked()
{
    emit display(3);
}

void MainWindow::on_rebootButton_clicked()
{
    char head[HEAD_LEN] = {0};
    Global *global = Global::getGlobal();
    global->pipe->send_pipe(head, REBOOT_PIPE, 0);
}

