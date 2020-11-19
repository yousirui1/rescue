#ifndef __BT_CLIENT_H__
#define __BT_CLIENT_H__

#ifdef __cplusplus
extern "C" 
{
#endif

#include "base.h"
#include "packet.h"

//int start_torrent(char *torrent, char *save_path, char *file_name, uint64_t physical_offset);



//int add_torrent(char *torrent, const char *save_path, char *file_name, uint64_t physical_offset);
int add_torrent(const char *torrent, const char *save_path, char *file_name,  uint64_t physical_offset);

void clear_torrent();
void stop_bt();

//void *thread_bt(void *param);

#ifdef __cplusplus
}
#endif

#endif


