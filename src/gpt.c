#include "base.h"
#include "gpt.h"
#include "device.h"
#include "geom.h"
#include "disk.h"
#include "StoreConfig.h"

//#include "file.h"

#ifdef _yzyUEFI_
#include "RamDisk.h"
#endif // _yzyUEFI_

// variables
typedef struct 
{
	uint32_t time_low;
	uint16_t time_mid;
	uint16_t time_hi_and_version;
	uint8_t clock_seq_hi_and_reserved;
	uint8_t clock_seq_low;
	uint8_t node[6];
}efi_guid_t;


struct _GPTDiskData
{
	PedGeometry data_area;
	int entry_count;
	efi_guid_t uuid;
	int pmbr_boot;
};

uint8_t empty_guid[16] = { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 };

#define YZY_DRV_TAG "YZYVOI"


MBR_PARTTYPE    mbr_types[] = {
    { 0x01, STR("FAT12 (CHS)") },
    { 0x04, STR("FAT16 <32M (CHS)") },
    { 0x05, STR("Extended (CHS)") },
    { 0x06, STR("FAT16 (CHS)") },
    { 0x07, STR("NTFS/HPFS") },
    { 0x0b, STR("FAT32 (CHS)") },
    { 0x0c, STR("FAT32 (LBA)") },
    { 0x0e, STR("FAT16 (LBA)") },
    { 0x0f, STR("Extended (LBA)") },
    { 0x11, STR("Hidden FAT12 (CHS)") },
    { 0x14, STR("Hidden FAT16 <32M (CHS)") },
    { 0x16, STR("Hidden FAT16 (CHS)") },
    { 0x17, STR("Hidden NTFS/HPFS") },
    { 0x1b, STR("Hidden FAT32 (CHS)") },
    { 0x1c, STR("Hidden FAT32 (LBA)") },
    { 0x1e, STR("Hidden FAT16 (LBA)") },
    { 0x82, STR("Linux swap / Solaris") },
    { 0x83, STR("Linux") },
    { 0x85, STR("Linux Extended") },
    { 0x86, STR("NT FAT volume set") },
    { 0x87, STR("NTFS volume set") },
    { 0x8e, STR("Linux LVM") },
    { 0xa5, STR("FreeBSD") },
    { 0xa6, STR("OpenBSD") },
    { 0xa7, STR("NeXTSTEP") },
    { 0xa8, STR("MacOS UFS") },
    { 0xa9, STR("NetBSD") },
    { 0xab, STR("MacOS Boot") },
    { 0xac, STR("Apple RAID") },
    { 0xaf, STR("MacOS HFS+") },
    { 0xbe, STR("Solaris Boot") },
    { 0xbf, STR("Solaris") },
    { 0xeb, STR("BeOS") },
    { 0xee, STR("EFI Protective") },
    { 0xef, STR("EFI System (FAT)") },
    { 0xfd, STR("Linux RAID") },
    { 0, NULL },
};

GPT_PARTTYPE    gpt_types[] = {
    // Sony uses this one
    { "\x32\x97\x01\xF4\x6E\x06\x12\x4E\x82\x73\x34\x6C\x56\x41\x49\x4F", 0x00, STR("Sony System (FAT)"), GPT_KIND_FATAL },
    // Defined by EFI/UEFI specification
    { "\x28\x73\x2A\xC1\x1F\xF8\xD2\x11\xBA\x4B\x00\xA0\xC9\x3E\xC9\x3B", 0xef, STR("EFI System (FAT)"), GPT_KIND_SYSTEM },
    { "\x41\xEE\x4D\x02\xE7\x33\xD3\x11\x9D\x69\x00\x08\xC7\x81\xF3\x9F", 0x00, STR("MBR partition scheme"), GPT_KIND_FATAL },
    // Generally well-known
    { "\x16\xE3\xC9\xE3\x5C\x0B\xB8\x4D\x81\x7D\xF9\x2D\xF0\x02\x15\xAE", 0x00, STR("MS Reserved"), GPT_KIND_SYSTEM },
    { "\xA2\xA0\xD0\xEB\xE5\xB9\x33\x44\x87\xC0\x68\xB6\xB7\x26\x99\xC7", 0x00, STR("Basic Data"), GPT_KIND_BASIC_DATA },
    // From Wikipedia
    { "\xAA\xC8\x08\x58\x8F\x7E\xE0\x42\x85\xD2\xE1\xE9\x04\x34\xCF\xB3", 0x00, STR("MS LDM Metadata"), GPT_KIND_FATAL },
    { "\xA0\x60\x9B\xAF\x31\x14\x62\x4F\xBC\x68\x33\x11\x71\x4A\x69\xAD", 0x00, STR("MS LDM Data"), GPT_KIND_FATAL },
    { "\x1E\x4C\x89\x75\xEB\x3A\xD3\x11\xB7\xC1\x7B\x03\xA0\x00\x00\x00", 0x00, STR("HP/UX Data"), GPT_KIND_DATA },
    { "\x28\xE7\xA1\xE2\xE3\x32\xD6\x11\xA6\x82\x7B\x03\xA0\x00\x00\x00", 0x00, STR("HP/UX Service"), GPT_KIND_SYSTEM },
    // From Linux repository, fs/partitions/efi.h
    { "\x0F\x88\x9D\xA1\xFC\x05\x3B\x4D\xA0\x06\x74\x3F\x0F\x84\x91\x1E", 0xfd, STR("Linux RAID"), GPT_KIND_DATA },
    { "\x6D\xFD\x57\x06\xAB\xA4\xC4\x43\x84\xE5\x09\x33\xC8\x4B\x4F\x4F", 0x82, STR("Linux Swap"), GPT_KIND_SYSTEM },
    { "\x79\xD3\xD6\xE6\x07\xF5\xC2\x44\xA2\x3C\x23\x8F\x2A\x3D\xF9\x28", 0x8e, STR("Linux LVM"), GPT_KIND_DATA },
    { "\xAF\x3D\xC6\x0F\x83\x84\x72\x47\x8E\x79\x3D\x69\xD8\x47\x7D\xE4", 0x83, STR("Linux Filesystem"), GPT_KIND_DATA },
    // From Wikipedia
    { "\x39\x33\xA6\x8D\x07\x00\xC0\x60\xC4\x36\x08\x3A\xC8\x23\x09\x08", 0x00, STR("Linux Reserved"), GPT_KIND_SYSTEM },
    // From grub2 repository, grub/include/grub/gpt_partition.h
    { "\x48\x61\x68\x21\x49\x64\x6F\x6E\x74\x4E\x65\x65\x64\x45\x46\x49", 0x00, STR("GRUB2 BIOS Boot"), GPT_KIND_SYSTEM },
    // From FreeBSD repository, sys/sys/gpt.h
    { "\xB4\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", 0xa5, STR("FreeBSD Data"), GPT_KIND_DATA },
    { "\xB5\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", 0x00, STR("FreeBSD Swap"), GPT_KIND_SYSTEM },
    { "\xB6\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", 0xa5, STR("FreeBSD UFS"), GPT_KIND_DATA },
    { "\xB8\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", 0x00, STR("FreeBSD Vinum"), GPT_KIND_DATA },
    { "\xBA\x7C\x6E\x51\xCF\x6E\xD6\x11\x8F\xF8\x00\x02\x2D\x09\x71\x2B", 0xa5, STR("FreeBSD ZFS"), GPT_KIND_DATA },
    { "\x9D\x6B\xBD\x83\x41\x7F\xDC\x11\xBE\x0B\x00\x15\x60\xB8\x4F\x0F", 0xa5, STR("FreeBSD Boot"), GPT_KIND_DATA },
    // From NetBSD repository, sys/sys/disklabel_gpt.h
    { "\x32\x8D\xF4\x49\x0E\xB1\xDC\x11\xB9\x9B\x00\x19\xD1\x87\x96\x48", 0x00, STR("NetBSD Swap"), GPT_KIND_SYSTEM },
    { "\x5A\x8D\xF4\x49\x0E\xB1\xDC\x11\xB9\x9B\x00\x19\xD1\x87\x96\x48", 0xa9, STR("NetBSD FFS"), GPT_KIND_DATA },
    { "\x82\x8D\xF4\x49\x0E\xB1\xDC\x11\xB9\x9B\x00\x19\xD1\x87\x96\x48", 0xa9, STR("NetBSD LFS"), GPT_KIND_DATA },
    { "\xAA\x8D\xF4\x49\x0E\xB1\xDC\x11\xB9\x9B\x00\x19\xD1\x87\x96\x48", 0xa9, STR("NetBSD RAID"), GPT_KIND_DATA },
    { "\xC4\x19\xB5\x2D\x0E\xB1\xDC\x11\xB9\x9B\x00\x19\xD1\x87\x96\x48", 0xa9, STR("NetBSD CCD"), GPT_KIND_DATA },
    { "\xEC\x19\xB5\x2D\x0E\xB1\xDC\x11\xB9\x9B\x00\x19\xD1\x87\x96\x48", 0xa9, STR("NetBSD CGD"), GPT_KIND_DATA },
    // From http://developer.apple.com/mac/library/technotes/tn2006/tn2166.html
//    { "\x00\x53\x46\x48\x00\x00\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0x00, STR("MacOS HFS+"), GPT_KIND_SYSTEM },
    { "\x00\x53\x46\x48\x00\x00\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0xaf, STR("MacOS HFS+"), GPT_KIND_DATA },
    { "\x72\x6F\x74\x53\x67\x61\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0xaf, STR("MacOS Core Storage"), GPT_KIND_DATA },
    { "\x00\x53\x46\x55\x00\x00\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0xa8, STR("MacOS UFS"), GPT_KIND_DATA },
    { "\x74\x6F\x6F\x42\x00\x00\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0xab, STR("MacOS Boot"), GPT_KIND_DATA },
    { "\x44\x49\x41\x52\x00\x00\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0xac, STR("Apple RAID"), GPT_KIND_DATA },
    { "\x44\x49\x41\x52\x4F\x5F\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0xac, STR("Apple RAID (Offline)"), GPT_KIND_DATA },
    { "\x65\x62\x61\x4C\x00\x6C\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0x00, STR("Apple Label"), GPT_KIND_SYSTEM },
    // From Wikipedia
    { "\x6F\x63\x65\x52\x65\x76\xAA\x11\xAA\x11\x00\x30\x65\x43\xEC\xAC", 0x00, STR("Apple TV Recovery"), GPT_KIND_DATA },
    // From OpenSolaris repository, usr/src/uts/common/sys/efi_partition.h
    { "\x7f\x23\x96\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Reserved"), GPT_KIND_SYSTEM },
    { "\x45\xCB\x82\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0xbf, STR("Solaris Boot"), GPT_KIND_DATA },
    { "\x4D\xCF\x85\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0xbf, STR("Solaris Root"), GPT_KIND_DATA },
    { "\x6F\xC4\x87\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Swap"), GPT_KIND_SYSTEM },
    { "\xC3\x8C\x89\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0xbf, STR("Solaris Usr / Apple ZFS"), GPT_KIND_DATA },
    { "\x2B\x64\x8B\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Backup"), GPT_KIND_SYSTEM },
    { "\xC7\x2A\x8D\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Reserved (Stand)"), GPT_KIND_SYSTEM },
    { "\xE9\xF2\x8E\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0xbf, STR("Solaris Var"), GPT_KIND_DATA },
    { "\x39\xBA\x90\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0xbf, STR("Solaris Home"), GPT_KIND_DATA },
    { "\xA5\x83\x92\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Alternate Sector"), GPT_KIND_SYSTEM },
    { "\x3B\x5A\x94\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Reserved (Cache)"), GPT_KIND_SYSTEM },
    { "\xD1\x30\x96\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Reserved"), GPT_KIND_SYSTEM },
    { "\x67\x07\x98\x6A\xD2\x1D\xB2\x11\x99\xa6\x08\x00\x20\x73\x66\x31", 0x00, STR("Solaris Reserved"), GPT_KIND_SYSTEM },
    // List sentinel
    { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 0, NULL, 0 },
};
GPT_PARTTYPE    gpt_dummy_type =
    { { 0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0 }, 0, STR("Unknown"), GPT_KIND_FATAL };

//
// MBR functions
//


int def_read_sector(void* hd, uint64_t lba, uint8_t* buffer)
{

	PedDevice  *dev = (PedDevice *)hd;

	assert(!dev->external_mode);
    assert(dev->open_count > 0); 
	
	if(linux_read(dev, buffer, lba, 1)) 
    {   
        return -1;
    }   
	return 0;
}
int def_write_sector(void* hd, uint64_t lba, uint8_t* buffer)
{
	PedDevice  *dev = (PedDevice *)hd;

	assert(!dev->external_mode);
    assert(dev->open_count > 0); 
	
	if(linux_write(dev, buffer, lba, 1)) 
    {   
        return -1;
    }   
	return 0;
}


CHARN * mbr_parttype_name(uint8_t type)
{
	int i;
    for (i = 0; mbr_types[i].name; i++)
        if (mbr_types[i].type == type)
            return mbr_types[i].name;
    return STR("Unknown");
}


extern StoreDriver storeDrv;


int write_sector(int fd, char *buf, int count)
{
    void *diobuf;
    void *diobuf_start;
    ssize_t write_length = count * 512;

    if(posix_memalign(&diobuf, 512, write_length) != 0)
        return 0;

    memcpy(diobuf, buf, write_length);
    diobuf_start = diobuf;
    
    for(;;)
    {   
        ssize_t status = write(fd, diobuf, write_length);
        if(status == write_length)
            break;
        if(status > 0)
        {   
            write_length -= status;
            diobuf = (char *) diobuf + status;
            continue;
        }   
        if(status < 0)
        {   
            DEBUG("disk write error");
            break;
        }   
    }   
    free(diobuf_start);
    return SUCCESS; 
}

int _device_seek(int fd, PedSector sector)
{
    off_t pos = sector * 512;
    return lseek(fd, pos, SEEK_SET) == pos;
}

//1：系统盘base，2：数据盘base，3：系统差异盘，4：数据差异盘
int read_mbr(DiskDriver* pdd)
{
    uint32_t               status;
    uint32_t               i;
    uint8_t             used;
    MBR_PART_INFO       *table;
    uint8_t             sector[512];

	PedDevice *dev = (PedDevice *)pdd->hd;
	_device_seek(dev->fd, 0);

    DEBUG("Current MBR partition table:\n");
    if (pdd->read_sector == NULL)
        pdd->read_sector = def_read_sector;
    if (pdd->write_sector == NULL)
        pdd->write_sector = def_write_sector;

    // read MBR data
    status = pdd->read_sector(pdd->hd,0, sector);
    if (status != 0) return status;

    // check for validity
    if (*((uint16_t *)(sector + 510)) != 0xaa55) {
        DEBUG(" No MBR partition table present!\n");
        return 1;
    }
    table = (MBR_PART_INFO *)(sector + 446);
    for (i = 0; i < 4; i++) {
        if (table[i].flags != 0x00 && table[i].flags != 0x80) {
            DEBUG(" MBR partition table is invalid!\n");
            return 1;
        }
    }

    // check if used
	used = 0;
    for (i = 0; i < 4; i++) {
        if (table[i].start_lba > 0 && table[i].size > 0) {
            used = 1;
            break;
        }
    }
    if (!used) {
        DEBUG(" No partitions defined\n");
        return 0;
    }

    // dump current state & fill internal structures
    DEBUG(" # A    Start LBA      End LBA  Type\n");
    for (i = 0; i < 4; i++) {
        if (table[i].start_lba == 0 || table[i].size == 0)
            continue;

        pdd->mbr_parts[pdd->mbr_part_count].index     = i;
        pdd->mbr_parts[pdd->mbr_part_count].start_lba = (uint64_t)table[i].start_lba;
        pdd->mbr_parts[pdd->mbr_part_count].end_lba   = (uint64_t)table[i].start_lba + (uint64_t)table[i].size - 1;
        pdd->mbr_parts[pdd->mbr_part_count].mbr_type  = table[i].type;
        pdd->mbr_parts[pdd->mbr_part_count].active    = (table[i].flags == 0x80) ? TRUE : FALSE;

        DEBUG(" %d %s %lld %lld  %02x  %s\n",
            pdd->mbr_parts[pdd->mbr_part_count].index + 1,
            pdd->mbr_parts[pdd->mbr_part_count].active ? STR("*") : STR(" "),
            pdd->mbr_parts[pdd->mbr_part_count].start_lba,
            pdd->mbr_parts[pdd->mbr_part_count].end_lba,
            pdd->mbr_parts[pdd->mbr_part_count].mbr_type,
              mbr_parttype_name((uint8_t)pdd->mbr_parts[pdd->mbr_part_count].mbr_type));

		if(pdd->mbr_parts[pdd->mbr_part_count].active)
		{
			dev->boot_flag = 1;
			DEBUG("------------- boot flag ---------------");
		}
		
        pdd->mbr_part_count++;
    }

    return 0;
}

//
// GPT functions
//

GPT_PARTTYPE * gpt_parttype(uint8_t *type_guid)
{
	int i;
    for (i = 0; gpt_types[i].name; i++)
        if (guids_are_equal(gpt_types[i].guid, type_guid))
            return &(gpt_types[i]);
    return &gpt_dummy_type;
}

int read_gpt(DiskDriver* pdd)
{
    uint32_t       status;
    GPT_HEADER  *header;
    GPT_ENTRY   *entry;
    uint64_t      entry_lba;
    uint32_t       entry_count, entry_size, i;
    uint8_t             sector[512];

	PedDevice *dev = (PedDevice *)pdd->hd;

    DEBUG("Current GUID partition table:\n");
    if (pdd->read_sector == NULL)
        pdd->read_sector = def_read_sector;
    if (pdd->write_sector == NULL)
        pdd->write_sector = def_write_sector;
    // read GPT header
    status = pdd->read_sector(pdd->hd,1, sector);
    if (status != 0)
        return status;

    // check signature
    header = (GPT_HEADER *)sector;
    if (header->signature != 0x5452415020494645ULL) {
        DEBUG(" No GPT partition table present!\n");
        return ERROR;
    }
    if (header->spec_revision != 0x00010000UL) {
        DEBUG(" Warning: Unknown GPT spec revision 0x%08x\n", header->spec_revision);
    }
    if ((512 % header->entry_size) > 0 || header->entry_size > 512) {
        DEBUG(" Error: Invalid GPT entry size (misaligned or more than 512 bytes)\n");
        return ERROR;
    }

    // read entries
    entry_lba   = header->entry_lba;
    entry_size  = header->entry_size;
    entry_count = header->entry_count;
    pdd->disk_size_lba = header->last_usable_lba;
    memcpy(pdd->diskName, header->disk_guid, 16);

    for (i = 0; i < entry_count; i++) {
        if (((i * entry_size) % 512) == 0) {
            status = pdd->read_sector(pdd->hd,entry_lba, sector);
            if (status != 0)
                return status;
            entry_lba++;
        }
        entry = (GPT_ENTRY *)(sector + ((i * entry_size) % 512));

        if (guids_are_equal(entry->type_guid, empty_guid))
            continue;
        if (pdd->gpt_part_count == 0) {
            DEBUG(" #      Start LBA      End LBA  Type\n");
        }

        pdd->gpt_parts[pdd->gpt_part_count].index     = i;
        pdd->gpt_parts[pdd->gpt_part_count].start_lba = entry->start_lba;
        pdd->gpt_parts[pdd->gpt_part_count].end_lba   = entry->end_lba;
        memcpy(&pdd->gpt_parts[pdd->gpt_part_count].gpt_entry ,entry,sizeof(GPT_ENTRY));
        pdd->gpt_parts[pdd->gpt_part_count].mbr_type  = 0;
        pdd->gpt_parts[pdd->gpt_part_count].gpt_parttype = gpt_parttype(entry->type_guid);
        pdd->gpt_parts[pdd->gpt_part_count].active = FALSE;

        DEBUG(" %d   %12lld %12lld  %s   %ws\n",
            pdd->gpt_parts[pdd->gpt_part_count].index + 1,
            pdd->gpt_parts[pdd->gpt_part_count].start_lba,
            pdd->gpt_parts[pdd->gpt_part_count].end_lba,
            pdd->gpt_parts[pdd->gpt_part_count].gpt_parttype->name,
            pdd->gpt_parts[pdd->gpt_part_count].gpt_entry.name);

        pdd->gpt_part_count++;
    }
    if (pdd->gpt_part_count == 0) {
        DEBUG(" No partitions defined\n");
        return ERROR;
    }
    return 0;
}

int scan_yzy_gpt(DiskDriver* pdd)
{
	int i;
    for (i = 0; i < pdd->gpt_part_count; i++)
    {
        if (memcmp(pdd->gpt_parts[i].gpt_entry.name, YZY_DRV_TAG, sizeof(YZY_DRV_TAG)) == 0)
            return i;
    }
    return -1;
    //  rootPathName = 0x001dd914 "\\\\?\\Volume{34e9d950-d1e7-4639-bd71-3c0b5da8865b}\\"
}

static PedDisk *gpt_alloc(const PedDevice *dev)
{
#if 0
	PedDisk *disk;
	GPTDiskData *gpt_disk_data;
	PedSector data_start, data_end;

	disk = disk_alloc();

	if(!disk)
		goto error;

	data_start = 2 + GPT_DEFAULT_PARTITION_ENTRY_ARRAY_SIZE / dev->sector_size;
	data_end = dev->length - 2 - GPT_DEFAULT_PARTITION_ENTRY_ARRAY_SIZE / dev->sector_size;
		
	/* If the device is too small to have room for data, reject it */
	if(data_end <= data_start)
		goto error_free_disk;

	disk->disk_specific = gpt_disk_data = malloc(sizeof(GPTDiskData));
	
	if(!disk->disk_specific)
		goto error_free_disk;

	//ped_geometry_init();
	
	gpt_disk_data->entry_count = GPT_DEFAULT_PARTITION_ENTRIES;
	uuid_generate();
	swap_uuid_and_efi_guid();
	gpt_disk_data->pmbr_boot = 0;	

	return disk;
error_free_disk:
	free(disk);
error:
	return NULL;
#endif
}

static int gpt_read(const PedDisk *disk)
{
	GPTDiskData *gpt_disk_data = disk->disk_specific;
	int i;

	int write_back = 0;

	ped_disk_delete_all(disk);
	
	/* motivation: let the user decide about the pmbr... during
	 * ped_disk_probe(), they probably didn't get a choice .. */
	if(!gpt_probe(disk->dev))
		goto error;
	
	GuidPartitionTableHeader_t *gpt = NULL;
	GuidPartitionTableHeader_t *primary_gpt;
	GuidPartitionTableHeader_t *backup_gpt;
	PedSector backup_sector_num;		
	int read_failure = gpt_read_headers();
	
	if(read_failure)
	{
		/* This include the case in which there used to be a GPT partition
	 	 * table here, with an alternate LBA that extended beyond the current
		 * end-of-device. It's treated as a non-match */
		pth_free(backup_gpt);
		pth_free(primary_gpt);
		return 0;
	}
	
	if(primary_gpt && backup_gpt)
	{
		/* Both are valid */
		if(PED_LE64_TO_CPU() < disk->dev->length - 1)
		{

		}
	}
	else if()
	{

	}
	else if()
	{

	}
	else
	{

	}
	backup_gpt = NULL;
	primary_gpt = NULL;
}


static int gpt_write(const PedDisk *disk)
{

}

/* p2v 修改gpt 分区大小和位置 */
int modify_gpt_size(const PedDevice *dev, uint64_t sector_size)
{
	PedDisk *disk = gpt_alloc(dev);
		
	gpt_read(disk);	

	//disk->size = 0;

	gpt_write(disk);
}
