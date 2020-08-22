#ifndef __TFTP_H__
#define __TFTP_H__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <dirent.h>


#define CMD_RRQ (short)1
#define CMD_WRQ (short)2
#define CMD_DATA (short)3
#define CMD_ACK (short)4
#define CMD_ERROR (short)5
#define CMD_LIST (short)6
#define CMD_HEAD (short)7


// Without a '/' at the end.
char *conf_document_root;

#define SERVER_PORT 10220
// Max request datagram size
#define MAX_REQUEST_SIZE 1024
// TFTPX_DATA_SIZE
#define DATA_SIZE 512
//
#define LIST_BUF_SIZE (DATA_SIZE * 8)


// Max packet retransmission.
#define PKT_MAX_RXMT 3
// usecond
#define PKT_SEND_TIMEOUT 12*1000*1000
#define PKT_RECV_TIMEOUT 3*1000*1000
// usecond
#define PKT_TIME_INTERVAL 5*1000

struct tftp_packet{
    unsigned short cmd;
    union{
        unsigned short code;
        unsigned short block;
        // For a RRQ and WRQ TFTP packet
        char file_name[2];
    };
    char data[DATA_SIZE];
};
#if 0
struct tftp_request{
    int size;
    struct sockaddr_in client;
    struct tftp_packet packet;
};
#endif

#endif
