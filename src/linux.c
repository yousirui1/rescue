#include "base.h"
#include "device.h"
#include "xstrtol.h"
#include <scsi/scsi.h>
#include <stdarg.h>


#define PED_SECTOR_SIZE_DEFAULT 512LL

#ifndef S_IFMT
# define S_IFMT 0170000              
#endif   

#define _S_IFBLK  0x6000

#define S_ISBLK(m) (((m) & S_IFMT) == S_IFBLK)


/* from <linux/fs.h> */
#define BLKRRPART  _IO(0x12,95) /* re-read partition table */
#define BLKGETSIZE _IO(0x12,96) /* return device size */
#define BLKFLSBUF  _IO(0x12,97) /* flush buffer cache */
#define BLKSSZGET  _IO(0x12,104) /* get block device sector size */
#define BLKGETLASTSECT  _IO(0x12,108) /* get last sector of block device */
#define BLKSETLASTSECT  _IO(0x12,109) /* set last sector of block device */

/* return device size in bytes (u64 *arg) */
#define BLKGETSIZE64 _IOR(0x12,114,size_t)

struct blkdev_ioctl_param {
        unsigned int block;
        size_t content_length;
        char * block_contents;
};


/* from <linux/major.h> */
#define IDE0_MAJOR              3
#define IDE1_MAJOR              22
#define IDE2_MAJOR              33
#define IDE3_MAJOR              34
#define IDE4_MAJOR              56
#define IDE5_MAJOR              57
#define SCSI_CDROM_MAJOR        11
#define SCSI_DISK0_MAJOR        8
#define SCSI_DISK1_MAJOR        65
#define SCSI_DISK2_MAJOR        66
#define SCSI_DISK3_MAJOR        67
#define SCSI_DISK4_MAJOR        68
#define SCSI_DISK5_MAJOR        69
#define SCSI_DISK6_MAJOR        70
#define SCSI_DISK7_MAJOR        71
#define SCSI_DISK8_MAJOR        128
#define SCSI_DISK9_MAJOR        129
#define SCSI_DISK10_MAJOR       130
#define SCSI_DISK11_MAJOR       131
#define SCSI_DISK12_MAJOR       132
#define SCSI_DISK13_MAJOR       133
#define SCSI_DISK14_MAJOR       134
#define SCSI_DISK15_MAJOR       135
#define COMPAQ_SMART2_MAJOR     72
#define COMPAQ_SMART2_MAJOR1    73
#define COMPAQ_SMART2_MAJOR2    74
#define COMPAQ_SMART2_MAJOR3    75
#define COMPAQ_SMART2_MAJOR4    76
#define COMPAQ_SMART2_MAJOR5    77
#define COMPAQ_SMART2_MAJOR6    78
#define COMPAQ_SMART2_MAJOR7    79
#define COMPAQ_SMART_MAJOR      104
#define COMPAQ_SMART_MAJOR1     105
#define COMPAQ_SMART_MAJOR2     106
#define COMPAQ_SMART_MAJOR3     107
#define COMPAQ_SMART_MAJOR4     108
#define COMPAQ_SMART_MAJOR5     109
#define COMPAQ_SMART_MAJOR6     110
#define COMPAQ_SMART_MAJOR7     111
#define DAC960_MAJOR            48
#define ATARAID_MAJOR           114
#define I2O_MAJOR1              80
#define I2O_MAJOR2              81
#define I2O_MAJOR3              82
#define I2O_MAJOR4              83
#define I2O_MAJOR5              84
#define I2O_MAJOR6              85
#define I2O_MAJOR7              86
#define I2O_MAJOR8              87
#define UBD_MAJOR               98
#define DASD_MAJOR              94
#define VIODASD_MAJOR           112
#define AOE_MAJOR               152
#define SX8_MAJOR1              160
#define SX8_MAJOR2              161
#define XVD_MAJOR               202
#define SDMMC_MAJOR             179
#define LOOP_MAJOR              7
#define MD_MAJOR                9
#define BLKEXT_MAJOR            259
#define RAM_MAJOR               1

#define SCSI_BLK_MAJOR(M) (                                             \
                (M) == SCSI_DISK0_MAJOR                                 \
                || (M) == SCSI_CDROM_MAJOR                              \
                || ((M) >= SCSI_DISK1_MAJOR && (M) <= SCSI_DISK7_MAJOR) \
                || ((M) >= SCSI_DISK8_MAJOR && (M) <= SCSI_DISK15_MAJOR))

#define KERNEL_VERSION(a,b,c) (((a) << 16) + ((b) << 8) + (c))


/* from <linux/hdreg.h> */
#define HDIO_GETGEO             0x0301  /* get device geometry */
#define HDIO_GET_IDENTITY       0x030d  /* get IDE identification info */


#define RD_MODE (O_RDONLY)
#define WR_MODE (O_WRONLY)
#define RW_MODE (O_RDWR)

struct hd_geometry {
        unsigned char heads;
        unsigned char sectors;
        unsigned short cylinders;
        unsigned long start;
};


int _mount_table_search(const char* file_name, dev_t dev)
{
	struct stat part_stat;
	char line[512];
	char part_name[512];
	FILE *file;
	
	file = fopen(file_name, "r");
	if(!file)
		return 0;
	while(fgets(line, 512, file))
	{
		if(sscanf(line, "%s", part_name) == 1 
			&& stat(part_name, &part_stat) == 0)
		{
			if(part_stat.st_rdev == dev)
			{
				fclose(file);
				return 1;
			}
		}
	}	
	fclose(file);
	return 0;	
}



static int _partition_is_mounted_by_dev (dev_t dev)
{
        return  _mount_table_search( "/proc/mounts", dev)
                || _mount_table_search( "/proc/swaps", dev)
                || _mount_table_search( "/etc/mtab", dev);
}


static int _partition_is_mounted_by_path(const char *path)
{
	struct stat part_stat;
	if(stat(path, &part_stat) != 0)
		return 0;
	//if(
	return _partition_is_mounted_by_dev (part_stat.st_rdev);
}


/* we need to flush the master device , and all the partition */
static int _device_stat(PedDevice *dev, struct stat *dev_stat)
{
	assert(dev != NULL);
	assert(!dev->external_mode);

	for(;;)
	{
		if(!stat(dev->path, dev_stat))
			return 1;
		else
		{
			DEBUG("Could not stat device %s - %s.", dev->path, strerror(errno));		
			return 0;
		}
	}	
}

static void _device_set_sector_size(PedDevice *dev)
{
	int sector_size;
	
	dev->sector_size = PED_SECTOR_SIZE_DEFAULT;
	dev->phys_sector_size = PED_SECTOR_SIZE_DEFAULT;

	assert(dev->open_count);

	if(ioctl(dev->fd, BLKSSZGET, &sector_size))
	{
		DEBUG("Could not determine sector size for %s: %s.", dev->path, strerror (errno));
	}
	else
	{
		dev->sector_size = (long long)sector_size;
		dev->phys_sector_size = dev->sector_size;
	}	
}

static int _get_linux_version ()
{
        static int kver = -1;

        struct utsname uts;
        int major = 0;
        int minor = 0;
        int teeny = 0;

        if (kver != -1)
                return kver;

        if (uname (&uts))
                return kver = 0;
        int n = sscanf (uts.release, "%u.%u.%u", &major, &minor, &teeny);
        assert (n == 2 || n == 3);
        return kver = KERNEL_VERSION (major, minor, teeny);
}



static int _kernel_has_blkgetsize64(void)
{
	int version = _get_linux_version();
	
    if (version >= KERNEL_VERSION (2,5,4)) return 1;
    if (version <  KERNEL_VERSION (2,5,0) &&
        version >= KERNEL_VERSION (2,4,18)) return 1;
    return 0;
}


static PedSector _device_get_length(PedDevice *dev)
{
	unsigned long size;
	uint64_t bytes = 0;
	const char *test_str;
	PedSector test_size;
	
	assert(dev->open_count > 0);
	assert(dev->sector_size % PED_SECTOR_SIZE_DEFAULT == 0);
	
	test_str = getenv("PARTED_TEST_DEVICE_LENGTH");
		
	//if(test_str && __xstrtol (test_str, NULL, 10, &test_size, NULL) == LONGINT_OK)
	//	return test_size;
	if(_kernel_has_blkgetsize64())
	{
		if(ioctl(dev->fd, BLKGETSIZE64, &bytes) == 0)
			return bytes / dev->sector_size;
	}
	if(ioctl(dev->fd, BLKGETSIZE, &size))
	{
		DEBUG("Unable to determine the size of %s (%s).", dev->path, strerror(errno));
		return 0;
	}	
	return size;	
}

static int _device_probe_geometry(PedDevice *dev)
{
	struct stat dev_stat;
	struct hd_geometry geometry;
	
	if(!_device_stat(dev, &dev_stat))
		return 0;
	
	//assert(S_ISBLK(dev_stat.st_mode));

	_device_set_sector_size(dev);
	
	dev->length = _device_get_length(dev);
	if(!dev->length)
		return 0;
	
	/* The GETGEO ioctl is no longer useful (as of linux 2.6.x). We could
	 * still use it in 2.4.x, but this is contentious, Perhaps we should 
	 * move to EDD */
	dev->bios_geom.sectors = 63;
	dev->bios_geom.heads = 255;
	dev->bios_geom.cylinders = dev->length / (63 * 255);
	
	/* FIXME: what should we put here ? (TODO: discuss on linux-kernel) */
	
	if(!ioctl(dev->fd, HDIO_GETGEO, &geometry) 
			&& geometry.sectors && geometry.heads)
	{
		dev->hw_geom.sectors = geometry.sectors;
		dev->hw_geom.heads = geometry.heads;
		dev->hw_geom.cylinders = dev->length / (dev->hw_geom.heads * dev->hw_geom.sectors);
	}
	else
	{
		dev->hw_geom = dev->bios_geom;	
	}									
	
	return 1;
}
static char *zasprintf(const char *format, ...)
{
  	va_list args;
  	char *resultp;
  	va_start (args, format);
  	int r = vasprintf (&resultp, format, args);
  	va_end (args);
  	return r < 0 ? NULL : resultp;
}


static char *_device_get_part_path(PedDevice *dev, int num)
{
	size_t path_len = strlen(dev->path);
	char *result;
	/* 
	 */
	if(5 < path_len && !strcmp (dev->path + path_len - 5, "/disc"))
	{
	  	/* replace /disc with /part%d */
    	result = zasprintf ("%.*s/part%d",
                  (int) (path_len - 5), dev->path, num);

	}
	else
	{
		char const *p = (dev->type == PED_DEVICE_DAC960
                                 || dev->type == PED_DEVICE_CPQARRAY
                                 || dev->type == PED_DEVICE_ATARAID
                                 || isdigit (dev->path[path_len - 1])
                                 ? "p" : ""); 
                result = zasprintf ("%s%s%d", dev->path, p, num);
	}
	return result;
}

static _device_seek(const PedDevice* dev, PedSector sector)
{
	assert(dev->sector_size % PED_SECTOR_SIZE_DEFAULT == 0);
	assert(dev != NULL);
	assert(!dev->external_mode);
	
	off_t pos = sector * dev->sector_size;
	return lseek(dev->fd, pos, SEEK_SET) == pos;	
}


void linux_destroy(PedDevice *dev)
{
	void *p = dev->dmtype;

	free(p);
	free(dev->path);
	free(dev->model);
	free(dev);	
}



int linux_is_busy(PedDevice *dev)
{
	int i;
	char *part_name;

	if(_partition_is_mounted_by_path(dev->path))
		return 1;
	for(i = 0; i < 32; i++)
	{
		int status;
	
		part_name = _device_get_part_path(dev, i);
		if(!part_name)
			return 1;
		status = _partition_is_mounted_by_path(part_name);
		free(part_name);
		if(status)
			return 1;
	}
	return 0;
}


int linux_read(const PedDevice *dev, void *buffer, PedSector start, PedSector count)
{
	void *diobuf = NULL;
	
	assert(dev != NULL);
	assert(dev->sector_size % PED_SECTOR_SIZE_DEFAULT == 0);

	if(!_device_seek(dev, start))
		return 1;
	
	size_t read_length = count * dev->sector_size;
	if(posix_memalign (&diobuf, dev->sector_size, read_length) != 0)
		return 1;

	for(;;)
	{
		ssize_t status = read(dev->fd, diobuf, read_length);
		if(status > 0)
			memcpy(buffer, diobuf, status);
		if(status == (ssize_t)read_length)
			break;
		if(status > 0)
		{
			read_length -= status;
			buffer = (char *)buffer + status;
			continue;
		}
	}
	
	free(diobuf);
	return 0;
}

int linux_write(PedDevice *dev, const void *buffer, PedSector start, PedSector count)
{
	void *diobuf;
	void *diobuf_start;
	
	assert(dev->sector_size % PED_SECTOR_SIZE_DEFAULT == 0);
			
	if(dev->read_only)
	{
		return 1;
	}

	if(!_device_seek(dev, start))
		return 1;			
	
	size_t write_length = count * dev->sector_size;
	dev->dirty = 1;
			
	if(posix_memalign(&diobuf, dev->sector_size, write_length) != 0)
		return 1;
	
	memcpy(diobuf, buffer, write_length);
	diobuf_start = diobuf;
	for(;;)
	{
		ssize_t status = write(dev->fd, diobuf, write_length);
		if(status == write_length)
			break;
		if(status > 0)
		{
			write_length -= status;
			diobuf = (char *)diobuf + status;
			continue;
		}		
		if(status < 0)
		{
	        DEBUG("disk write error");
            break;
		}
	}
	free(diobuf_start);
	return 0;
}


int linux_close(PedDevice *dev)
{
	if(dev->dirty)
		;//_flush_cache(dev);
	
retry:
	if(fsync(dev->fd) < 0 || close(dev->fd) < 0)
		goto retry;
	return 1;		
}


int linux_open(PedDevice *dev)
{
	dev->fd = open(dev->path, RW_MODE);
	if(dev->fd == -1)
	{
		DEBUG("Error opening %s: %s", dev->path, strerror(errno));
		return 0;
	}
	else
	{
		dev->read_only = 0;
	}
	dev->open_count = 1;
	return 1;
}


PedDevice *linux_new(const char *path)
{
	PedDevice *dev;
	
	assert(path != NULL);
	
	dev = (PedDevice *)malloc(sizeof(PedDevice));
	memset(dev, 0, sizeof(PedDevice));
	if(!dev)
		goto error;
	dev->path = strdup(path);
	if(!dev->path)
		goto error_free_dev;
	
#if 0
	dev->arch_specific = (LinuxSpecific *)ped_malloc(sizeof(LinuxSpecific));
		
	if(!dev->arch_specific)
		goto error_free_path;
#endif
		
	//arch_specific = LINUX_SPECIFIC(dev);
	//arch_specific->dmtype = NULL;
#if USE_BLKID
	//arch_specific->probe = NULL;
	//arch_specific->topology = NULL;
#endif		
	dev->open_count = 0;
	dev->read_only = 0;
	dev->external_mode = 0;
	dev->dirty = 0;
	dev->boot_dirty = 0;

	if(!linux_open(dev))
		goto error_free_path;
	_device_probe_geometry(dev);

#ifdef ENABLE_DEVICE_MAPPER
	//dm_udev_set_sync_support(1);
#endif
#if 0
	if(!_device_probe_type(dev))
		goto error_free_path;
		//goto error_free_arch_specific;
	
	switch(dev->type)
	{
		case PED_DEVICE_IDE:
			DEBUG("IDE -------------");
			//if(!init_ide(dev))
				//goto error_free_arch_specific;
			break;
		case PED_DEVICE_SCSI:
			DEBUG("PED_DEVICE_SCSI");
			if(!init_scsi(dev))
				goto error_free_path;
			break;
		default:
			DEBUG("dev type noknow ----");
			break;
	}	
#endif
	return dev;

error_free_path:
	DEBUG("error_free_path %s", dev->path);
	free(dev->path);

error_free_dev:
	DEBUG("error_free_dev");
	free(dev);
error:
	return NULL;
}


int read_sector(PedDevice const *dev, PedSector start, void **buf)
{
	char *b = (char *) malloc(dev->sector_size);
	assert(b != NULL);
	assert(dev != NULL);
	assert(!dev->external_mode);
	assert(dev->open_count > 0);
	
	if(linux_read(dev, b, start, 1))
	{
		free(b);
		return 0;
	}
	*buf = b;
	return 1;
}




#if 0
static PedDeviceArchOps linux_dev_ops = {
	_new:			linux_new,
	destroy:		linux_destroy,
	is_busy:		linux_is_busy,
	open:			linux_open,
	refresh_open:	linux_refresh_open,
	close:			linux_close,
	refresh_close:	linux_refresh_close,
	read:			linux_read,
	write:			linux_write,
	check:			linux_check,
	sync:			linux_sync,
	sync_fast:		linux_sync_fast,
	probe_all:		linux_probe_all,
#if defined __s390__ || defined __s390x__
	//get_minimum_alignment:	s
	//
#elif USE_BLKID
	get_minimum_alignment:	linux_get_minimum_alignment,
	get_optimum_alignment:	linux_get_optimum_alignment,
#endif
};
#endif
