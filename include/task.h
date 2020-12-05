#ifndef __TASK_H__
#define __TASK_H__

#define MAX_TASKBUFSIZE 1024 * 1024 * 3

typedef enum TASK_TYPE
{
	TASK_BT = 1,
	TASK_P2V,
	TASK_TFTP,
}TASK_TYPE;

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

struct torrent_task {
	char torrent_file[128];
	char file_name[128];
	char uuid[36];
	int diff;
	int diff_mode;	

	unsigned long long real_size;
	unsigned long long space_size;
	unsigned long long file_size;	
	
	int operate_id;

	int disk_type;
};

struct desktop_task {
	char group_name[128];
	char group_uuid[36];			
	int diff_mode;
	uint64_t total_size;
	uint64_t download_size;
	int size;	
	int offset;
	struct torrent_task bt[8];
};


#endif
