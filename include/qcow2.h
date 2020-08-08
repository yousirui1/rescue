#ifndef __QCOW2_H__
#define __QCOW2_H__

#define QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#define QCOW_VERSION 2
#define DEFAULT_CLUSTER_SIZE 65536

#define YZY_DISK_TAG "YZYDISK"

#include "StoreConfig.h"

#define YZY_MAX_PATH 256


typedef struct _QCowHeader {
    uint32_t magic;
    uint32_t version;
    uint64_t backing_file_offset;
    uint32_t backing_file_size;
    uint32_t cluster_bits;
    uint64_t size; /* in bytes */
    uint32_t crypt_method;
    uint32_t l1_size; /* XXX: save number of clusters instead ? */
    uint64_t l1_table_offset;
    uint64_t refcount_table_offset;
    uint32_t refcount_table_clusters;
    uint32_t nb_snapshots;
    uint64_t snapshots_offset;
} QCowHeader;

typedef struct _YZY_DISK_BACK_FILE {
    char backing_file[YZY_MAX_PATH]; //必须放到最前面保持兼容
    YZYGUID qcow;
    uint32_t difLevel;
} YZY_DISK_BACK_FILE, * PYZY_DISK_BACK_FILE;






#endif
