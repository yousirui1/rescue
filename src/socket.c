#include "base.h"
#include "packet.h"
#include "client.h"


/* packet msg */
unsigned short read_packet_order(unsigned char *buf)
{
    return *(uint32_t *)&buf[PACKET_ORDER_OFFSET];
}

uint32_t read_packet_size(unsigned char *buf)
{
    return *(uint32_t *)&buf[PACKET_LEN_OFFSET];
}

unsigned short read_packet_token(unsigned char *buf)
{
    return *(uint16_t *)&buf[PACKET_TOKEN_OFFSET];
}

unsigned short read_packet_supplementary(unsigned char *buf)
{
	return *(uint16_t *)&buf[PACKET_SUPPLEMENTARY_OFFSET];
}

/* pipe msg */
unsigned char read_msg_syn(unsigned char *buf)
{   
    return *(unsigned char *)&buf[DATA_SYN_OFFSET];
}

unsigned short read_msg_order(unsigned char *buf)
{   
    return *(unsigned short *)&buf[DATA_ORDER_OFFSET];
}

int read_msg_size(unsigned char *buf)
{   
    return *(int *)&buf[DATA_LEN_OFFSET];
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

/* buf packet 
 * cmd  1000
 * data_type ox00:二进制数据 0x01:json 0x02:protobuf
 */
void set_packet_head(char *buf, int cmd, int data_size, char data_type, int req_flag)
{
    yzy_packet *packet = (yzy_packet *)buf;
    packet->version_chief = 1;
    packet->version_sub = 0;
    packet->service_code = cmd;
    packet->request_code = 0;
    packet->dataSize = data_size;
    packet->dataType = 0x01;
    packet->encoding = 0x00;
	if(conf.install_flag)
    	packet->clientType = 0x02;
	else
    	packet->clientType = 0x05;
	
    if(req_flag)
        packet->reqOrRes = 0x02;
    else
        packet->reqOrRes = 0x01;
    packet->supplementary = 0x00;
}    

int send_request(struct client *cli)
{
    int ret = ERROR;
    if (!cli || !cli->fd)
        return ERROR;
    ret = send_msg(cli->fd, cli->head_buf, HEAD_LEN);
    if (cli->data_buf && cli->data_size < DATA_SIZE && cli->data_size > 0)
        ret = send_msg(cli->fd, cli->data_buf, cli->data_size);
    return ret;
}

int send_packet(struct client *cli)
{
    int ret;
    if (!cli || !cli->fd)
        return ERROR;

    ret = send_msg(cli->fd, cli->packet, PACKET_LEN);

    if(cli->token && cli->token_size < DATA_SIZE && cli->token_size > 0)
        ret = send_msg(cli->fd, cli->token, cli->token_size);

    if(cli->data_buf && cli->data_size < DATA_SIZE && cli->data_size > 0)
        ret = send_msg(cli->fd, cli->data_buf, cli->data_size);
    return ret;
}

/* 发送数据 */
int send_msg(const int fd, const char *buf, const int len)
{
    const char *tmp = buf;
    int cnt = 0;
    int send_cnt = 0;
    while (send_cnt != len)
    {
        cnt = send(fd, tmp + send_cnt, len - send_cnt, 0);
        if (cnt < 0)
        {
            if (errno == EINTR || errno == EAGAIN)
                continue;
            return ERROR;
        }
        send_cnt += cnt;
    }
    return SUCCESS;
}

/* 接收数据 */
int recv_msg(const int fd, char *buf, const int len)
{
    char *tmp = buf;
    int cnt = 0;
    int read_cnt = 0;
    while (read_cnt != len)
    {
        cnt = recv(fd, tmp + read_cnt, len - read_cnt, 0);
        if (cnt == 0)
        {
            return ERROR;
        }
        else
        {
            if (cnt < 0)
            {
                if (errno == EINTR || errno == EAGAIN)
                {
                    continue;
                }
                return ERROR;
            }
        }
        read_cnt += cnt;
    }
    return SUCCESS;
}


/* 关闭socket */
void close_fd(int fd) 
{
    if (fd)
    {   
#ifdef _WIN32
        closesocket(fd);
#else
        close(fd);
#endif
    }   
}


int send_pipe(char *buf, short cmd, int size, int type)
{
    int ret; 
    set_request_head(buf, 0x0, cmd, size);
    switch(type)
    {    
        case PIPE_QT:
            ret = write(pipe_qt[1], buf, size + HEAD_LEN);
            break;
        case PIPE_TCP:
            ret = write(pipe_tcp[1], buf, size + HEAD_LEN);
            break;
        case PIPE_UDP:
            ret = write(pipe_udp[1], buf, size + HEAD_LEN);
            break;
		case PIPE_EVENT:
            ret = write(pipe_event[1], buf, size + HEAD_LEN);
			break;
    }    
	return ret;
}

int set_network(const char *ip, const char *netmask)
{
	char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};
    sprintf(cmd, "ifconfig eth0 %s netmask %s", ip, netmask);
	
	exec_cmd(cmd, result);
}

#if 0
int create_udp()
{

}
#endif

struct sock_udp create_udp(char *ip, int port, int mreq_flag)
{
    DEBUG("udp create ip %s, port %d", ip, port);
    int fd = -1; 
    int sock_opt = 0;
    struct sockaddr_in send_addr, recv_addr;
    struct sock_udp udp;

    memset(&udp, 0, sizeof(struct sock_udp));

#ifdef _WIN32
    int socklen = sizeof(struct sockaddr_in);
#else
    socklen_t socklen = sizeof(struct sockaddr_in);
#endif
    fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP); 
    if (fd < 0)
    {   
        DEBUG("socket creating err in udptalk");
        return udp;
    }   
    if (NULL != ip) 
    {   
        memset(&send_addr, 0, socklen);
        send_addr.sin_family = AF_INET;
        send_addr.sin_port = htons(port);
        send_addr.sin_addr.s_addr = inet_addr(ip);

        if(mreq_flag)
        {   
            /* 加入组播 */
            struct ip_mreq mreq;
            /* 设置要加入组播的地址 */
            memset(&mreq, 0, sizeof (struct ip_mreq));
            mreq.imr_multiaddr.s_addr = inet_addr(ip);
            /* 设置发送组播消息的源主机的地址信息 */
            mreq.imr_interface.s_addr = htonl (INADDR_ANY);
    
            /* 把本机加入组播地址，即本机网卡作为组播成员，只有加入组才能收到组播消息 */
            if (setsockopt(fd, IPPROTO_IP, IP_ADD_MEMBERSHIP , (char *)&mreq,sizeof (struct ip_mreq)) == -1)
            {
                DEBUG ("setsockopt IP_ADD_MEMBERSHIP error: %s", strerror(errno));
                close_fd(fd);
                return udp;
            }
        }
    }

    sock_opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *)&sock_opt, sizeof(sock_opt)) < 0)
    {
        DEBUG("setsocksock_sock_sock_opt SO_REUSEADDR");
    }

    sock_opt = 521 * 1024; //设置为512K
    if (setsockopt(fd, SOL_SOCKET, SO_SNDBUF, (char *)&sock_opt, sizeof(sock_opt)) == -1)
    {
        DEBUG("IP_MULTICAST_LOOP set fail!");
    }

    sock_opt = 512 * 1024; //设置为512K
    if (setsockopt(fd, SOL_SOCKET, SO_RCVBUF, (char *)&sock_opt, sizeof(sock_opt)) == -1)
    {
        DEBUG("IP_MULTICAST_LOOP set fail!");
    }
#ifdef _WIN32
    sock_opt = 1;
    if (ioctlsocket(fd, FIONBIO, (u_long *)&sock_opt) == SOCKET_ERROR)
    {
        DEBUG("fcntl F_SETFL fail");
    }
#endif
    memset(&recv_addr, 0, sizeof(recv_addr));
    recv_addr.sin_family = AF_INET;
    recv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    recv_addr.sin_port = htons(port);

    /* 绑定自己的端口和IP信息到socket上 */
    if (bind(fd, (struct sockaddr *)&recv_addr, sizeof(struct sockaddr_in)) == -1)
    {
        DEBUG("bind port %d error", port);
        close_fd(fd);
        return udp;
    }
    if (fd)
        udp.fd = fd;
    udp.port = port;
    udp.recv_addr = recv_addr;
    udp.send_addr = send_addr;

    DEBUG("udp.fd %d", udp.fd);
    return udp;
}

int create_tcp()
{
    int fd, sock_opt;
    int keepAlive = 1;    //heart echo open
    int keepIdle = 15;    //if no data come in or out in 15 seconds,send tcp probe, not send ping
    int keepInterval = 3; //3seconds inteval
    int keepCount = 5;    //retry count

    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {
        DEBUG("unable to create socket");
        goto run_out;
    }

    sock_opt = 1;
    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &sock_opt, sizeof(sock_opt)) != 0)
        goto run_out;
#ifdef _WIN32

#else
    if (fcntl(fd, F_SETFD, 1) == -1)
    {
        DEBUG("can't set close-on-exec on server socket!");
        goto run_out;
    }
    if ((sock_opt = fcntl(fd, F_GETFL, 0)) < -1)
    {
        DEBUG("can't set close-on-exec on server socket!");
        goto run_out;
    }
    if (fcntl(fd, F_SETFL, sock_opt | O_NONBLOCK) == -1)
    {
        DEBUG("fcntl: unable to set server socket to nonblocking");
        goto run_out;
    }
    if (setsockopt(fd, SOL_SOCKET, SO_KEEPALIVE, (void *)&keepAlive, sizeof(keepAlive)) != 0)
        goto run_out;
    if (setsockopt(fd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle)) != 0)
        goto run_out;
    if (setsockopt(fd, SOL_TCP, TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval)) != 0)
        goto run_out;
#endif
    return fd;
run_out:
    close_fd(fd);
    return -1;
}


/* 连接服务器 */
int connect_server(int fd, const char *ip, int port, int count)
{
    struct sockaddr_in client_sockaddr;
    memset(&client_sockaddr, 0, sizeof client_sockaddr);

    client_sockaddr.sin_family = AF_INET;
    client_sockaddr.sin_addr.s_addr = inet_addr(ip);
    client_sockaddr.sin_port = htons(port);

    while (connect(fd, (struct sockaddr *)&client_sockaddr, sizeof(client_sockaddr)) != 0)
    {
        DEBUG("connection failed reconnection after 1 seconds");
        sleep(1);
        if (!count--)
        {
            return ERROR;
        }
    }
    return SUCCESS;
}
