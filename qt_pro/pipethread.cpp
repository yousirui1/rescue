#include "pipethread.h"
#include <errno.h>
#include <unistd.h>
#include <time.h>
#include <stdint.h>
#include <QApplication>
#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else

#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>
#endif

#include "global.h"

PipeThread::PipeThread(int pipe_fd[2])
{
    pipe_qt[0] = pipe_fd[0];
    pipe_qt[1] = pipe_fd[1];
    run_flag = 1;
}

typedef struct _req_head
{
    unsigned char syn;
    unsigned char encrypt_flag;
    unsigned short cmd;
    unsigned int data_size;
}req_head;

unsigned short read_msg_order(unsigned char * buf)
{
    return *(unsigned short *)&buf[DATA_ORDER_OFFSET];
}

void set_request_head(char *buf, char encrypt_flag, short cmd, int data_size)
{
    int ret = -1;
    req_head *head = (req_head *)buf;
    head->syn = 0xff;
    head->encrypt_flag = encrypt_flag;
    head->cmd = cmd;
    head->data_size = data_size;
}

void PipeThread::close_pipe()
{
    run_flag = 0;
    pipe_qt[0] = 0 ;
    pipe_qt[1] = 0;
}

int PipeThread::send_pipe(char *buf, short cmd, int size)
{
    int ret;
    set_request_head(buf, 0x0, cmd, size);
    if(pipe_qt[0])
        ret = write(pipe_qt[0], buf, size + HEAD_LEN);
    return ret;
}

void PipeThread::process_pipe(unsigned char *msg, int len)
{
    int ret;
    unsigned char *tmp = &msg[HEAD_LEN];
    DEBUG("process_pipe read_msg_order(msg) %d", read_msg_order(msg));
    switch(read_msg_order(msg))
    {
        case CLIENT_CONNECT_PIPE:           //连接中
        {
            Global *global = Global::getGlobal();
            global->net_state = NET_UP;
            break;
        }
        case CLIENT_DISCONNECT_PIPE:        //断开
        {
            Global *global = Global::getGlobal();
            global->net_state = NET_OFFLINE;
            break;
        }

        case CLIENT_DOWN_PIPE:        //短网线
        {
            DEBUG("CLIENT_DOWN_PIPE");
            Global *global = Global::getGlobal();
            global->net_state = NET_DOWN;
            break;
        }

        case CLIENT_ONLINE_PIPE:            //在线
        {
            Global *global = Global::getGlobal();
            global->net_state = NET_ONLINE;
            break;
        }

        case UPDATE_CONFIG_PIPE:              //更新配置
        {
            Global *global = Global::getGlobal();

            struct config *conf = (struct config *)tmp;
            global->setConfig(conf);
            break;
        }
        case PROGRESS_PIPE:
        {
            Global *global = Global::getGlobal();
            struct progress_info *info = (struct progress_info *)tmp;
            if(!global->download_flag)  //上传
            {
                memcpy(info->filename, global->template_i.template_name, sizeof(global->template_i.template_name));
            }
            global->setProgressValue(info);
            break;
        }
        case ERROR_MSG_PIPE:
        {
            Global *global = Global::getGlobal();
            global->setErrorMsg(*(int*)&tmp[0]);
            break;
        }
        default:
           break;
    }
}

void PipeThread::run()
{
    int ret, maxfd = -1, nready;
    unsigned char buf[1024 * 10] = {0};
    fd_set fds;

    FD_ZERO(&fds);
    FD_SET(pipe_qt[0], &fds);
    maxfd = pipe_qt[0];
    Global *global = Global::getGlobal();
    while(!global->main_ui)
    {
        sleep(1);
    }

#ifndef _WIN32
    while(run_flag)
    {
        ret = select(maxfd + 1, &fds, NULL, NULL, NULL);
        if(ret < 0)
            continue;

        nready = ret;

        if(FD_ISSET(pipe_qt[0], &fds))
        {
            ret = read(pipe_qt[0], (unsigned char *)buf, sizeof(buf));
            if(ret >= HEAD_LEN)
            {
				DEBUG("qt process_pipe start");
                process_pipe(buf, ret);
				DEBUG("qt process_pipe end");
            }
            if(ret == 1 && buf[0] == 'S')
            {
                run_flag = 0;
                qApp->exit();
                break;
            }
            if(--nready <= 0)
                continue;
        }
    }
#endif
}
