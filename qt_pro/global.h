#ifndef GLOBAL_H
#define GLOBAL_H

#include <QTimer>
#include <QDebug>
#include <QByteArray>
#include <QString>
#include <QLabel>
#include "pipethread.h"
#include "mainwindow.h"
#include "external.h"
#include "downloadwindow.h"

#define __DEBUG__
#ifdef __DEBUG__
#define DEBUG(format,...) \
        do { qWarning("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
        }while(0)

#define FAIL(format,...) \
        do { qWarning("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
        }while(0)

#define DIE(format,...) \
        do { qWarning("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
            pthread_exit((void *)ERROR); \
        }while(0)
#else
#define DEBUG(format,...)
#define FAIL(format,...)
#define DIE(format,...) pthread_exit((void *)ERROR)
#endif      //__DEBUG__

#define STRPREFIX(a,b) (strncmp((a),(b),strlen((b))) == 0)

typedef enum WINDOW_PAGE{
    MAINWINDOW_PAGE = 0,
    CONFIGWINDOW_PAGE,
    UPLOADWINDOW_PAGE,
    OSWINDOW_PAGE,
    DIALOGWINDOW_PAGE,
}WINDOW_PAGE;

typedef enum NET_STATE{
    NET_UP = 0,
    NET_DOWN,
    NET_ONLINE,
    NET_OFFLINE,
    NET_EXCEPTION,
}NET_STATE;

typedef enum PIPE_MSG{
    CLIENT_CONNECT_PIPE = 1,
    CLIENT_DISCONNECT_PIPE,
    CLIENT_ONLINE_PIPE,
    CLIENT_DOWN_PIPE,
	INIT_PIPE,
    INSTALL_PIPE,
    UPDATE_CONFIG_PIPE,
    P2V_OS_PIPE,
    PROGRESS_PIPE,
    PROGRESS_ERROR_PIPE,
    EXIT_PROGRESS_PIPE,
    REBOOT_PIPE,
    SHUTDOWN_PIPE,
    ERROR_MSG_PIPE,
    INSTALL_DONE,
    RESET_PIPE,
}PIPE_MSG;



struct hwinfo
{ 
    char num[8];      //序号
    char name[24];
    char cpu[128];
    char netcard[128];
    unsigned int memory;
    unsigned int disk_size;
    unsigned int use_desktop;
    unsigned int down_desktop;
};

struct progress_info{
    char file_name[36];
    char state[12];
    unsigned int long progress;
    unsigned long long download_rate;
    unsigned long long upload_rate;
    unsigned long long total_size;      //下载总大小
    unsigned long long file_size;

    int type;

    char image_name[128];
    char storage[128];
};




struct template_info{
    unsigned char template_name[36];
    unsigned int template_desc[128];
    //float template_size;
};

class Global : public QObject
{
Q_OBJECT
public:
     static Global *getGlobal();
     void setPipe(int fd[2]);
     void closePipe();
     void setMainWindow(MainWindow *);
     void setUploadWindow(Uploadwindow *);
     void setConfigWindow(ConfigWindow *);
     void setOSWindow(OSWindow *);
     void setDialogWindow(DialogWindow *);
     void setNetState(NET_STATE);
     //void setHwInfo(struct hwinfo *info);
     void setConfig(struct config *conf);
     void setProgressValue(struct progress_info *info);
     void setWindowFlag(int flag);
     void setDownLoadFlag(int flag);
     void FormatDisk();
     void setDesktopRect(QRect rect);
	 void setInstallFlag(int flag);
     void setDownloadWindow(DownloadWindow *w);
     void startTimeOut();
     void setDisplay(int index);
     void setErrorMsg(int msg);
     void setTemplate(struct template_info *info);

private:
     Global();
     ~Global();
signals:
    void display(int number);
    void close_pipe();
    void setValue(struct progress_info *info);
    void setError(QString msg);

public slots:
     void setCurrentIndex(int);

private slots:
    void onUpdateTime();

private:
    static Global *mGlobal;

public:
    QTimer *timer;
    QLabel *data_label;
    MainWindow *main_ui;
    Uploadwindow *upload_ui;
    ConfigWindow *config_ui;
    OSWindow *os_ui;
    DialogWindow *dialog_ui;
    //QString m_ip;
    DownloadWindow *download_ui;
    NET_STATE net_state;
    int current_index;
    //struct hwinfo hw;
    PipeThread *pipe;
    struct config conf;
    int window_flag;
    QRect desktop_rect;
	int install_flag;
    int download_flag;
    struct template_info template_i;

    QStringList err_msg_list;
};




#endif // GLOBAL_H
