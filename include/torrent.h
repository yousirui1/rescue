#ifndef __TORRENT_H__
#define __TORRENT_H__

#ifdef __cplusplus
extern "C" 
{
#endif

void *thread_torrent(void *param);
int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset);
void stop_torrent();

struct torrent_task{
	char torrent_file[128];
	char file_name[128];
	char uuid[36];
	int diff;
	char group_uuid[36];
	int disk_type;
	uint64_t offset;
};

struct progress_info{
    char file_name[36];
    char state[12];
    unsigned int long progress;
    unsigned long long download_rate;
	unsigned long long upload_rate;
    unsigned long long total_size;		//下载总大小
	unsigned long long file_size;
	
	int type;
	
	char image_name[128];
	char storage[128];
};


typedef struct progress_info progress_info;

#ifdef __cplusplus
}

#endif
#endif

