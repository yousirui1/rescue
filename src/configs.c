#include "base.h"
#include "configs.h"
#include "cJSON.h"
#include <sys/reboot.h>

struct configs conf = {0};
char config_file[128] = "config.ini";


int send_config_pipe()
{
	char pipe_buf[HEAD_LEN + sizeof(struct configs)] = {0};	
	memcpy(&pipe_buf[HEAD_LEN], &conf, sizeof(struct configs));
	return send_pipe(pipe_buf, UPDATE_CONFIG_PIPE, sizeof(struct configs), PIPE_UI);
}

void init_configs()
{
    char buf[128] = {0};
	int flag = 0;

    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};

    terminal_info *terminal = &(conf.terminal);
    netcard_param *net = &(conf.netcard);
    server_info *server = &(conf.server);


    DEBUG("config_file %s", config_file);
	terminal->id = 1;
    /* base */
    conf.install_flag = read_profile_int(BASE_SECTION, BASE_INSTALL_KEY, 0, config_file); 

    /* terminal */
    terminal->id = read_profile_int(TERMINAL_SECTION, TM_ID_KEY, terminal->id, config_file);

    if(read_profile_string(TERMINAL_SECTION, TM_NAME_KEY, buf, sizeof(buf), terminal->name, config_file))
    {
		if(strlen(buf) != 0)
		{
			memset(terminal->name, 0, sizeof(terminal->name));
        	memcpy(terminal->name, buf, strlen(buf));
		}
    }
    if(read_profile_string(TERMINAL_SECTION, TM_PLATFORM_KEY, buf, sizeof(buf), terminal->platform, config_file))
    {
		if(strlen(buf) != 0)
		{
			memset(terminal->platform, 0, sizeof(terminal->platform));
        	memcpy(terminal->platform, buf, strlen(buf));
		}
    }
	terminal->desktop_type = read_profile_int(TERMINAL_SECTION, TM_DESKTOP_TYPE_KEY, 0, config_file);
	terminal->auto_desktop = read_profile_int(TERMINAL_SECTION, TM_AUTO_DESKTOP_KEY, 0, config_file);
    conf.config_ver = read_profile_int(TERMINAL_SECTION, TM_CONFIG_VER_KEY, -2, config_file);

    /* network */
    net->is_dhcp = read_profile_int(NET_SECTION, NET_DHCP_KEY, 1, config_file);

	if(!net->is_dhcp ||	STRPREFIX(net->ip, "169.254.1.1"))
	{
	    if(read_profile_string(NET_SECTION, NET_IP_KEY, buf, sizeof(buf), net->ip, config_file))
    	{
			if(strlen(buf) != 0)
			{
				memset(net->ip, 0, sizeof(net->ip));
        		memcpy(net->ip, buf, strlen(buf));
			}
    	}
    	if(read_profile_string(NET_SECTION, NET_NETMASK_KEY, buf, sizeof(buf), net->netmask, config_file))
    	{
			if(strlen(buf) != 0)
			{
				memset(net->netmask, 0, sizeof(net->netmask));
        		memcpy(net->netmask, buf, strlen(buf));
			}
    	}

		if(read_profile_string(NET_SECTION, NET_GATEWAY_KEY, buf, sizeof(buf), net->gateway, config_file))
    	{
			if(strlen(buf) != 0)
			{
				memset(net->gateway, 0, sizeof(net->gateway));
        		memcpy(net->gateway, buf, strlen(buf));
			}
    	}
		DEBUG("set static  ip address");
		char cmd[MAX_BUFLEN] = {0};
		char result[MAX_BUFLEN] = {0};
		exec_cmd("udhcpc -t 1 -R -q -n ", result);		//关闭dhcp
		sprintf(cmd, "ifconfig eth0 %s netmask %s", net->ip, net->netmask);
		exec_cmd(cmd, result);
		
		if(strlen(net->gateway) > 0)
		{
        	sprintf(cmd, "route add default gw %s", net->gateway);
			DEBUG("cmd: %s", cmd);
        	exec_cmd(cmd, result);
		}
	}

    if(read_profile_string(NET_SECTION, NET_DNS1_KEY, buf, sizeof(buf), net->dns1, config_file))
    {
		if(strlen(buf) != 0)
		{
			memset(net->dns1, 0, sizeof(net->dns1));
        	memcpy(net->dns1, buf, strlen(buf));
		}
    }
    if(read_profile_string(NET_SECTION, NET_DNS2_KEY, buf, sizeof(buf), net->dns2, config_file))
    {
		if(strlen(buf) != 0)
		{
			memset(net->dns2, 0, sizeof(net->dns2));
        	memcpy(net->dns2, buf, strlen(buf));
		}
    }

#if 0
    if(read_profile_string(NET_SECTION, NET_MAC_KEY, buf, sizeof(buf), net->mac, config_file))
    {
		if(strlen(buf) != 0)
		{
			memset(net->mac, 0, sizeof(net->mac));
        	memcpy(net->mac, buf, strlen(buf));
			strupr(net->mac);
		}
    }
#endif

    /* server info */
    if(read_profile_string(SERVER_SECTION, SERVER_IP_KEY, buf, sizeof(buf),  server->ip, config_file))
    {
		if(strlen(buf) != 0)
		{
			memset(server->ip, 0, sizeof(server->ip));
        	memcpy(server->ip, buf, strlen(buf));
		}
    }

    server->port = read_profile_int(SERVER_SECTION, SERVER_PORT_KEY, SERVER_PORT, config_file);

    /* version */
    //conf.major_ver = read_profile_int(VERSION_SECTION, VER_MAJOR_KEY, 0, config_file);
    //conf.minor_ver = read_profile_int(VERSION_SECTION, VER_MINOR_KEY, 0, config_file);
	
	get_version(&conf.major_ver, &conf.minor_ver);

	DEBUG("dhcp %d set static ip: %s netmask: %s gw: %s mac: %s", net->is_dhcp, net->ip, net->netmask, net->gateway, net->mac);
	DEBUG("version: v%d.%d", conf.major_ver, conf.minor_ver);	
}

int update_config(char *buf, int len)
{
    char *data = &buf[HEAD_LEN];
    struct configs *c = (struct configs *)&data[0];

	DEBUG("version: v%d.%d", conf.major_ver, conf.minor_ver);	
    memcpy(&conf, c, sizeof(struct configs));    
	DEBUG("version: v%d.%d", conf.major_ver, conf.minor_ver);	
    
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};
    netcard_param *net = &(c->netcard);
	terminal_info *termainl = &(c->terminal);
    if(net->is_dhcp)
    {   
		DEBUG("update_config ip: %s", net->ip);
		strcpy(cmd, "udhcpc -n -i eth0 ");
        exec_cmd(cmd, result);
		find_all_netcards();
		DEBUG("update_config dhcpd ip: %s", net->ip);
		send_config_pipe();
    }   
	else
	{
		if(strlen(net->ip) != 0 && strlen(net->netmask) != 0 )
		{
        	sprintf(cmd, "ifconfig eth0 %s netmask %s", net->ip, net->netmask);
        	exec_cmd(cmd, result);
			if(strlen(net->gateway) > 0)
			{
        		sprintf(cmd, "route add default gw %s", net->gateway);
        		exec_cmd(cmd, result);
			}
		}
	}
	DEBUG("dhcp %d set static ip: %s netmask: %s gw: %s mac: %s", net->is_dhcp, net->ip, net->netmask, net->gateway, net->mac);
	DEBUG("termainl->id %d", termainl->id);
	DEBUG("termainl->name %s", termainl->name);
    return save_config();
}





int save_config()
{
    char buf[128] = {0};
    
    terminal_info *terminal = &(conf.terminal);
    netcard_param *net = &(conf.netcard);
    server_info *server = &(conf.server);
        
    /* base */
    sprintf(buf, "%d", conf.install_flag);
    write_profile_string(BASE_SECTION, BASE_INSTALL_KEY, buf, config_file);
		
	conf.config_ver += 1;

    /* terminal */
    sprintf(buf, "%d", terminal->id);
    write_profile_string(TERMINAL_SECTION, TM_ID_KEY, buf, config_file);
    write_profile_string(TERMINAL_SECTION, TM_NAME_KEY, terminal->name, config_file);
    write_profile_string(TERMINAL_SECTION, TM_PLATFORM_KEY, terminal->platform, config_file);
    sprintf(buf, "%d", terminal->desktop_type);
    write_profile_string(TERMINAL_SECTION, TM_DESKTOP_TYPE_KEY, buf, config_file);
    sprintf(buf, "%d", terminal->auto_desktop);
    write_profile_string(TERMINAL_SECTION, TM_AUTO_DESKTOP_KEY, buf, config_file);
    sprintf(buf, "%d", conf.config_ver);
    write_profile_string(TERMINAL_SECTION, TM_CONFIG_VER_KEY, buf, config_file);

    /* network */
    sprintf(buf, "%d", net->is_dhcp);
    write_profile_string(NET_SECTION, NET_DHCP_KEY, buf, config_file);
    write_profile_string(NET_SECTION, NET_IP_KEY, net->ip, config_file);

    write_profile_string(NET_SECTION, NET_NETMASK_KEY, net->netmask, config_file);

    write_profile_string(NET_SECTION, NET_GATEWAY_KEY, net->gateway, config_file);
    write_profile_string(NET_SECTION, NET_DNS1_KEY, net->dns1, config_file);
    write_profile_string(NET_SECTION, NET_DNS2_KEY, net->dns2, config_file);
    write_profile_string(NET_SECTION, NET_MAC_KEY, net->mac, config_file);

    /* server info */
    write_profile_string(SERVER_SECTION, SERVER_IP_KEY, server->ip, config_file);
    
    sprintf(buf, "%d", 50007);
    write_profile_string(SERVER_SECTION, SERVER_PORT_KEY, buf, config_file);

    /* version */
    sprintf(buf, "%d", conf.major_ver);
    write_profile_string(VERSION_SECTION, VER_MAJOR_KEY, buf, config_file);
    sprintf(buf, "%d", conf.minor_ver);
    write_profile_string(VERSION_SECTION, VER_MINOR_KEY, buf, config_file);

    sprintf(buf, "v%d.%d", conf.major_ver, conf.minor_ver);
    write_profile_string(VERSION_SECTION, VER_LINUX_KEY, buf, config_file);

	DEBUG("save config success");
}
#define MAX_DESKTOP 8

void update_desktop(char *json)
{
	int i, j, ret;
	char section[12] = {0};
	char buf[12] = {0};
	cJSON *root = cJSON_Parse(json);
	if(!root)
	{
		DEBUG("cJSON_Parse root error");
		return ERROR;
	}

	cJSON* desktop = cJSON_GetObjectItem(root, "desktop");
	if(!desktop)
	{
		DEBUG("cJSON_Parse desktop error");
		return ERROR;
	}
	
	cJSON *desktop_group_uuid = cJSON_GetObjectItem(desktop, "desktop_group_uuid");
	if(!desktop_group_uuid)
	{
		DEBUG("cJSON_Parse desktop_group_uuid error");
		return ERROR;
	}	


	for(i = 0; i < MAX_DESKTOP; i++)		//是否存在
	{
        sprintf(section, DESKTOP_SECTION"%02d", i);
		if(read_profile_string(section, DESKTOP_GROUP_UUID_KEY, buf, sizeof(buf), NULL, DESKTOP_FILE)) 
        {
            if(STRPREFIX(desktop_group_uuid->valuestring, buf))            //相等
			{
				DEBUG("section: %s found %s check ", section, desktop_group_uuid->valuestring);
				goto add_desktop;
			}
        }
	}

	for(i = 0; i < MAX_DESKTOP; i++)		//是否存在
	{
        sprintf(section, DESKTOP_SECTION"%02d", i);
		if(!read_profile_string(section, DESKTOP_GROUP_UUID_KEY, buf, sizeof(buf), NULL, DESKTOP_FILE))  //不存在
        {
			DEBUG("section: %s NO found ", section);
			goto add_desktop;
        }

		if(read_profile_int(section, DESKTOP_VALID_KEY, 0, DESKTOP_FILE) == 0)		//valid = 0
		{
			DEBUG("section: %s valid = 0", section);
			goto add_desktop;	
		}
	}
	DEBUG("update_desktop error");
	return ERROR;

add_desktop:
	sprintf(buf, "%d", 1);
	write_profile_string(section, DESKTOP_VALID_KEY, buf, DESKTOP_FILE);
	
	cJSON *desktop_is_dhcp = cJSON_GetObjectItem(desktop, "desktop_is_dhcp");
	if(desktop_is_dhcp)
	{
		sprintf(buf, "%d", desktop_is_dhcp->valueint);
		write_profile_string(section, DESKTOP_DHCP_KEY, buf, DESKTOP_FILE);
	}	

	cJSON *desktop_ip = cJSON_GetObjectItem(desktop, "desktop_ip");
	if(desktop_ip)
		write_profile_string(section, DESKTOP_IP_KEY, desktop_ip->valuestring, DESKTOP_FILE);

	cJSON *desktop_mask = cJSON_GetObjectItem(desktop, "desktop_mask");
	if(desktop_mask)
		write_profile_string(section, DESKTOP_MASK_KEY, desktop_mask->valuestring, DESKTOP_FILE);

	cJSON *desktop_gateway = cJSON_GetObjectItem(desktop, "desktop_gateway");
	if(desktop_gateway)
		write_profile_string(section, DESKTOP_GATEWAY_KEY, desktop_gateway->valuestring, DESKTOP_FILE);
		
	cJSON *desktop_dns1 = cJSON_GetObjectItem(desktop, "desktop_dns1");
	if(desktop_dns1)
		write_profile_string(section, DESKTOP_DNS1_KEY, desktop_dns1->valuestring, DESKTOP_FILE);
	
	cJSON *desktop_dns2 = cJSON_GetObjectItem(desktop, "desktop_dns2");
	if(desktop_dns2)
		write_profile_string(section, DESKTOP_DNS2_KEY, desktop_dns2->valuestring, DESKTOP_FILE);
	
	cJSON *desktop_group_name = cJSON_GetObjectItem(desktop, "desktop_group_name");
	if(desktop_group_name)
		write_profile_string(section, DESKTOP_GROUP_NAME_KEY, desktop_group_name->valuestring, DESKTOP_FILE);
	
	write_profile_string(section, DESKTOP_GROUP_UUID_KEY, desktop_group_uuid->valuestring, DESKTOP_FILE);
	
#if 0
	cJSON *desktop_group_status = cJSON_GetObjectItem(desktop, "desktop_group_status");
	if(desktop_group_status)
	{
		sprintf(buf, "%d", desktop_group_status->valueint);
		write_profile_string(section, DESKTOP_GROUP_STATUS_KEY, buf, DESKTOP_FILE);
	}
#endif
	
	cJSON *default_desktop_group = cJSON_GetObjectItem(desktop, "default_desktop_group");
	if(default_desktop_group)
	{
		sprintf(buf, "%d", default_desktop_group->valueint);
		write_profile_string(section, DESKTOP_DEFAULT_GROUP_KEY, buf, DESKTOP_FILE);
	}

	cJSON *os_sys_type = cJSON_GetObjectItem(desktop, "os_sys_type");
	if(os_sys_type)
	{
		sprintf(buf, "%d", os_sys_type->valueint);
		write_profile_string(section, DESKTOP_OS_TYPE_KEY, buf, DESKTOP_FILE);
	}

	cJSON *desktop_name = cJSON_GetObjectItem(desktop, "desktop_name");
	if(desktop_name)
		write_profile_string(section, DESKTOP_NAME_KEY, desktop_name->valuestring, DESKTOP_FILE);

	cJSON *desktop_group_desc = cJSON_GetObjectItem(desktop, "desktop_group_desc");
	if(desktop_group_desc)
		write_profile_string(section, DESKTOP_DESC_KEY, desktop_group_desc->valuestring, DESKTOP_FILE);

	cJSON *disks = cJSON_GetObjectItem(desktop, "disks");
	if(!disks)
	{
		DEBUG("not find disks");
		return ERROR;
	}

	cJSON *uuid, *type, *prefix, *dif_level, *real_size,  *reserve_size, *torrent_file,*restore_flag;
	for(j = 0; j < cJSON_GetArraySize(disks); j++)
	{
		cJSON *item = cJSON_GetArrayItem(disks, j);
        uuid = cJSON_GetObjectItem(item, "uuid");
        type = cJSON_GetObjectItem(item, "type");
        prefix = cJSON_GetObjectItem(item, "prefix");
        dif_level = cJSON_GetObjectItem(item, "dif_level");
        real_size = cJSON_GetObjectItem(item, "real_size");
        reserve_size = cJSON_GetObjectItem(item, "reserve_size");
        //torrent_file = cJSON_GetObjectItem(item, "torrent_file");
		if(!uuid || !type || !prefix || !dif_level || !real_size ||!reserve_size )
		{
			DEBUG("param json error");
			return ERROR;
		}

		switch(type->valueint)
		{
			case 0:		//系统盘
			{
                write_profile_string(section, BASE_UUID_KEY, uuid->valuestring, DESKTOP_FILE);
                sprintf(buf, "%d", dif_level->valueint);
				DEBUG("dif_level->valueint %d", dif_level->valueint);
                write_profile_string(section, BASE_DIF_KEY, buf, DESKTOP_FILE);
                write_profile_string(section, BASE_PREFIX_KEY, prefix->valuestring, DESKTOP_FILE);
                write_profile_string(section, BASE_REAL_SIZE_KEY, real_size->valuestring, DESKTOP_FILE);
                write_profile_string(section, BASE_RESERVE_SIZE_KEY, reserve_size->valuestring, DESKTOP_FILE);
                //sprintf(buf, "%d", disk->restore_flag);
                //write_profile_string(section, BASE_FLAG_KEY, buf, DESKTOP_FILE);
                break;
			}	
            case 1:             //数据盘
            {
				write_profile_string(section, DATA_UUID_KEY, uuid->valuestring, DESKTOP_FILE);
                sprintf(buf, "%d", dif_level->valueint);
                write_profile_string(section, DATA_DIF_KEY, buf, DESKTOP_FILE);
                write_profile_string(section, DATA_PREFIX_KEY, prefix->valuestring, DESKTOP_FILE);
                write_profile_string(section, DATA_REAL_SIZE_KEY, real_size->valuestring, DESKTOP_FILE);
                write_profile_string(section, DATA_RESERVE_SIZE_KEY, reserve_size->valuestring, DESKTOP_FILE);
                //sprintf(buf, "%d", disk->restore_flag);
                //write_profile_string(section, DATA_FLAG_KEY, buf, DESKTOP_FILE);

                break;
            }
            case 2:             //共享盘
            {
		
				write_profile_string(section, SHARE_UUID_KEY, uuid->valuestring, DESKTOP_FILE);
                sprintf(buf, "%d", dif_level->valueint);
                write_profile_string(section, SHARE_DIF_KEY, buf, DESKTOP_FILE);
                write_profile_string(section, SHARE_PREFIX_KEY, prefix->valuestring, DESKTOP_FILE);
                write_profile_string(section, SHARE_REAL_SIZE_KEY, real_size->valuestring, DESKTOP_FILE);
                write_profile_string(section, SHARE_RESERVE_SIZE_KEY, reserve_size->valuestring, DESKTOP_FILE);
        		restore_flag = cJSON_GetObjectItem(item, "restore_flag");
				if(restore_flag)
				{
                	sprintf(buf, "%d", restore_flag->valueint);
                	write_profile_string(section, SHARE_RESTORE_FLAG, buf, DESKTOP_FILE);
				}
                break;
            }
            default:
                break;
		}	
	}
}


void update_desktop_disk_level(char *group_uuid, char *disk_uuid, int diff_level, int type)
{
	int i;	
	char section[12] = {0};
	char buf[12] = {0};


	if(diff_level < 0 && diff_level > 3)
		return ;

	if(diff_level == 3)
		diff_level = 2;

	for(i = 0; i < MAX_DESKTOP; i++)		//是否存在
	{
        sprintf(section, DESKTOP_SECTION"%02d", i);
		if(read_profile_string(section, DESKTOP_GROUP_UUID_KEY, buf, sizeof(buf), NULL, DESKTOP_FILE)) 
        {
            if(STRPREFIX(group_uuid, buf))            //相等
			{
				switch(type)
				{
					case 0:		
                		sprintf(buf, "%d", diff_level);
                		write_profile_string(section, BASE_DIF_KEY, buf, DESKTOP_FILE);
						break;
					case 1:
                		sprintf(buf, "%d", diff_level);
                		write_profile_string(section, DATA_DIF_KEY, buf, DESKTOP_FILE);
						break;
					case 2:
                		sprintf(buf, "%d", diff_level);
                		write_profile_string(section, SHARE_DIF_KEY, buf, DESKTOP_FILE);
						break;
					default:
						break;
				}
	        }
        }
	}
}

#if 0
void init_test()
{
	
    int i, j, ret;
    char section[12] = {0};
    char buf[12] = {0};
	sprintf(buf, "%d", 0);

    for(i = 0; i < MAX_DESKTOP; i++)        //是否存在
    {   
        sprintf(section, DESKTOP_SECTION"%02d", i); 
		write_profile_string(section, "edit", buf, DESKTOP_FILE);
    }   

	test_programe();
	sleep(5);
	client_disconnect();
    sync();
    reboot(RB_AUTOBOOT);    
}
#endif
