#include "base.h"
#include "device.h"
#include "geom.h"
#include "disk.h"

PedDisk *_ped_disk_alloc(const PedDevice *dev, const PedDiskType *disk_type)
{
	PedDisk *disk;
	disk = (PedDisk *)malloc(sizeof(PedDisk));
	
	if(!disk)
		goto error;

	disk->dev = (PedDevice *)dev;
	disk->type = disk_type;
	disk->update_mode = 1;
	disk->part_list = NULL;

	return disk;
error:
	return NULL;
}

void _ped_disk_free(PedDisk *disk)
{
	//_disk_push_update_mode();
	//ped_disk_delete_all(disk);
	free(disk);
}

