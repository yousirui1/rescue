#ifndef __TASK_H__
#define __TASK_H__

#define MAX_VIDSBUFSIZE 1024 * 1024 * 3

struct p2v_task{
    char server_ip[32];
    char user[32];  
    char passwd[32];    
    char storage[32];
    char image_name[128];
	char template_name[128];
};

struct tftp_task {
	char file_name[128];	
    char server_ip[32];
    char remote_file[128];
    char local_file[128];
    char type;
};


#endif
