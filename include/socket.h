#ifndef __SOCKET_H__
#define __SOCKET_H__

#define PACKET_LEN_OFFSET 12
#define PACKET_ORDER_OFFSET 4
#define PACKET_TOKEN_OFFSET 20

#define DATA_SYN 0xFF
#define DATA_SYN_OFFSET 0
#define DATA_ENCRYPT 1
#define DATA_ORDER_OFFSET 2
#define DATA_LEN_OFFSET 4

#define DATA_SIZE 1452
#define HEAD_LEN 	8
#define PACKET_LEN 24

/* socket */
struct sock_udp
{
    unsigned int fd; 
    unsigned int port;
    
    struct sockaddr_in recv_addr;
    struct sockaddr_in send_addr;
};

typedef struct _req_head
{
    unsigned char syn;
    unsigned char encrypt_flag;
    unsigned short cmd;
    unsigned int data_size;
}req_head;


struct sock_udp create_udp(char *ip, int port, int mreq_flag);


/* -----pipe---------- */
typedef enum PIPE_TYPE {
    PIPE_TCP = 0,
    PIPE_UDP,
    PIPE_QT,
    PIPE_EVENT,
}PIPE_TYPE;

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
}PIPE_MSG;


#endif
