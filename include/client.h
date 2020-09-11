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


#endif
