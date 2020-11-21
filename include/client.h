#ifndef __CLIENT_H__
#define __CLIENT_H__

#define CLIENT_BUF 1024 * 1024
#define IPADDR_LEN 128

struct client
{
    char is_online;
    int fd; 
    
    char ip[IPADDR_LEN];
    int port;   
    
    unsigned char *head_buf;
    unsigned char *packet;
    unsigned char *token;
    
    int token_size;

    int login_flag;

    /** has read msg head or not ,0 :not 1: yes**/
    int has_read_head ;
    
    unsigned char *data_buf;

	unsigned char heartbeat_buf[1024];
	unsigned int heartbeat_len;

    /** current data position **/
    int pos;
    /** curreant data size **/
    int data_size;
    /** max alloc size **/
    int max_size;

    time_t last_time;
};


struct desktop_group
{
	uint8_t diff_mode;
	uint8_t auto_update;
	//uint8_t max_diff;

	char desktop_group_uuid[40];
	char desktop_group_name[128];
	
	char os_uuid[40];
	char data_uuid[40];
	char share_uuid[40]	
};


#endif
