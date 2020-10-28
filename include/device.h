#ifndef __DEVICE_H__
#define __DEVICE_H__

/* We can address 2^63 sectors */
typedef long long PedSector;

typedef enum {
        PED_DEVICE_UNKNOWN      = 0,
        PED_DEVICE_SCSI         = 1,
        PED_DEVICE_IDE          = 2,
        PED_DEVICE_DAC960       = 3,
        PED_DEVICE_CPQARRAY     = 4,
        PED_DEVICE_FILE         = 5,
        PED_DEVICE_ATARAID      = 6,
        PED_DEVICE_I2O          = 7,
        PED_DEVICE_UBD          = 8,
        PED_DEVICE_DASD         = 9,
        PED_DEVICE_VIODASD      = 10, 
        PED_DEVICE_SX8          = 11, 
        PED_DEVICE_DM           = 12, 
        PED_DEVICE_XVD          = 13, 
        PED_DEVICE_SDMMC        = 14, 
        PED_DEVICE_VIRTBLK      = 15, 
        PED_DEVICE_AOE          = 16, 
        PED_DEVICE_MD           = 17, 
        PED_DEVICE_LOOP         = 18, 
        PED_DEVICE_NVME         = 19, 
        PED_DEVICE_RAM          = 20, 
        PED_DEVICE_PMEM         = 21
} PedDeviceType;

typedef struct _PedDevice PedDevice;
typedef struct _PedCHSGeometry PedCHSGeometry;


/*
 * A cylinder-head-sector "old-style" geometry
 * 
 * A device addressed in this way has C*H*S sectors.
 */
struct _PedCHSGeometry{
    int cylinders;
    int heads;
    int sectors;
};


/* A block device - for exxample , /dev/hda, not /dev/hda3 */
struct _PedDevice {
    PedDevice *next;

    char *model;                //brief description of hardware
                                // manufacturer, model

    char *path;                 //device /dev entry

    PedDeviceType type;         //SCSI IDE etc sa PedDeviceType
    long long sector_size;      //logical sector size
    long long phys_sector_size; //physical sector size
    PedSector length;           //device length (LBA) 

    int open_count;             //the number of times this device has
                                //been opened with ped_device_open();   
    int read_only;
    int external_mode;
    int dirty;
    int boot_dirty;

    PedCHSGeometry hw_geom;
    PedCHSGeometry bios_geom;
    short host, did;

    //void *    
    int fd;
    int major;
    int minor;

    char *dmtype;

	uint8_t disk_name[16];
	//uint64_t startLba;	
	uint8_t boot_flag;		
    //bl
};

typedef enum disk_type {
	DISK_TYPE_HD,
	DISK_TYPE_USB,
};


struct disk_param
{
    unsigned char name[16];
    unsigned char disk_type;        // 2 硬盘 3 u 盘 
    unsigned int total_space;       // 总大小 kb
    unsigned int partition;     	// 1 gpt 0 mgr
	unsigned char disk_ready;		// 检测是否有yzy 结构体crc校验 和tag	
    PedDevice *dev;
};

typedef struct disk_param disk_param;

struct device_info
{
    /* disk */
    int disk_count;
	//int mini_disk;
	//int usb_disk;
    disk_param *mini_disk;
    disk_param *usb_disk;
	disk_param *boot_disk;

    /* netcard */
    netcard_param *net;
    int netcard_count;
    disk_param disks[10];
};


#endif
