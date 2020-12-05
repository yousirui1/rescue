#ifndef BLOCK_GPT
#define BLOCK_GPT

#ifndef FALSE
#define FALSE (0)
#endif
#ifndef TRUE
#define TRUE  (1)
#endif

typedef unsigned short      CHAR16;
typedef char                CHARN;

#define STR(x) x

#define copy_guid(destguid, srcguid) (memcpy(destguid, srcguid, 16))
#define guids_are_equal(guid1, guid2) (memcmp(guid1, guid2, 16) == 0)

#define GPT_KIND_SYSTEM     (0)
#define GPT_KIND_DATA       (1)
#define GPT_KIND_BASIC_DATA (2)
#define GPT_KIND_FATAL      (3)

#define MAX_MBR_LBA  0xFFFFFFFF


//
// platform-independent types
//

typedef struct {
    uint8_t   flags;
    uint8_t   start_chs[3];
    uint8_t   type;
    uint8_t   end_chs[3];
    uint32_t  start_lba;
    uint32_t  size;
} MBR_PART_INFO;

typedef struct {
    uint8_t   type;
    CHARN   *name;
} MBR_PARTTYPE;

typedef struct {
    uint64_t  signature;
    uint32_t  spec_revision;
    uint32_t  header_size;
    uint32_t  header_crc32;
    uint32_t  reserved;
    uint64_t  header_lba;
    uint64_t  alternate_header_lba;
    uint64_t  first_usable_lba;
    uint64_t  last_usable_lba;
    uint8_t   disk_guid[16];
    uint64_t  entry_lba;
    uint32_t  entry_count;
    uint32_t  entry_size;
    uint32_t  entry_crc32;
} GPT_HEADER;

typedef struct {
    uint8_t   type_guid[16];
    uint8_t   partition_guid[16];
    uint64_t  start_lba;
    uint64_t  end_lba;
    uint64_t  attributes;
    CHAR16  name[36];
} GPT_ENTRY;

typedef struct {
    uint8_t   guid[16];
    uint8_t   mbr_type;
    CHARN   *name;
    uint32_t   kind;
} GPT_PARTTYPE;

typedef struct {
    uint32_t    index;
    uint64_t    start_lba;
    uint64_t    end_lba;
    uint32_t    mbr_type;
    GPT_PARTTYPE *gpt_parttype;
    GPT_ENTRY    gpt_entry;
    uint8_t      active;
} PARTITION_INFO;


typedef struct _DiskDriver {
    int flags;
    void* hd;
    int (*read_sector)(void* hd, uint64_t lba, uint8_t* buffer);
    int (*write_sector)(void* hd, uint64_t lba, uint8_t* buffer);

    uint32_t             mbr_part_count;
    uint32_t             gpt_part_count;
    PARTITION_INFO       mbr_parts[4];
    PARTITION_INFO gpt_parts[18]; //128 不需要这么多
    uint64_t disk_size_lba;
    uint8_t diskName[16];
}DiskDriver;

//
// vars and functions provided by the common lib module
//

int def_read_sector(void* hd, uint64_t lba, uint8_t* buffer);
int def_write_sector(void* hd, uint64_t lba, uint8_t* buffer);

int read_mbr(DiskDriver* pdd);
int read_gpt(DiskDriver* pdd);
int scan_yzy_gpt(DiskDriver* pdd);

#endif
