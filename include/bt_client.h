#ifndef __BT_CLIENT_H__
#define __BT_CLIENT_H__

#ifdef __cplusplus
extern "C" 
{
#endif

#include "base.h"
#include "packet.h"

void clear_torrent();
int start_torrent(char *torrent, char *save_path, char *file_name, int diff_mode, uint64_t physical_offset);

//int add_torrent(char *torrent, const char *save_path, char *file_name, uint64_t physical_offset);
//int add_torrent(const char *torrent, const char *save_path, char *file_name,  uint64_t physical_offset);
void bt_client();

void stop_torrent();

//void *thread_bt(void *param);

#ifdef __cplusplus
}
#endif

#endif


