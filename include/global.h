#ifndef __GLOBAL_H__
#define __GLOBAL_H__

#include "queue.h"
#include "device.h"

/* main.c */
extern time_t current_time;
extern struct configs conf;
extern char config_file[128];
extern int pipe_event[2];
extern int pipe_tcp[2];
extern int pipe_ui[2];
extern int pipe_task[2];
struct sock_udp create_udp(char *ip, int port, int mreq_flag);

void *thread_event(void *param);

/* client.c */
void *thread_client(void *param);
extern struct client m_client;

/* task.c */
void *thread_task(void *param);
void clear_task();
extern QUEUE task_queue;
/* shell.c */
extern char mount_sh[];
extern char mount_mmc_sh[];
extern char umount_sh[];
extern char install_sh[];
extern char install_sh_1[];
extern char upgrad_sh[];
extern const char err_msg_desc[][128] ;

/* ui.c */
void *thread_ui(void *param);

/* device.c */
extern struct device_info dev_info;

/* log.c */
void log_msg(const char *fmt, ...);
void err_msg(const char *fmt, ...);

/* socket.c */
int send_pipe(char *buf, short cmd, int size, int type);

extern pthread_mutex_t bt_mtx;
extern pthread_cond_t bt_cond;

void *thread_bt(void *param);
extern int download_finish;

#endif
