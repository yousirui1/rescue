#ifndef BLOCK_CONFIG
#define BLOCK_CONFIG

//#include "file.h"

#define YZY_STORE_TAG "yzyVOI"
#define YZY_MAX_STORE_QCOW_ENTRY 20 //两K可存放的条目数
#define YZY_MAX_STORE_BUFF_LEN 2048
#define YZY_MAX_STORE_NAME_LEN 24
#define YZY_MAX_DISK_COUNT 10

#define yzy_file_t  void *

#define MAXINT64 0x7FFFFFFFFFFFFFFF

#define FILESECBYTES 1

#pragma pack(push)
#pragma pack(1)
//
typedef struct _yzyGUID
{
    uint32_t Data1;
    uint16_t Data2;
    uint16_t Data3;
    uint64_t Data4;
} YZYGUID, *PYZYGUID;

typedef struct _YZY_DISK_ITEM
{
    uint64_t diskSizeLba;
    YZYGUID diskName;
} YZY_DISK_ITEM;

typedef struct _YZY_QCOW_ENTRY
{
    uint8_t     type;   
    uint8_t     isDel; //删除标志，还原模式记得创建即删除
    uint8_t     stype;   
    uint8_t     isReduction;
    uint32_t    difLevel; //差异文件层数 0是base
    YZYGUID     diskName; // 多硬盘支持,磁盘GUID，用来查找当前节点数据所在物理磁盘,找不到默认为单磁盘
    YZYGUID     uuQcow;         //QCOW GUID   
    uint64_t    startLba; //qcow起始位置
    uint64_t    endLba;  //qcow 按保留的空间结束位置，正常按容量，也可以指定大小
    uint64_t    realLba;  //qcow容量
	uint8_t 	flag;
	uint8_t		R1;
	uint16_t	R2;
	uint32_t	R3;
    uint64_t    R4;          //保留
} YZY_QCOW_ENTRY, *PYZY_QCOW_ENTRY;

typedef struct _YZY_STORE_CONFIG
{
    char                tag[8]; // "yzyVOI"   
    uint32_t            crc;     //crc校验码
    uint32_t            qcowCount; //qcow条目数
    YZYGUID             currBoot;       //当前启动盘 GUID
    YZYGUID             currData;       //当前数据盘 GUID 
    YZYGUID             shareData;      //共享数据盘 GUID
    uint32_t            bootLevel;
    uint32_t            dataLevel; 
    uint32_t            shareLevel; 
    uint8_t             isEditMode;  //是否为编辑模式
    uint8_t             r0; 
    uint16_t            r1; 

    YZY_DISK_ITEM       diskItem[YZY_MAX_DISK_COUNT]; //物理硬盘大小
    uint64_t            serverIP;
    uint64_t            clientIP;
    uint64_t            netmask;
    uint64_t            gateway;
    uint64_t            dns1;
    uint64_t            dns2;
    uint8_t             domain[32];
    uint8_t             computerName[32];
    struct _YZY_QCOW_ENTRY   entry[];  //一直存到168扇区，除第一个外每个扇区保存 个
}YZY_STORE_CONFIG,*PYZY_STORE_CONFIG;

#pragma pack(pop)

typedef struct _StoreDriver
{
    uint64_t storeLba;           //配置数据全局位置偏移
    PYZY_STORE_CONFIG pStoreCfg; //配置表头

    void (*setLba)(uint64_t lba);                //传入上一个分区END_LBA
    uint64_t (*getQcowLba)(PYZY_QCOW_ENTRY pQe); //本节点实际起始位置
    int (*init)();
    int (*del)(uint32_t difLevel, PYZYGUID name); // 注意！！！！ 删除节点后会重建，所有指针失效，需要重新SCAN
    PYZY_QCOW_ENTRY(*scan)
    (uint32_t difLevel, PYZYGUID name);
    int (*alloc)(uint32_t difLevel, PYZYGUID name, PYZYGUID diskName, uint64_t sizeLba, uint64_t realLba, int type, PYZY_QCOW_ENTRY *ppQe);
    int (*load)(yzy_file_t hd); //加载失败后需要init
    int (*save)(yzy_file_t hd);
    void (*rebuild)(); //在删除节点后，需要进行重建，重建后所有指针失效，需要重新scan
    int (*setDiskSizeLba)(PYZYGUID diskName, uint64_t size_lba);
    uint64_t (*getDiskSizeLba)(PYZYGUID diskName);
    int (*addDisk)(PYZYGUID diskName, uint64_t size_lba); /* 返回值 >=0 成功值为当前下标  -2 已存在,会更新size  -1已满 添加失败 */
    void (*setBoot)(int idx, PYZY_QCOW_ENTRY pQe);        //idx: 0系统盘，1数据盘，2共享盘   leve 设置为-1表示当前不使用
} StoreDriver;

extern StoreDriver storeDrv;

int yzy_CompareGuid(PYZYGUID guid1, PYZYGUID guid2);
#endif
