#include "base.h"
#include "device.h"
#include "StoreConfig.h"

struct device_info dev_info;
#define STRPREFIX(a,b) (strncmp((a),(b),strlen((b))) == 0)

/**
 * Get parent device of a partition.
 *
 * Returns C<0> if no parent device could be found.
 */
static dev_t
partition_parent (dev_t part_dev)
{
  FILE *fp = NULL;
  char *path = NULL, *content = NULL;
  size_t len = 0;
  unsigned parent_major, parent_minor;

  if (asprintf (&path, "/sys/dev/block/%ju:%ju/../dev",
                (uintmax_t) major (part_dev),
                (uintmax_t) minor (part_dev)) == -1)
    error (EXIT_FAILURE, errno, "asprintf");

  fp = fopen (path, "r");
  if (fp == NULL)
    return 0;

  if (getline (&content, &len, fp) == -1) 
    error (EXIT_FAILURE, errno, "getline");

    
	if (sscanf (content, "%u:%u", &parent_major, &parent_minor) != 2)
    	return 0;

  return makedev (parent_major, parent_minor);
}


/**
 * Return true if the named device (eg. C<dev == "sda">) contains the
 * root filesystem.  C<root_device> is the major:minor of the root
 * filesystem (eg. C<8:1> if the root filesystem was F</dev/sda1>).
 *
 * This doesn't work for LVs and so on.  However we only really care
 * if this test works on the P2V ISO where the root device is a
 * regular partition.
 */
static int 
device_contains (const char *dev, dev_t root_device)
{
  struct stat statbuf;
  char *dev_name = NULL;
  dev_t root_device_parent;

  if (asprintf (&dev_name, "/dev/%s", dev) == -1) 
    error (EXIT_FAILURE, errno, "asprintf");

  if (stat (dev_name, &statbuf) == -1) 
    return 0;

  /* See if dev is the root_device. */
  if (statbuf.st_rdev == root_device)
    return 1;

  /* See if dev is the parent device of the root_device. */
  root_device_parent = partition_parent (root_device);
  if (root_device_parent == 0)
    return 0;
  if (statbuf.st_rdev == root_device_parent)
    return 1;

  return 0;
}


static void find_all_disks()
{
	DIR *dir;
	struct dirent *d;
	size_t nr_disks = 0, nr_removable = 0;
	dev_t root_device = 0;
	struct stat statbuf;
	char buf[128] = {0};
	char result[128] = {0};
	PedDevice *dev;
	if(stat("/", &statbuf) == 0)
		root_device = statbuf.st_dev;
	
	/* The default list of disks is everything in /sys/block which 
	 * matches the common patterns for disk names */
	dir = opendir("/sys/block");
	if(!dir)
		DIE("opendir /sys/block error");
	for(;;)
	{
		errno = 0;
		d = readdir(dir);
		if(!d)
			break;
		if (STRPREFIX (d->d_name, "cciss!") ||
        	STRPREFIX (d->d_name, "hd") ||
        	STRPREFIX (d->d_name, "sd") ||
        	STRPREFIX (d->d_name, "ubd") ||
        	STRPREFIX (d->d_name, "vd")) 
		{
			if(device_contains(d->d_name, root_device))
				continue;

			//dev_info.disks = realloc (dev_info.disks, sizeof (disk_param) * (dev_info.disk_count + 1));
			strcpy(dev_info.disks[dev_info.disk_count].name, d->d_name);	
			
			sprintf(buf, "cat /sys/block/%s/removable", d->d_name);
			memset(result, 0, sizeof(result));
			exec_cmd(buf, result);	

			sprintf(buf, "/dev/%s", d->d_name);
			dev = linux_new(buf);
			DEBUG("----------------------");

			if(!dev || dev->length <= 0)
			{
				DEBUG("%s dev open file error", buf);
				continue;							
			}

			if(atoi(result) == 1)
			{
				dev_info.disks[dev_info.disk_count].disk_type = DISK_TYPE_USB;	
				dev_info.usb_disk = &(dev_info.disks[dev_info.disk_count]);
			}
			else
			{
				dev_info.disks[dev_info.disk_count].disk_type = DISK_TYPE_HD;
			
				dev_info.disks[dev_info.disk_count].dev = dev;
				dev_info.disks[dev_info.disk_count].total_space = dev->length ; // KB

				if(check_qcow2(dev) == SUCCESS)
					dev_info.disks[dev_info.disk_count].disk_ready = 1;
				else
					dev_info.disks[dev_info.disk_count].disk_ready = 0;

				if(!dev_info.mini_disk)
					dev_info.mini_disk = &(dev_info.disks[dev_info.disk_count]);
				else if(dev_info.mini_disk->total_space > dev_info.disks[dev_info.disk_count].total_space)
					dev_info.mini_disk = &(dev_info.disks[dev_info.disk_count]);

				/* p2v only disk have window boot */	
				DEBUG("---------- dev->path %s boot_flag %d", dev->path, dev->boot_flag);
				if(dev->boot_flag)
				{
					DEBUG("have boot flag dev: %s",dev->path);
					dev_info.boot_disk = &(dev_info.disks[dev_info.disk_count]);
				}
				else
				{
					DEBUG("no have boot flag  %s", dev->path);
				}
			}
			dev_info.disk_count++;
		}
	}
}

void find_all_netcards()
{
	int ret;
    char buf[MAX_BUFLEN] = {0};
    char result[MAX_BUFLEN] = {0};
    ret = get_netcard_info(&dev_info.net);

    if(ret != -1) 
    {   
        dev_info.netcard_count = ret;       //网卡数
    }   

    int i;
    netcard_param *net = &(conf.netcard);
    for(i = 0; i < dev_info.netcard_count; i++)
    {   
        DEBUG("dev_info.net[%d].name %s",i, dev_info.net[i].name);
        DEBUG("dev_info.net[%d].ip %s", i, dev_info.net[i].ip);
        //DEBUG("dev_info.net[%d].mac %s", i ,dev_info.net[i].mac);
        DEBUG("dev_info.net[%d].netmask %s", i, dev_info.net[i].netmask);
		
        if(STRPREFIX(dev_info.net[i].name, "eth0"))
        {   
            memcpy(net->ip, dev_info.net[i].ip, 32);
            memcpy(net->netmask, dev_info.net[i].netmask, 32);

			//char *mac = dev_info.net[i].mac;
			//mac = strupr(mac);
			strupr(dev_info.net[i].mac);	
            //memcpy(net->mac, mac, 32);
			DEBUG("mac %s", dev_info.net[i].mac);
			memcpy(net->mac, dev_info.net[i].mac, 32);
			
    		exec_cmd("cat /sys/class/net/eth0/carrier ", result);
			if(result[0] == '0')
			{
				DEBUG("network down");
				char head[HEAD_LEN] = {0};
				client_disconnect();
				send_pipe(head, CLIENT_DOWN_PIPE, 0, PIPE_QT);
			}
        } 	
    }   
	DEBUG("net->ip %s net->netmask %s net->mac %s", net->ip, net->netmask, net->mac);
}


void init_device()
{
	int ret;
    char buf[MAX_BUFLEN] = {0};
    char result[MAX_BUFLEN] = {0};

	terminal_info *terminal = &(conf.terminal);
    memset(&dev_info, 0, sizeof(struct device_info));
    
    exec_cmd("hwinfo --short --cpu | awk '{if (NR>1) {print $0}}'", result);
    sscanf(result, "\t\t\t\t\t\t%[^\n]", terminal->cpu);
    
    memset(result, 0, MAX_BUFLEN);
    exec_cmd("hwinfo --short --netcard | awk '{if (NR>1) {print $0}}'", result);
    sscanf(result, "%s\t\t\t\t\t\t%[^\n]",buf, terminal->netcard);
	DEBUG("terminal->netcard %s", terminal->netcard);

	ret = sysconf (_SC_PHYS_PAGES);
	if(-1 == ret)
		terminal->memory = 1024 * 1024 * 1024;
	else
		terminal->memory = ret;
	DEBUG("terminal->memory %lld", terminal->memory);	
	DEBUG("terminal->cpu %s", terminal->cpu);	
	DEBUG("terminal->netcard %s", terminal->netcard);	
	
    exec_cmd("mdev -s", result); 

	find_all_disks();
	find_all_netcards();
    
	if(dev_info.mini_disk)
	{
		DEBUG("-----------%s  dev_info.mini_disk init_qcow2 ---------- %d ",dev_info.mini_disk->dev->path, dev_info.mini_disk->disk_ready);
		init_qcow2(dev_info.mini_disk->dev, dev_info.mini_disk->disk_ready);	
		terminal->disk_size = dev_info.mini_disk->total_space;
		DEBUG("terminal->disk_size %llu", terminal->disk_size);
        if(dev_info.mini_disk->disk_ready) // 安装ok
        {
			umount_boot();
			if(mount_boot() == SUCCESS)
			{
				exec_cmd("mkdir -p /boot/conf", result);
                strcpy(config_file, "/boot/conf/config.ini");
			}
        }
	}
	else
		send_error_msg(DISK_NO_FOUND_ERR);
	
	print_qcow2(dev_info.mini_disk->dev);
}

int mount_boot()
{
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};

    sprintf(cmd, mount_sh, dev_info.mini_disk->name);
    exec_cmd(cmd, result);
    if(strstr(result, "successd"))
    {   
		return SUCCESS;
    }   
	else
	{
		return ERROR;
	}
}

int umount_boot()
{
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};

    sprintf(cmd, umount_sh, dev_info.mini_disk->name);
    exec_cmd(cmd, result);
    if(strstr(result, "successd"))
    {   
		return SUCCESS;
    }   
	else
	{
		return ERROR;
	}
}

int format_disk(const char *path)
{
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};

	umount_boot();

    sprintf(cmd, "parted -s %s mklabel gpt", path);
    DEBUG("cmd: %s", cmd);
    exec_cmd(cmd, result);  
    if(strstr(result, "Error"))
    {   
        DEBUG("%s", result);
        return ERROR;
    }   

    sprintf(cmd, "parted -s %s mkpart YZYVOI fat32 0%c 1.075G", path, '%');
    DEBUG("cmd: %s", cmd);
    exec_cmd(cmd, result);  
    if(strstr(result, "Error"))
    {   
        DEBUG("%s", result);
        return ERROR;
    }   

    sleep(1);
    exec_cmd("mdev -s", result); 
    //sleep(1);

    sprintf(cmd, "mkfs.vfat %s1", path);
    DEBUG("cmd: %s", cmd);
    exec_cmd(cmd, result); 
	DEBUG("result %s", result);
	return SUCCESS;
    if(!strlen(result))
    {   
        DEBUG("format_disk fat 1G ok");
		return SUCCESS;
    }   
    else
    {   
        DEBUG("format disk fat 1G error");
		return ERROR;
    }  
}


int upgrad_programe(char *file)
{
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};
	struct server_info *server = &(conf.server);
	
	if(!dev_info.mini_disk)
	{
		DEBUG("no found ready disk");	
		return ERROR;
	}

	if(!conf.install_flag)
	{
		DEBUG("no install program disable upgrad");	
		return ERROR;
	}
    sprintf(cmd, upgrad_sh, file, server->ip);
	DEBUG("%s", cmd);
    exec_cmd(cmd, result);

    exec_cmd(upgrad_sh, result);
	DEBUG("result %s", result);
	if(strstr(result, "successd"))
	{
		DEBUG("upgrad programe ok");
		umount_boot();
		return SUCCESS;
	}
	else
	{
		DEBUG("upgrad programe error");
		return ERROR;
	}	
}

int install_programe()
{
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};
	
	if(!dev_info.usb_disk)
	{
		DEBUG("no found usb flash disk");	
		send_error_msg(U_DISK_NO_FOUD_ERR);
		return ERROR;
	}

	if(!dev_info.mini_disk)
	{
		DEBUG("no found ready disk");	
		return ERROR;
	}

	sprintf(cmd, "/dev/%s", dev_info.mini_disk->name);
	format_disk(cmd);
	init_qcow2(dev_info.mini_disk->dev, 0);

    sprintf(cmd, install_sh, dev_info.mini_disk->name, dev_info.usb_disk->name);
    exec_cmd(cmd, result);
	//DEBUG("result %s", result);
	if(strstr(result, "successd"))
	{
		DEBUG("install programe ok");
		conf.install_flag = 1;
		mount_boot();

		exec_cmd("mkdir -p /boot/conf", result);
        strcpy(config_file, "/boot/conf/config.ini");
		DEBUG("install_flag %d", conf.install_flag);
		return SUCCESS;
	}
	else
	{
		DEBUG("install programe error %s", result);
		conf.install_flag = 0;
		umount_boot();
		DEBUG("install_flag %d", conf.install_flag);
		return ERROR;
	}	
}
