#ifndef __BT_CLIENT_H__
#define __BT_CLIENT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "base.h"

 int bt_client();

int add_torrent(char *torrent, char *save_path, uint64_t physical_offset);

//int start_torrent(char *torrent, char *save_path, char *file_name, int diff_mode, uint64_t physical_offset);
int start_torrent(char *torrent, char *pipe_buf, int diff_mode);

void stop_torrent();

//int add_torrent(const char *torrent, const char *save_path, char *file_name, uint64_t physical_offset);
//int del_torrent();


#ifdef __cplusplus
}
#endif

#endif
