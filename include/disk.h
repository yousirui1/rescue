#ifndef __DISK_H__
#define __DISK_H__

enum _PedPartitionType {
	PED_PARTITION_NORMAL		= 0x0,
    PED_PARTITION_LOGICAL           = 0x01,
    PED_PARTITION_EXTENDED          = 0x02,
    PED_PARTITION_FREESPACE         = 0x04,
    PED_PARTITION_METADATA          = 0x08,
    PED_PARTITION_PROTECTED         = 0x10
};


typedef struct _PedDisk PedDisk;
typedef struct _PedDiskType PedDiskType;
typedef struct _PedPartition PedPartition;
typedef enum _PedPartitionType PedPartitionType;




struct _PedDisk {
	PedDevice *dev;					//the device where the partition table lies
	const PedDiskType *type;		//type of disk label
	const int *block_sizes;			//block sizes supported by this label
	
	PedPartition *part_list;		//list of partitions Access with ped_disk_next_partition()
	
	void *disk_specific;	
	
	int needs_clobber;				//clobber before write
	int update_mode;				//mode without free/metadata partitions, for easier update
};

struct _PedPartition 
{
	PedPartition *prev;
	PedPartition *next;
	
	/* the partition table of the partition */
	PedDisk *disk;
	//PedGeometry geom;			//geometry of the partition

	/* the partition number: In Linux this is the
	 *
	 */
	int num;
	PedPartitionType type;

//	const PedFileSystemType *fs_type;

	PedPartition *part_list;
	
	void *disk_specific;
};


struct _PedDiskType {
	//PedDiskType *next;
	const char *name;
	//PedDiskOps *const ops;
	//PedDiskTypeFeature features
};

#endif //__DISK_H__
