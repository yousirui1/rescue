#include "base.h"
#include "StoreConfig.h"
#include "device.h"

#ifndef _yzyUEFI_
#include <stdio.h>
#include <string.h>
#endif // !_yzyUEFI_

#ifdef _yzyUEFI_
#include "RamDisk.h"
#define memset(dest,ch,count)      SetMem(dest,(uint32_t)(count),(uint8_t)(ch))
#endif // _yzyUEFI_

#include <stddef.h>

#define MAX(a,b)  (((a)>(b))?(a):(b))
#define MIN(a,b)  (((a)<(b))?(a):(b))

#define yzy_offsetof(s,m) offsetof(s,m)

uint8_t gStoreBuf[YZY_MAX_STORE_BUFF_LEN];
uint32_t gStoreBufLen = YZY_MAX_STORE_BUFF_LEN;

//crc//////////////////////////////////
uint32_t POLYNOMIAL = 0xEDB88320;
int have_table = 0;
uint32_t table[256];


static int yzy_file_read(void *hd, uint64_t offset_lba, uint8_t * buf, uint64_t buf_len)
{
    PedDevice  *dev = (PedDevice *)hd;
    assert(!dev->external_mode);
    assert(dev->open_count > 0); 

	uint64_t sector = buf_len / 512;
	if(buf_len % 512 != 0)
	{
		sector += 1;
	}

    if(linux_read(dev, buf, offset_lba, sector))
    {   
        return -1;
    }   
    return 0;
}

static int  yzy_file_write(void *hd, uint64_t offset_lba, uint8_t * buf, uint64_t buf_len)
{
   	PedDevice  *dev = (PedDevice *)hd;
    assert(!dev->external_mode);
    assert(dev->open_count > 0); 

	uint64_t sector = buf_len / 512;
	if(buf_len % 512 != 0)
	{
		sector += 1;
	}
	//_device_seek(dev->fd, offset_lba);
	//write_sector(dev->fd, buf, buf_len / 512);

    if(linux_write(dev, buf, offset_lba, sector))
    {   
        return -1;
    }   
    return 0;
}


static void make_table()
{
    int i, j;
    have_table = 1;
    for (i = 0; i < 256; i++)
        for (j = 0, table[i] = i; j < 8; j++)
            table[i] = (table[i] >> 1) ^ ((table[i] & 1) ? POLYNOMIAL : 0);
}


uint32_t crc32(uint32_t crc, char* buff, int len)
{
    if (!have_table) make_table();
    crc = ~crc;
	int i;
    for (i = 0; i < len; i++)
        crc = (crc >> 8) ^ table[(crc ^ buff[i]) & 0xff];
    return ~crc;
}
//crc//////////////////////////////////


static int VerifyCrc()
{
    uint32_t len = storeDrv.pStoreCfg->qcowCount * sizeof(YZY_QCOW_ENTRY);

    uint32_t crc = crc32(186, &gStoreBuf[yzy_offsetof(YZY_STORE_CONFIG, entry)], len);
	
    return crc == storeDrv.pStoreCfg->crc;
}

static void SetCrc()
{
    uint32_t len = storeDrv.pStoreCfg->qcowCount * sizeof(YZY_QCOW_ENTRY);

    uint32_t crc = crc32(186, &gStoreBuf[yzy_offsetof(YZY_STORE_CONFIG, entry)], len);
    storeDrv.pStoreCfg->crc = crc;
}

static void ZeroGuid(PYZYGUID guid)
{
    guid->Data1 = 0;
    guid->Data2 = 0;
    guid->Data3 = 0;
    guid->Data4 = 0;
}

int yzy_CompareGuid(PYZYGUID guid1, PYZYGUID guid2)
{
    if (guid1->Data1 == guid2->Data1 && guid1->Data2 == guid2->Data2 &&
        guid1->Data3 == guid2->Data3 && guid1->Data4 == guid2->Data4)
    {
        return 0;
    }
    return -1;
}

static uint32_t GetDiskList(PYZYGUID diskName, PYZY_QCOW_ENTRY *diskList)
{
    uint32_t j = 0;
	uint32_t i;
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++) //从已删除的位置alloc
    {
        PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[i];
        if (yzy_CompareGuid(&pQe->diskName, diskName) == 0)
        {
            diskList[j] = pQe;
            j++;
        }
    }
    return j;
}

uint64_t GetDiskSizeLba(PYZYGUID diskName)
{
	int i;
	if (diskName == NULL)
        return storeDrv.pStoreCfg->diskItem[0].diskSizeLba;
    for (i = 0; i < YZY_MAX_DISK_COUNT; i++)
    {
        if (yzy_CompareGuid(diskName, &storeDrv.pStoreCfg->diskItem[i].diskName) == 0)
        {
            return storeDrv.pStoreCfg->diskItem[i].diskSizeLba;
        }
    }
    return 0;
}

int SetDiskSizeLba(PYZYGUID diskName, uint64_t size_lba)
{
	int i;
    for (i = 0; i < YZY_MAX_DISK_COUNT; i++)
    {
        if (yzy_CompareGuid(diskName, &storeDrv.pStoreCfg->diskItem[i].diskName) == 0)
        {
            storeDrv.pStoreCfg->diskItem[i].diskSizeLba = size_lba;
            return 0;
        }
    }
    return -1;
}

/* 返回值 >=0 成功值为当前下标  -2 已存在,会更新size  -1已满 添加失败 */
int AddDisk(PYZYGUID diskName, uint64_t size_lba)
{
    int j = -1;
	int i;
    for (i = 0; i < YZY_MAX_DISK_COUNT; i++)
    {
        if (yzy_CompareGuid(diskName, &storeDrv.pStoreCfg->diskItem[i].diskName) == 0)
        {
            storeDrv.pStoreCfg->diskItem[i].diskSizeLba = size_lba;
            return -2;
        }
        if (j == -1 && storeDrv.pStoreCfg->diskItem[i].diskSizeLba == 0)
            j = i;
    }
    if (j >= 0)
    {
        storeDrv.pStoreCfg->diskItem[j].diskName = *diskName;
        storeDrv.pStoreCfg->diskItem[j].diskSizeLba = size_lba;
    }
    return j;
}

//gpt头disk_guid
static int InitStoreConfig()
{
    memset(storeDrv.pStoreCfg, 0, sizeof(YZY_STORE_CONFIG));
    memcpy(&storeDrv.pStoreCfg->tag, "yzyVOI", 6);
    storeDrv.pStoreCfg->qcowCount = 0;
    storeDrv.pStoreCfg->crc = 0;
    return 0;
}

void RebuildStoreConfig() //剔除已删除的项目,位置排序，以便空隙查找
{
    uint8_t rebuildBuf[YZY_MAX_STORE_BUFF_LEN];

    PYZY_STORE_CONFIG pConfig = (PYZY_STORE_CONFIG)rebuildBuf;
    memcpy(rebuildBuf, storeDrv.pStoreCfg, yzy_offsetof(YZY_STORE_CONFIG, entry));
    uint32_t j = 0;
	uint32_t i;
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++) //剔除已删除的项目
    {
        if (storeDrv.pStoreCfg->entry[i].isDel == 1)
            continue;

        memcpy(&pConfig->entry[j], &storeDrv.pStoreCfg->entry[i], sizeof(YZY_QCOW_ENTRY));
        j++;
    }
    pConfig->qcowCount = j;
    memcpy(gStoreBuf, rebuildBuf, yzy_offsetof(YZY_STORE_CONFIG, entry)); //恢复头

    for (j = 0; j < pConfig->qcowCount; j++)
    {
        PYZY_QCOW_ENTRY pQe = NULL;
        uint64_t minLba = (uint64_t)-1;
        for (i = 0; i < pConfig->qcowCount; i++)
        {
            if (pConfig->entry[i].startLba < minLba)
            {
                minLba = pConfig->entry[i].startLba;
                pQe = &pConfig->entry[i];
            }
        }
        if (pQe)
        {
            memcpy(&storeDrv.pStoreCfg->entry[j], pQe, sizeof(YZY_QCOW_ENTRY));
            pQe->startLba = (uint64_t)-1;
        }
    }
}
int uuid2str(PYZYGUID GUID, char* uuid);

static int UpdateStoreEntry(PYZY_QCOW_ENTRY qe, uint32_t difLevel, PYZYGUID name, PYZYGUID diskName, uint64_t startLba, uint64_t endLba, uint64_t realLba, uint8_t type)
{
    qe->difLevel = difLevel;
    qe->uuQcow = *name;
    qe->diskName = *diskName;
    qe->startLba = startLba;
    qe->endLba = endLba;
    qe->type = type;
    qe->isDel = 0;
	qe->flag = 0;
    qe->realLba = realLba;
	qe->operate_id = 0;
    return 0;
}

static int AddStoreEntry(uint32_t difLevel, PYZYGUID name, PYZYGUID diskName, uint64_t sizeLba, uint64_t realLba, int type, PYZY_QCOW_ENTRY *diskList, uint32_t count) //锟铰斤拷位锟矫达拷0锟斤拷始锟斤拷锟斤拷锟叫讹拷写锟斤拷锟斤拷锟斤拷锟斤拷锟饺?锟斤拷位锟斤拷偏锟斤拷
{
    uint64_t diskSizeLba;
    if (storeDrv.pStoreCfg->qcowCount >= YZY_MAX_STORE_QCOW_ENTRY)
    {
        return -1;
    }

    PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[storeDrv.pStoreCfg->qcowCount];
    if (count > 0)
    {
        PYZY_QCOW_ENTRY pLastQe = diskList[count - 1];

        DEBUG("count %d", count);   
        diskSizeLba = GetDiskSizeLba(diskName);
        DEBUG("diskName %16X", diskName);
        DEBUG("diskSizeLba %ldd", diskSizeLba);
        DEBUG("pLastQe->startLba %lld", pLastQe->startLba);
        DEBUG("pLastQe->endLba %lld", pLastQe->endLba);
        DEBUG("pLastQe->realLba %lld", pLastQe->realLba);
        DEBUG("diskSizeLba %lu", diskSizeLba);
        DEBUG("sizeLba %lu",  sizeLba);
        DEBUG("pLastQe->endLba %lu", pLastQe->endLba);
        //diskSizeLba = GetDiskSizeLba(diskName);
        if (pLastQe->endLba + sizeLba > diskSizeLba)
        {
            return -2; //超出硬盘容量
        }

        if (UpdateStoreEntry(pQe, difLevel, name, diskName, pLastQe->endLba, pLastQe->endLba + sizeLba, realLba, type) < 0)
            return -1;
    }
    else
    {
        if (UpdateStoreEntry(pQe, difLevel, name, diskName, 0, sizeLba, realLba, type) < 0)
            return -1;
    }
    storeDrv.pStoreCfg->qcowCount++;
    return 0;
}

static int DeleteStoreEntry(uint32_t difLevel, PYZYGUID name)
{
	uint32_t i;
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++)
    {
        PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[i];
        if (difLevel == pQe->difLevel && yzy_CompareGuid(&pQe->uuQcow, name) == 0)
        {
            pQe->isDel = 1;
            RebuildStoreConfig();
            return 0;
        }
    }
    return -1;
}

static PYZY_QCOW_ENTRY ScanStoreEntry(uint32_t difLevel, PYZYGUID name)
{
	uint32_t i;
    for (i = 0; i < storeDrv.pStoreCfg->qcowCount; i++)
    {
        PYZY_QCOW_ENTRY pQe = &storeDrv.pStoreCfg->entry[i];
        if (pQe->isDel == 1)
            continue;
        if (difLevel == pQe->difLevel && yzy_CompareGuid(&pQe->uuQcow, name) == 0)
        {
            return pQe;
        }
    }
    return NULL;
}

//static void  ScanSpaceFormDelete(uint64_t sizeLba, PYZYGUID diskName, int64_t* startLba, int64_t* endLba, PYZY_QCOW_ENTRY* diskList,uint32_t count)
//{
//    int64_t sLba,l;
//    int64_t suit = MAXINT64;
//
//    *startLba = 0;
//    *endLba = 0;
//
//    for (uint32_t i = 0; i < count; i++) //从已删除的位置alloc
//    {
//        PYZY_QCOW_ENTRY pQe = diskList[i];
//        if (pQe->isDel == 0)
//            continue;
//
//        sLba = pQe->startLba;
//        if (i > 0)
//        {
//            PYZY_QCOW_ENTRY pQe0 = diskList[i - 1]; //包括了上层空隙
//            sLba = pQe0->endLba;
//        }
//
//        l = (pQe->endLba - sLba) - sizeLba;
//        if (l >= 0 && suit > l)  //找到一个最合适的位置
//        {
//            suit = l;
//            *startLba = sLba;
//            *endLba = pQe->endLba;
//        }
//    }
//}

static void ScanSpaceFormInterval(uint64_t sizeLba, PYZYGUID diskName, int64_t *startLba, int64_t *endLba, PYZY_QCOW_ENTRY *diskList, uint32_t count)
{
    int64_t l;
    int64_t suit = MAXINT64;
    YZY_QCOW_ENTRY Qe = {0};
    PYZY_QCOW_ENTRY pQe0;
    PYZY_QCOW_ENTRY pQe1;

    *startLba = 0;
    *endLba = 0;
	uint32_t i;
    for (i = 0; i < count; i++) //从两个文件间空隙alloc
    {
        if (i == 0) //从0开始
        {
            pQe0 = &Qe;
        }
        else
            pQe0 = diskList[i - 1];

        pQe1 = diskList[i];
        l = (pQe1->startLba - pQe0->endLba) - sizeLba;
        if (l >= 0 && suit > l) //找到一个最合适的位置
        {
            suit = l;
            *startLba = pQe0->endLba;
            *endLba = pQe1->startLba;
        }
    }
}

// 如果中间空出来40G，可以放入两个20G
static int AllocStore(uint32_t difLevel, PYZYGUID name, PYZYGUID diskName, uint64_t sizeLba, uint64_t realLba, uint8_t type, PYZY_QCOW_ENTRY* ppQe)
{
    uint64_t iStart, iEnd;
    int ret;

    PYZY_QCOW_ENTRY pQe = ScanStoreEntry(difLevel, name);
    if (pQe)
    {
        *ppQe = pQe;
        return 0;
    }

    PYZY_QCOW_ENTRY diskList[YZY_MAX_STORE_QCOW_ENTRY];
    uint32_t j = GetDiskList(diskName, diskList);

    ScanSpaceFormInterval(sizeLba, diskName, &iStart, &iEnd, diskList, j);
    if (iEnd - iStart == 0) //没有删除或间隙
    {
        ret = AddStoreEntry(difLevel, name, diskName, sizeLba, realLba, type, diskList, j);
        if (ret < 0)
            return ret;
        *ppQe = &storeDrv.pStoreCfg->entry[storeDrv.pStoreCfg->qcowCount - 1];
        return 0;
    }
    if (storeDrv.pStoreCfg->qcowCount >= YZY_MAX_STORE_QCOW_ENTRY)
    {
        return -1;
    }
    *ppQe = &storeDrv.pStoreCfg->entry[storeDrv.pStoreCfg->qcowCount];
    storeDrv.pStoreCfg->qcowCount++;

    if (UpdateStoreEntry(*ppQe, difLevel, name, diskName, iStart, iEnd, realLba, type) < 0)
        return -1;

    return 0;
}

// sizeLba: 保留大小
// realLba: 实际大小
static int AllocStoreSpace(uint32_t difLevel, YZYGUID name, YZYGUID diskName, uint64_t sizeLba, uint64_t realLba, uint8_t type, PYZY_QCOW_ENTRY* ppQe)
{
    int ret = AllocStore(difLevel, &name, &diskName, sizeLba, realLba, type, ppQe);
    if (ret == 0)
    {
        RebuildStoreConfig();
        *ppQe = ScanStoreEntry(difLevel, &name);
    }
	if(*ppQe == NULL)
		return -1;
		
    return ret;
}

uint64_t GetBackLba()
{
    return storeDrv.storeLba + 28;
}

int LoadFormOffset(yzy_file_t hd, uint64_t offset)
{
    int ret = yzy_file_read(hd, offset, gStoreBuf, YZY_MAX_STORE_BUFF_LEN);
    if (ret == -1)
	{
		DEBUG("yzy_file_read error");
        return -1;
	}

    if (memcmp(&storeDrv.pStoreCfg->tag, "yzyVOI", 6) != 0)
        return 1;

    if (VerifyCrc() == 0)
        return 2;

    return 0;
}

/* -1 read disk error
 *  1  tag erro
 *  2  crc error
 */
int LoadStoreConfig(yzy_file_t hd)
{
    if (storeDrv.storeLba == 0)
	{
		DEBUG("storeDrv.storeLba == 0 ");
        return -1;
	}

    int ret = LoadFormOffset(hd, storeDrv.storeLba * FILESECBYTES);

    if (ret != 0)
        ret = LoadFormOffset(hd, GetBackLba() * FILESECBYTES); //read backup

    return ret;
}

int SaveStoreConfig(yzy_file_t hd)
{
    if (storeDrv.storeLba == 0)
	{
        return -1;
	}
    RebuildStoreConfig();
    SetCrc();

    int ret = yzy_file_write(hd, storeDrv.storeLba * FILESECBYTES, gStoreBuf, gStoreBufLen);

    ret = yzy_file_write(hd, GetBackLba() * FILESECBYTES, gStoreBuf, gStoreBufLen); //write backup
    if (ret == gStoreBufLen)
        return 0;
    return -1;
}

void SetStoreLba(uint64_t lba)
{
    storeDrv.storeLba = lba + 100;
}

uint64_t GetQcowLba(PYZY_QCOW_ENTRY pQe)
{
    return storeDrv.storeLba + 68 + pQe->startLba;
}

void SetBoot(int idx, PYZY_QCOW_ENTRY pQe)
{
    if (idx == 0)
    {
        storeDrv.pStoreCfg->currBoot = pQe->uuQcow;
        storeDrv.pStoreCfg->bootLevel = pQe->difLevel;
    }
    if (idx == 1)
    {
        storeDrv.pStoreCfg->currData = pQe->uuQcow;
        storeDrv.pStoreCfg->dataLevel = pQe->difLevel;
    }
    if (idx == 2)
    {
        storeDrv.pStoreCfg->shareData = pQe->uuQcow;
        storeDrv.pStoreCfg->shareLevel = pQe->difLevel;
    }
}

StoreDriver storeDrv = {
    0,
    (PYZY_STORE_CONFIG)gStoreBuf,
    SetStoreLba,
    GetQcowLba,
    InitStoreConfig,
    DeleteStoreEntry,
    ScanStoreEntry,
    AllocStoreSpace,
    LoadStoreConfig,
    SaveStoreConfig,
    RebuildStoreConfig,
    SetDiskSizeLba,
    GetDiskSizeLba,
    AddDisk,
    SetBoot,
};
