
#define QCOW_MAGIC (('Q' << 24) | ('F' << 16) | ('I' << 8) | 0xfb)
#define QCOW_VERSION 2
#define DEFAULT_CLUSTER_SIZE 65536
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

#define YZY_DISK_TAG "YZYDISK"

static int qcow_change_backing_file(yzy_string path, yzy_string backing_file, uint32_t len, uint64_t startOffset)
{
    int ret;
    yzy_file_t fd;

    if (backing_file && len > 1023) {
        return -1;
    }

    fd = yzy_file_open(path, yzy_O_RDWR);
    if (fd < 0)
    {
        return -1;
    }
    char* buf = yzy_Malloc(DEFAULT_CLUSTER_SIZE);
    if (!buf) return -1;
    QCowHeader* header = (QCowHeader*)buf;

    ret = yzy_file_read(fd, 0 + startOffset, buf, DEFAULT_CLUSTER_SIZE);
    if (ret < 0)
    {
        yzy_Free(buf);
        return -1;
    }

    if (be32_to_cpu(header->magic) != QCOW_MAGIC || be32_to_cpu(header->version) < QCOW_VERSION)
    {
        yzy_Free(buf);
        return -1;
    }

    be64_to_cpus(&header->backing_file_offset);
    be32_to_cpus(&header->backing_file_size);

    memcpy(&buf[header->backing_file_offset], backing_file, len);
    buf[header->backing_file_offset + len] = '\0';


    be64_to_cpus(&header->backing_file_offset);
    header->backing_file_size = cpu_to_be32(len);

    ret = yzy_file_write(fd, 0 + startOffset, buf, DEFAULT_CLUSTER_SIZE);
    yzy_Free(buf);
    if (ret < 0) return -1;
    return 0;
}


int ChangeBackFile(PDISK_HANDLE_ITEM path, PYZY_QCOW_ENTRY pCurrent, PYZY_QCOW_ENTRY pBacking)
{
    YZY_DISK_BACK_FILE dbf = { 0 };
    memcpy(dbf.backing_file, YZY_DISK_TAG, sizeof(YZY_DISK_TAG));
    dbf.difLevel = pBacking->difLevel;
    dbf.qcow = pBacking->uuQcow;

    return qcow2.change_backing_file(path, &dbf, sizeof(YZY_DISK_BACK_FILE), storeDrv.getQcowStartLba(pCurrent) * FILESECBYTES);
}
