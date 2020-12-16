#ifndef __CLIENT_H__
#define __CLIENT_H__

#define CLIENT_BUF 1024 * 1024

#include "task.h"

struct client
{
    int fd; 
	int online;

    /** has read msg head or not ,0 :not 1: yes**/
    int has_read_head;

    unsigned char send_head[PACKET_LEN + 1];
    unsigned char recv_head[PACKET_LEN + 1];

	unsigned char *send_buf;
    unsigned char *recv_buf;

    unsigned int send_size;
	unsigned int recv_size;

	int token_size;
	unsigned char *token;

    /* current data position */
    int pos;
    /* curreant data size */
    /* max alloc size */
    int max_size;

    time_t last_time;
};

struct desktop_group
{
    uint8_t diff_mode;
    uint8_t auto_update;
	uint8_t default_desktop_flag;
    //uint8_t max_diff;

    char group_uuid[40];
    char group_name[128];
    
    char os_uuid[40];
    char data_uuid[40];
    char share_uuid[40];

	//struct desktop_task task;
};


#endif
