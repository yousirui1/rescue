#ifndef __GLOBAL_H__
#define __GLOBAL_H__

/* main.c */
extern int pipe_event[2];
extern int pipe_tcp[2];
extern int pipe_udp[2];
extern int pipe_qt[2];
extern time_t current_time;
extern struct config conf;
extern char config_file[128];

void *thread_event(void *param);

/* client.c */
void *thread_client(void *param);

/* task.c */
void *thread_task(void *param);

/* shell.c */
extern char mount_sh[];
extern char umount_sh[];
extern char install_sh[];
extern const char err_msg_desc[][128] ;

/* window.c */
void *thread_qt(void *param);



#endif
