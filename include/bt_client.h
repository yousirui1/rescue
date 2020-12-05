#ifndef __BT_CLIENT_H__
#define __BT_CLIENT_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "base.h"

int start_torrent(char *torrent, char *save_path, char *file_name, int diff_mode, uint64_t physical_offset);

void stop_torrent();

#ifdef __cplusplus
}
#endif

#endif
