#include "base.h"
#include "device.h"
#include "gpt.h"
#include "StoreConfig.h"
#include "qcow2.h"

int str2uuid(char *uuid, PYZYGUID GUID)
{
    uint64_t data4_1 = 0;
    uint64_t data4_2 = 0;

    int ret = sscanf(uuid, "%x-%x-%x-%x-%llx",
        &GUID->Data1, &GUID->Data2, &GUID->Data3, &data4_1,
        &data4_2);
    GUID->Data4 = (data4_1 << 48) + data4_2;
    return ret;
}

int uuid2str(PYZYGUID GUID, char* uuid)
{
    uint64_t data4_1 = (GUID->Data4 >> 48);
    uint64_t data4_2 = (GUID->Data4 & 0xffffffffffff);

    int ret = sprintf(uuid, "%08x-%04x-%04x-%04x-%012llx",
        GUID->Data1, GUID->Data2, GUID->Data3, data4_1, data4_2);
    return ret;
}


int check_qcow2(PedDevice *dev)
{
    DiskDriver dd, *pdd;

    pdd = &dd;
    memset(pdd, 0, sizeof(DiskDriver)); 
    pdd->hd = (void *)(dev);

	if(read_mbr(pdd) != SUCCESS)
		return ERROR;

	if(read_gpt(pdd) != SUCCESS)
		return ERROR;

	storeDrv.setLba(pdd->gpt_parts[pdd->gpt_part_count - 1].end_lba);
	return storeDrv.load(dev);
}

int init_qcow2(PedDevice *dev, int flag)
{
    DiskDriver dd, *pdd;

    pdd = &dd;
    memset(pdd, 0, sizeof(DiskDriver)); 
    pdd->hd = (void *)(dev);

	if(read_mbr(pdd) != SUCCESS)
	{
		DEBUG("read_mbr error");
		//return ERROR;
	}

	if(read_gpt(pdd) != SUCCESS)
	{
		DEBUG("read_gpt error");
		return ERROR;
	}

	storeDrv.setLba(pdd->gpt_parts[pdd->gpt_part_count - 1].end_lba);
	memcpy(dev->disk_name, pdd->diskName, 16);
	
	if(!flag)
		storeDrv.init(dev);
	else
		storeDrv.load(dev);

	storeDrv.setLba(pdd->gpt_parts[pdd->gpt_part_count - 1].end_lba);
	storeDrv.addDisk(pdd->diskName, pdd->disk_size_lba);	
	DEBUG("pdd->disk_size_lba %lld", pdd->disk_size_lba);

    return storeDrv.save(dev);
}

uint64_t add_qcow2(PedDevice *dev, char *name, uint32_t diff, uint64_t sizeLba, uint64_t realLba, uint32_t stype, 
					uint32_t disk_type, uint32_t operate_id)
{
	int ret;
    YZY_QCOW_ENTRY  *pQe;

	DEBUG("sizeLba %lld", sizeLba);
	DEBUG("realLba %lld", realLba);
	YZYGUID uuid = {0};
	str2uuid(name, &uuid);
	char temp[32] = {0};
	uuid2str(&uuid, temp);

	DEBUG("temp %s", temp);
	DEBUG("diff %d", diff);
	DEBUG("name %s", name);
	DEBUG("disk_type %d", disk_type);

    ret = storeDrv.alloc(diff, &uuid, dev->disk_name, sizeLba, realLba, disk_type, &pQe);
	if(SUCCESS == ret)
	{
		DEBUG("sizeLba %lu", sizeLba);
		DEBUG("realLba %lu", realLba);
		DEBUG("pQe->startLba %lu", pQe->startLba);
   		DEBUG("pQe->endLba %lu", pQe->endLba);
		DEBUG("pQe->realLba %lu", pQe->realLba); 
    	DEBUG("alloc success GetQcowLba(pQe) %d", GetQcowLba(pQe));
		DEBUG("disk_type %d", disk_type);
		pQe->stype = stype;
		pQe->flag = 1;
		pQe->operate_id = operate_id;
    	//storeDrv.save(dev);
		return GetQcowLba(pQe);
	}
	else
	{
		DEBUG("ret %d", ret);
		return -1;	
	}
}

void del_qcow2(PedDevice *dev, char *name, int diff)
{
	YZYGUID uuid = {0};
	str2uuid(name, &uuid);
	
	storeDrv.del(diff, &uuid);
   	storeDrv.save(dev);
}


void del_diff_qcow2(PedDevice *dev, char *name)
{
	YZYGUID uuid = {0};
	str2uuid(name, &uuid);

    uint32_t i;
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++)
    {
        PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[i];
        if (pQe->isDel == 1)
            continue;
		if(yzy_CompareGuid(&pQe->uuQcow, &uuid) == 0)
		{
			if(pQe->difLevel == 0)
			{
				storeDrv.setBoot(pQe->type, pQe);
			}
			else
			{
				pQe->isDel = 1;
			}
		}
    }
	storeDrv.rebuild();
	storeDrv.save(dev);
}

void save_qcow2(PedDevice *dev)
{
	storeDrv.save(dev);
}

void print_qcow2(PedDevice *dev)
{
	char name[36] = {0};
    uint32_t i;

	init_qcow2(dev, 1);
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++)
    {
        PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[i];
		uuid2str(&pQe->uuQcow, name);
		DEBUG("--------------------------------------------");
	    DEBUG("pQe->uuid %s", name);
	    DEBUG("pQe->type %d", pQe->type);
		
	    DEBUG("pQe->startLba %lu", pQe->startLba);
        DEBUG("pQe->endLba %lu", pQe->endLba);
        DEBUG("pQe->realLba %lu", pQe->realLba); 

	    DEBUG("pQe->diffLevel %lu", pQe->difLevel);
	    DEBUG("pQe->operate_id %lu", pQe->operate_id);
		DEBUG("--------------------------------------------");
		//if(pQe->difLevel == 1)
    }
	//set_boot_qcow2(dev, 0, name);
}

int get_max_diff_qcow2(char *name)
{
	YZYGUID uuid = {0};
	str2uuid(name, &uuid);

    uint32_t i;
	int max_diff = -1;
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++)
    {
        PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[i];
		
        if (pQe->isDel == 1 || pQe->flag == 0)
            continue;
		if(yzy_CompareGuid(&pQe->uuQcow, &uuid) == 0 && pQe->flag)
		{
			max_diff = max_diff > (int)pQe->difLevel ? max_diff : pQe->difLevel;
		}
    }
	return max_diff;
}

int get_minor_max_diff_qcow2(char *name, uint32_t diff)
{
	YZY_QCOW_ENTRY  *pQe;
	YZYGUID uuid = {0};
	str2uuid(name, &uuid);

    uint32_t i;
	uint32_t j;
	for(i = --diff; i> 0 ; i--)
	{
		pQe = storeDrv.scan(i, &uuid);
		if(pQe && pQe->flag)
			return pQe->difLevel;
	}
}



PYZY_QCOW_ENTRY scan_qcow2(char *name, uint32_t difLevel)
{
    uint32_t i;
	YZYGUID uuid = {0};
	str2uuid(name, &uuid);
	
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++)
    {   
        PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[i];
        if (pQe->isDel == 1)
            continue;
        if (difLevel == pQe->difLevel && yzy_CompareGuid(&pQe->uuQcow, &uuid) == 0)
        {   
            return pQe;
        }   
    }   
    return NULL;
}


int get_operate_qcow2(char *name, uint32_t diff)
{
	PYZY_QCOW_ENTRY pQe = scan_qcow2(name, diff);
	if(pQe)
		return pQe->operate_id;
	else
		return -1;
}

int set_boot_qcow2(PedDevice *dev, uint32_t diff, int disk_type, char *name)
{
	YZY_QCOW_ENTRY  *pQe;
	DEBUG("set boot name %s diff %d", name, diff);

	YZYGUID uuid = {0};
	str2uuid(name, &uuid);

	pQe = storeDrv.scan(diff, &uuid);
	storeDrv.setBoot(disk_type, pQe);
	storeDrv.save(dev);
}


int change_back_file_qcow2(PedDevice *dev, char *name, uint32_t diff)
{   
    int ret;
    int len = sizeof(YZY_DISK_BACK_FILE);
    YZY_DISK_BACK_FILE disk_back = {0};

	YZYGUID uuid = {0};
	str2uuid(name, &uuid);

#if 0
	uint32_t base = get_minor_max_diff_qcow2(name, diff);
	DEBUG("minor max diff %d", base);
	if(base >= diff)
	{
		DEBUG("base %d > diff error %d", base, diff);
		return ERROR;
	}
#endif
	
	YZY_QCOW_ENTRY  *pBase = storeDrv.scan(diff - 1, &uuid);
	YZY_QCOW_ENTRY  *pDiff = storeDrv.scan(diff, &uuid);

	if(pBase == NULL || pDiff == NULL)
	{
		DEBUG("scan %s diff %d error", name, diff);
		return ERROR;
	}

    memcpy(disk_back.backing_file, YZY_DISK_TAG, sizeof(YZY_DISK_TAG));
    disk_back.difLevel = pBase->difLevel;
    disk_back.qcow = pBase->uuQcow;
    
    char buf[DEFAULT_CLUSTER_SIZE] = {0};
    QCowHeader* header = (QCowHeader*)&buf[0];
    
    ret = linux_read(dev, buf, GetQcowLba(pDiff), DEFAULT_CLUSTER_SIZE / 512);
    if(ret != SUCCESS)
    {   
        DEBUG("linux_read %s error", dev->path);
    }

    if(BSWAP_32(header->magic) != QCOW_MAGIC || BSWAP_32(header->version) < QCOW_VERSION)
    {   
        DEBUG("QCOW MAGIC or QCOW VERSION ERROR");
        return ERROR;
    }   
	header->backing_file_offset = BSWAP_64(header->backing_file_offset);
	header->backing_file_size = BSWAP_32(header->backing_file_size);
    
    memcpy(&buf[header->backing_file_offset], &disk_back, len);
    buf[header->backing_file_offset + len] = '\0';
        
	header->backing_file_offset = BSWAP_64(header->backing_file_offset);
	header->backing_file_size = BSWAP_32(len);

	if(header->backing_file_offset == 0)
	{
		DEBUG("this is base qcow2");
		return ERROR;
	}
    return linux_write(dev, buf, GetQcowLba(pDiff), DEFAULT_CLUSTER_SIZE / 512);
}
