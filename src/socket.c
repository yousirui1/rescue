#include "base.h"
#include "socket.h"
#include "client.h"

/* pipe */
int pipe_event[2] = {-1};
int pipe_tcp[2] = {-1};
int pipe_ui[2] = {-1};
int pipe_task[2] = {-1};

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

   
#if 0
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
#endif


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
        case PIPE_UI:
            ret = write(pipe_ui[1], buf, size + HEAD_LEN);
            break;
        case PIPE_TCP:
            ret = write(pipe_tcp[1], buf, size + HEAD_LEN);
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

/* --------- pipe ----------- */
int init_pipe()
{
    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_ui) < 0)
    {   
        DIE("create ui pipe error: %s", strerror(errno));
    }   

    fcntl(pipe_ui[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_ui[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_tcp) < 0)
    {   
        DIE("create tcp pipe error: %s", strerror(errno));
    }   

    fcntl(pipe_tcp[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_tcp[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_event) < 0)
    {   
        DIE("create event pipe error: %s", strerror(errno));
    }   

    fcntl(pipe_event[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_event[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_task) < 0)
    {   
       	DIE("create task pipe error: %s", strerror(errno));
    }   
    fcntl(pipe_task[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_task[1], F_SETFL, O_NONBLOCK);
    return SUCCESS;
}

void close_pipe()
{
    close_fd(pipe_event[0]);
    close_fd(pipe_event[1]);
    close_fd(pipe_tcp[0]);
    close_fd(pipe_tcp[1]);
    close_fd(pipe_ui[0]);
    close_fd(pipe_ui[1]);
    close_fd(pipe_task[0]);
    close_fd(pipe_task[1]);
}


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
/*
 * YZY PACKET 
 * cmd CODE
 * data_type: 0x00 二进制数据, 0x01 JSON, 0x02 protobuf
 * req_flag: 0 主动发送 1 被动应答
 */
void set_packet_head(char *buf, int cmd, int data_size, char data_type, int req_flag)
{
    yzy_packet *packet = (yzy_packet *)buf;
    packet->version_chief = 1;
    packet->version_sub = 0;
    packet->service_code = cmd;
    packet->request_code = 0;
    packet->dataSize = data_size;
    packet->dataType = data_type;
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

int set_packet_token(struct client *cli)
{
    if (cli->token)
        free(cli->token);
    cli->token = malloc(read_packet_token(cli->recv_head) + 1);
    cli->token_size = read_packet_token(cli->recv_head);
    memcpy(cli->token, cli->recv_buf, cli->token_size);

	memcpy(cli->send_head, cli->recv_head, PACKET_LEN);

	return SUCCESS;
}


/*
 * req_flag: 0 主动发送 1 被动应答
 */
int send_packet(struct client *cli, int req_flag)
{
    int ret;
    if (!cli || !cli->fd)
        return ERROR;

	if(req_flag)	//recv
	{
    	ret = send_msg(cli->fd, cli->recv_head, PACKET_LEN);

    	if(cli->token && cli->token_size < DATA_SIZE && cli->token_size > 0)
        	ret = send_msg(cli->fd, cli->token, cli->token_size);

    	if(cli->recv_buf && cli->recv_size > 0)
        	ret = send_msg(cli->fd, cli->recv_buf, cli->recv_size);
	}
	else			//send
	{
    	ret = send_msg(cli->fd, cli->send_head, PACKET_LEN);

    	if(cli->token && cli->token_size < DATA_SIZE && cli->token_size > 0)
        	ret = send_msg(cli->fd, cli->token, cli->token_size);

    	if(cli->send_buf && cli->send_size > 0)
        	ret = send_msg(cli->fd, cli->send_buf, cli->send_size);
	}
    return ret;
}

/* sockfd -> FILE  */
FILE *open_socket(const char *host, int port)
{
	int fd, ret;
	FILE *fp;

	fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd == -1)
    {   
        DEBUG("unable to create socket");
        return NULL;
    }

	DEBUG("host %s port %d", host, port);
    ret = connect_server(fd, host, port, TIME_OUT);
    if(ret != SUCCESS)
    {    
        DEBUG("connect http ip: %s port: %d timeout 10s", host, port);
        close_fd(fd);
        return NULL;
    }    
	
	fp = fdopen(fd, "r+");
	if(!fp)
		return NULL;
	else 
		return fp;
}


#if 0
void set_nport(struct sockaddr *sa, unsigned int port)
{
	if(sa->sa_famliy = AF_INET)
	{
		struct sockaddr_in *sin = (void *)sa;
		sin->sin_port = port;
		return ;
	}
}


/* host "1.2.3.4[:port]", www.baidu.com[:port]" */
static len_and_sockaddr* str2sockaddr(const char *host, int port, int ai_flags)
{
	int rc;
	len_and_sockaddr *r;
	struct addrinfo *result = NULL;
	struct addrinfo *used_res;
	const char *org_host = host;
	const char *cp;
	struct addrinfo hint;
	
	if(is_prefixed_with(host, "local:"))
	{
		struct sockaddr_un *sun;
		r = malloc(LSA_LEN_SIZE + sizeof(struct sockaddr_un));
		r->len = sizeof(struct sockaddr_un);
		r->u.sa.sa_family = AF_UNIX;
		sun = (struct sockaddr_un *)&r->u.sa;
		strncpy(sun->sun_path, host + 6, sizeof(sun->sun_path));
		return r;
	}
	
	r = NULL;

	cp = strrchr(host, ':');
	if(cp && strchr(host, ':') != cp)
	{
		cp = NULL;
	}
	if(cp)	// point to ':' or "]:" 
	{
		int sz = cp - host + 1;
		//host = strncpy(alloca(sz), host, sz);

		cp++;
		port = strtou(cp, NULL, 10);
		if(errno || (unsigned int)port > 0xffff)
		{
			DEBUG("port error %s", org_host);
			return NULL;
		}	
	}

	{
		struct in_addr in4;
		if(inet_aton(host, &in4) != 0)
		{
			r = malloc(LSA_LEN_SIZE + sizeof(struct sockaddr_in));
			r->len = sizeof(struct sockaddr_in);
			r->u.sa.sa_family = AF_INET;
			r->u.sin.sin_addr = in4;
			goto set_port;
		}

	}

	memset(&hint, 0, sizeof(hint));
	hint.ai_famliy = af;
	/* Need sock_stream, or else we get each address thrice 
	 * for each possible socket type (tcp, udp, raw ...); */
	hint.ai_socktype = SOCK_STREAM;
	hint.ai_socktype = SOCK_STREAM;
	rc = getaddrinfo(host, NULL, &hint, &result);
	if(rc || !result)
	{

	}	
	while(1)
	{


	}


set_port:
	set_nport(&r->u.sa, htons(port));
ret:
	if(result)
		freeaddrinfo(result);
	return r;
}
#endif
