#include "base.h"
#include "client.h"
#include "cJSON.h"
#include "packet.h"
#include <limits.h>
#include <ctype.h>
#include <sys/un.h>
#include <linux/rtc.h>
#include <linux/types.h>
#include <linux/netlink.h>
#include "torrent.h"

time_t current_time;
const char program_name[] = "bt_server";
int max_connections = 1024;
struct client **clients = NULL;

typedef enum yzy_cmd
{
    WAKEUP = 1000,
    SHUTDONW,   
    RESTART,
    DELETE,
    UPDATE_NAME,
    UPDATE_CONFIG,
    ENT,
    CLEAR_ALL_DESKTOP,
    
    SEND_DOWN_TORRENT = 1015,
    SEND_DESKTOP = 1017,
    //SHUT  

    HEARTBEAT = 9000,
    TERMINAL_LOGIN,
    TERMINAL_LOGOUT,
    GET_DATA_TIME,
    GET_CONFIG_VERSION,
    GET_CONFIG_INFO,
    UPDATE_CONFIG_INTO,
    P2V_OS_TRANSFORM = 9020,
    P2V_PROGRESS = 9021,
}yzy_cmd;


/* pipe */
int pipe_event[2];
int pipe_tcp[2];
int pipe_udp[2];
int pipe_bt[2];
int pipe_track[2];

pthread_t pthread_server, pthread_torrent, pthread_track;

int track_flag = 0;
int run_flag = 0;

//const ch
char *track_ip = NULL;
int track_port = 50020;
char tracker[128] = {0};
int torrent_port;

#define  SCHED_PRIORITY_SERVER 0

typedef enum BT_CMD {
	START_BT= 2000,
	STOP_BT,
	SET_TRACKER,
	ADD_TASK,
	DEL_TASK,
	MAKE_TORRETN,
	GET_TASK_STATE,

	TASK_END = 8000,
}BT_CMD;

int make_torrent(char *file_path, char *torrent_path, char *track);

static void sig_quit_listen(int e)
{
    char s = 'S';

    write(pipe_tcp[1], &s, sizeof(s));
    DEBUG("kill use signal end !");
}

static void do_exit()
{
	void *tret = NULL;
	pthread_join(pthread_server, &tret);
    DEBUG("pthread_exit %d event", (int *)tret);
}

static int send_code(struct client *cli, int code)
{
    int ret;

    if(cli->data_buf)
        free(cli->data_buf);
    
    cJSON *root = cJSON_CreateObject();
    char *buf = NULL;
    if(root)
    {   
        cJSON_AddNumberToObject(root, "code", code);
        cJSON_AddStringToObject(root, "msg", "Success");
    }
        
    cli->data_buf = cJSON_Print(root);
    cli->data_size = strlen(cli->data_buf);
    
    set_packet_head(cli->packet, read_packet_order(cli->packet), cli->data_size, JSON_TYPE, 1);
        
    ret = send_packet(cli);
    
    if(root)
       cJSON_Delete(root);
    return ret;
}

struct yzy_torrent{
    char uuid[36]; 
    char type;
    char sys_type;
    int dif_level;
    unsigned long long real_size;
    unsigned long long  space_size;
    unsigned long long  data_len;
};


static int recv_login(struct client *cli)
{
    int ret; 
    if(cli->data_buf)
        free(cli->data_buf);


	cli->data_buf = (char *)malloc(sizeof(yzy_torrent))

	yzy_torrent *torrent = (yzy_torrent *)&cli->data_buf[0];	
		
	strcpy(torrent->uuid, "1111",  4);
	torrent->type = 0;
	torrent->sys_type = 0;
	torrent->dif_level = 1;
	torrent->real_size = 1;
	torrent->space_size = 1;
	torrent->data_len  = 0;


    set_packet_head(cli->packet, GET_CONFIG_VERSION, cli->data_size, JSON_TYPE, 0);
    ret = send_packet(cli);

    if(root)
       cJSON_Delete(root);
    return ret; 
}

static int process_msg(struct client *cli)
{
	int ret;
	DEBUG("process_msg %d", read_packet_order(cli->packet));
	switch(read_packet_order(cli->packet))
	{
		case TERMINAL_LOGIN:
			ret = recv_login(cli);
			break;
	}
	return ret;
}

void close_client(struct client *cli)
{
	if(cli->fd)
		close(cli->fd);
	if(cli->packet)
		free(cli->packet);
	if(cli->data_buf)
		free(cli->data_buf);
	if(cli)
		free(cli);	
}

static void tcp_loop(int listenfd)
{
    int maxfd = 0, connfd, sockfd;
    int nready, ret, i, maxi = 0;
    int total_connections = 0;
    char buf[DATA_SIZE] = {0};          //pipe msg buf
    char *tmp = &buf[HEAD_LEN];

    struct client *cli = NULL;

    fd_set reset, allset;

    struct sockaddr_in cliaddr;
    socklen_t clilen = sizeof(cliaddr);

    struct timeval tv; 
    tv.tv_sec = 1;
    tv.tv_usec = 0;

    FD_ZERO(&allset);
    FD_SET(listenfd, &allset);
    FD_SET(pipe_tcp[0], &allset);

    maxfd = maxfd > listenfd ? maxfd : listenfd;
    maxfd = maxfd > pipe_tcp[0] ? maxfd : pipe_tcp[0];
    for(;;)
    {   
        tv.tv_sec = 1;
        reset = allset;
        ret = select(maxfd + 1, &reset, NULL, NULL, &tv);
        if(ret == -1) 
        {   
             if(errno == EINTR)
                continue;
            else if(errno != EBADF)
            {   
                FAIL("select %d %s ", errno, strerror(errno));
                return;
            }
        }
        nready = ret;
        /* pipe msg */
        if(FD_ISSET(pipe_tcp[0], &reset))
        {
            ret = recv(pipe_tcp[0], (void *)buf, sizeof(buf), 0);
            if(ret >= HEAD_LEN)
            {
                //process_server_pipe(buf, ret);
            }
            if(ret == 1)
            {
                if(buf[0] == 'S')
                {
                    DEBUG("event thread send stop msg");
                    break;
                }
           }
            if(--nready <= 0)
                continue;
        }
        /* new connect */
        if(FD_ISSET(listenfd, &reset))
        {
            connfd = accept(listenfd, (struct sockaddr*)&cliaddr, &clilen);
            if(connfd < 0)
                continue;
            cli = (struct client *)malloc(sizeof(struct client));
            if(!cli)
            {
                DEBUG("new connect and malloc struct client error :%s", strerror(errno));
                continue;
            }
            memset(cli, 0, sizeof(struct client));
            cli->fd = connfd;
            cli->data_size = PACKET_LEN;
            //cli->head_buf = (unsigned char *)malloc(HEAD_LEN);
            cli->packet = (unsigned char *)malloc(PACKET_LEN);
            if(!cli->packet)
            {
                DEBUG("fcntl connfd: %d  F_GETFL error :%s", connfd, strerror(errno));
                close_fd(connfd);
                free(cli);
                continue;
            }
            ret = fcntl(connfd, F_GETFL, 0);
            if(ret < 0)
            {
                DEBUG("fcntl connfd: %d  F_GETFL error :%s", connfd, strerror(errno));
                close_fd(connfd);
                free(cli->packet);
                free(cli);
                continue;
            }

            if(fcntl(connfd, F_SETFL, ret | O_NONBLOCK) < 0)
            {
                DEBUG("fcntl connfd: %d F_SETFL O_NONBLOCK error :%s", connfd, strerror(errno));
                close_fd(connfd);
                free(cli->packet);
                free(cli);
                continue;
            }
            /* recode client ip */
            if(inet_ntop(AF_INET, &cliaddr.sin_addr, cli->ip, sizeof(cli->ip)) == NULL)
            {
                DEBUG("connfd: %d inet_ntop error ",connfd, strerror(errno));
                close_fd(connfd);
                free(cli->packet);
                free(cli);
                continue;
            }
            FD_SET(connfd, &allset);
            for(i = 0; i < max_connections; i++)
            {
                if(clients[i] == NULL)
                    clients[i] = cli;
                break;
            }
            total_connections++;
            if(i > maxi)
                maxi = i;
            if(connfd > maxfd)
                maxfd = connfd;
            DEBUG("client index: %d total_connections: %d maxi: %d connfd ip: %s",i, total_connections, maxi, cli->ip);
            if(--nready <= 0)
                continue;
        }
        /* client msg */
        for(i = 0; i <= maxi; i++)
        {
            if(clients[i] == NULL || (sockfd = clients[i]->fd) < 0)
                continue;
            if(FD_ISSET(sockfd, &reset))
            {
                if(clients[i]->has_read_head == 0)
                {
                    if((ret = recv(sockfd, (void *)clients[i]->packet + clients[i]->pos,
                                        PACKET_LEN - clients[i]->pos, 0)) <= 0)
                    {
                        if(ret < 0)
                        {
                            if(errno == EINTR || errno == EAGAIN)
                                continue;
                        }
                        DEBUG("client close ip: %s port %d",
                                    clients[i]->ip, clients[i]->port);

                        FD_CLR(clients[i]->fd, &allset);
                        close_client(clients[i]);
                        clients[i] = NULL;
                        total_connections--;
                        continue;
                    }
                    clients[i]->pos += ret;
                    if(clients[i]->pos != PACKET_LEN)
                        continue;

#if 0
                    if(read_msg_syn(clients[i]->head_buf) != DATA_SYN)
                    {
                        DEBUG(" %02X client send SYN flag error close client index: %d ip: %s port %d",
                            read_msg_syn(clients[i]->head_buf), clients[i]->index, clients[i]->ip,
                            clients[i]->port);

                        FD_CLR(clients[i]->fd, &allset);
                        close_client(clients[i]);
                        clients[i] = NULL;
                        total_connections--;
                        continue;
                    }
#endif

                    clients[i]->has_read_head = 1;
					clients[i]->data_size = read_packet_size(clients[i]->packet) + read_packet_token(clients[i]->packet);
                    clients[i]->pos = 0;

                    if(clients[i]->data_size >= 0 && clients[i]->data_size < CLIENT_BUF)
                    {
                        if(clients[i]->data_buf)
                            free(clients[i]->data_buf);

                        clients[i]->data_buf = (unsigned char*)malloc(clients[i]->data_size + 1);
                        if(!clients[i]->data_buf)
                        {
                            DEBUG("malloc data buf error: %s close client: ip: %s port %d",
                                    strerror(errno),  clients[i]->ip, clients[i]->port);

                            FD_CLR(clients[i]->fd, &allset);
                            close_client(clients[i]);
                            clients[i] = NULL;
                            total_connections--;
                            continue;
                        }
                        memset(clients[i]->data_buf, 0, clients[i]->data_size);
                    }
                    else 
                    {
                        DEBUG("client send size: %d error close  ip: %s port %d",
                                clients[i]->data_size, clients[i]->ip, clients[i]->port);

                        FD_CLR(clients[i]->fd, &allset);
                        close_client(clients[i]);
                        clients[i] = NULL;
                        total_connections--;
                        continue;
                    }
                }

                if(clients[i]->has_read_head == 1)
                {
                    if(clients[i]->pos < clients[i]->data_size)
                    {
                        if((ret = recv(sockfd,clients[i]->data_buf+clients[i]->pos,
                                        clients[i]->data_size - clients[i]->pos,0)) <= 0)
                        {
                            if(ret < 0)
                            {
                                if(errno == EINTR || errno == EAGAIN)
                                    continue;
                                DEBUG("client ip: %s port %d",
                                         clients[i]->ip, clients[i]->port);

                                FD_CLR(clients[i]->fd, &allset);
                                close_client(clients[i]);
                                clients[i] = NULL;
                                total_connections--;
                                continue;
                            }
                        }
                        clients[i]->pos += ret;
                    }

                    if(clients[i]->pos == clients[i]->data_size)
                    {
                        if(process_msg(clients[i]))
                        {
                            DEBUG("process msg error : ip: %s port %d",
                                     clients[i]->ip, clients[i]->port);

                            FD_CLR(clients[i]->fd, &allset);
                            close_client(clients[i]);
                            clients[i] = NULL;
                            total_connections--;
                            continue;
                        }
                        memset(clients[i]->packet, 0, PACKET_LEN);
                        clients[i]->data_size = PACKET_LEN;
                        clients[i]->pos = 0;
                        if(clients[i]->data_buf)
                            free(clients[i]->data_buf);
                        clients[i]->data_buf = NULL;
                        clients[i]->has_read_head = 0;
                    }
                    if(clients[i]->pos > clients[i]->data_size)
                    {
                        DEBUG("loss msg data : ip: %s port %d",
                                clients[i]->ip, clients[i]->port);
                        FD_CLR(clients[i]->fd, &allset);
                        close_client(clients[i]);
                        clients[i] = NULL;
                        total_connections--;
                        continue;
                    }
                }
                if(--nready <= 0)
                    break;
            }
        }
    }
    DEBUG("thread server tcp end");
}

#define SCHED_PRIORITY_SERVER 0



void init_config()
{


}

int init_pipe()
{
    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_track) < 0)
    {
        DEBUG("create client bt err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_track[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_track[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_bt) < 0)
    {
        DEBUG("create client bt err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_bt[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_bt[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_tcp) < 0)
    {
        DEBUG("create server pipe err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_tcp[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_tcp[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_udp) < 0)
    {
        DEBUG("create client pipe err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_udp[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_udp[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_event) < 0)
    {
        DEBUG("create client pipe err %s", strerror(errno));
        return ERROR;
    }

    fcntl(pipe_event[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_event[1], F_SETFL, O_NONBLOCK);
    return SUCCESS;
}


void *thread_server(void *param)
{
    int ret = SUCCESS;
    pthread_attr_t st_attr;
    struct sched_param sched;
	
	int listenfd = *(int *)param;
    
    ret = pthread_attr_init(&st_attr);
    if(ret)
    {    
        DEBUG("ThreadUdp attr init warning ");
    }    
    ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
    if(ret)
    {    
        DEBUG("ThreadUdp set SCHED_FIFO warning");
    }    
    sched.sched_priority = SCHED_PRIORITY_SERVER;
    ret = pthread_attr_setschedparam(&st_attr, &sched);

    tcp_loop(listenfd);
    return (void *)ret;
}

int main(int argc, char *argv[])
{
	int ret;
	int server_s;
	srandom(time(NULL) + getpid());

	(void)time(&current_time);
	signal(SIGPIPE, SIG_IGN);
	//signal(SIGINT, SIG_IGN);	

	struct sigaction act;
    act.sa_handler = sig_quit_listen;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, 0); 
	
	init_logs();
    init_config();
    init_pipe();

    server_s = create_tcp();
    if(bind_socket(server_s, 50022) == -1) 
    {   
        FAIL("bind port %d error", 20001);
        return ERROR;
    }    

    clients = (struct client **)malloc(sizeof(struct client *) * max_connections);
    if(!clients)
    {
        FAIL("clients malloc error %s", strerror(errno));
        return ERROR;
    }
    memset(clients, 0, sizeof(struct client *) * max_connections);

	ret = pthread_create(&pthread_server, NULL, thread_server, (void *)&server_s);
	if(SUCCESS != ret)
	{
		DIE("create server thread ret %d error %s", ret, strerror(ret));
	}

	do_exit();
	return ret;
}



           //cJSON *desktop_group_uuid = cJSON_GetObjectItem(desktop, "desktop_group_uuid");
            //cJSON *desktop_group_status = cJSON_GetObjectItem(desktop, "desktop_group_status");
            //cJSON *default_desktop_group = cJSON_GetObjectItem(desktop, "default_desktop_group");
            //cJSON *os_sys_type = cJSON_GetObjectItem(desktop, "os_sys_type");
            //cJSON *desktop_name = cJSON_GetObjectItem(desktop, "desktop_name");
            //cJSON *template_uuid = cJSON_GetObjectItem(desktop, "template_uuid");
            //cJSON *sys_restore = cJSON_GetObjectItem(desktop, "sys_restore");
            //cJSON *data_restore = cJSON_GetObjectItem(desktop, "data_restore");
            //cJSON *disks = cJSON_GetObjectItem(desktop, "disks");

#if 0
           //desktop_info *desktop = (desktop_info *)malloc(sizeof(desktop_info));
            //if(!desktop)
            //return ERROR;

            if(desktop_group_name)
                strcpy(desktop->desktop_group_name, desktop_group_name->valuestring);
            if(desktop_group_uuid)
                strcpy(desktop->desktop_group_uuid, desktop_group_uuid->valuestring);
            if(desktop_name)
                strcpy(desktop->desktop_name, desktop_name->valuestring);
            if(template_uuid)
                strcpy(desktop->template_uuid, template_uuid->valuestring);
            if(desktop_group_status)
                desktop->desktop_group_status = desktop_group_status->valueint;
            if(default_desktop_group)
                desktop->default_desktop_group = default_desktop_group->valueint;
            if(os_sys_type)
                desktop->os_sys_type = os_sys_type->valueint;
            if(sys_restore)
                desktop->sys_restore = sys_restore->valueint;
            if(data_restore)
                desktop->data_restore = data_restore->valueint;

                   if(!disks)
            return ERROR;
        cJSON *uuid, *type, *prefix, *dif_level, *real_size,  *reserve_size, *torrent_file,*restore_flag;

        if(cJSON_GetArraySize(disks) > 0)
            desktop->disks = (disk_info *)malloc(sizeof(disk_info) * cJSON_GetArraySize(disks));
        else
            return ERROR;

        int i;
        desktop->disk_count = cJSON_GetArraySize(disks);
        DEBUG("desktop->disk_count !!!!!!!!!!! %d", desktop->disk_count);
        disk_info *disk;
        for(i = 0; i < cJSON_GetArraySize(disks); i++)
        {
            cJSON *item = cJSON_GetArrayItem(disks, i); 

            uuid = cJSON_GetObjectItem(item, "uuid");
            type = cJSON_GetObjectItem(item, "type");
            prefix = cJSON_GetObjectItem(item, "prefix");
            dif_level = cJSON_GetObjectItem(item, "dif_level");
            real_size = cJSON_GetObjectItem(item, "real_size");
            reserve_size = cJSON_GetObjectItem(item, "reserve_size");
            torrent_file = cJSON_GetObjectItem(item, "torrent_file");
            restore_flag = cJSON_GetObjectItem(item, "restore_flag");
            
            disk = &(desktop->disks[i]);
            if(uuid)
                strcpy(disk->uuid, uuid->valuestring);
            if(type)
                disk->type = type->valueint;
            if(prefix)
                strcpy(disk->prefix, prefix->valuestring);
            if(dif_level)
                disk->dif_level = dif_level->valueint;
            if(real_size)
                sscanf(real_size->valuestring, "%llu", &disk->real_size);
            if(reserve_size)
                sscanf(real_size->valuestring, "%llu", &disk->real_size);
            if(torrent_file)
                strcpy(disk->torrent_file, torrent_file->valuestring);
            if(restore_flag)
                disk->restore_flag = restore_flag->valueint;

            DEBUG("\n uuid %s \n type %d prefix %s dif_level %d \n real_size %llu reserve_size %llu \n"
                 "torrent_file %s restore_flag %d", disk->uuid, disk->type, disk->prefix, disk->dif_level,
                 disk->real_size, disk->reserve_size, disk->torrent_file, disk->restore_flag);
        }

            for(i = 0; i < cJSON_GetArraySize(disks); i++)
            {
                disk = &(desktop->disks[i]);
                DEBUG("\n uuid %s \n type %d prefix %s dif_level %d \n real_size %llu reserve_size %llu \n"
                    "torrent_file %s restore_flag %d", disk->uuid, disk->type, disk->prefix, disk->dif_level,
                    disk->real_size, disk->reserve_size, disk->torrent_file, disk->restore_flag);
            }
            //save_di

