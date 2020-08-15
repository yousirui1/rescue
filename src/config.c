#include "base.h"
#include "config.h"

int send_config_pipe()
{
	char pipe_buf[HEAD_LEN + sizeof(struct config)] = {0};	
	memcpy(&pipe_buf[HEAD_LEN], &conf, sizeof(struct config));
	return send_pipe(pipe_buf, UPDATE_CONFIG_PIPE, sizeof(struct config), PIPE_QT);
}

void init_config()
{
    char buf[128] = {0};

    char result[MAX_BUFLEN] = {0};

    terminal_info *terminal = &(conf.terminal);
    netcard_param *net = &(conf.netcard);
    server_info *server = &(conf.server);

    DEBUG("config_file %s", config_file);
    /* base */
    conf.install_flag = read_profile_int(BASE_SECTION, BASE_INSTALL_KEY, 0, config_file); 

    /* terminal */
    terminal->id = read_profile_int(TERMINAL_SECTION, TM_ID_KEY, terminal->id, config_file);

    if(read_profile_string(TERMINAL_SECTION, TM_NAME_KEY, buf, sizeof(buf), terminal->name, config_file))
    {
        memcpy(terminal->name, buf, strlen(buf));
    }
    if(read_profile_string(TERMINAL_SECTION, TM_PLATFORM_KEY, buf, sizeof(buf), terminal->platform, config_file))
    {
        memcpy(terminal->platform, buf, strlen(buf));
    }
	terminal->desktop_type = read_profile_int(TERMINAL_SECTION, TM_DESKTOP_TYPE_KEY, 0, config_file);
	terminal->auto_desktop = read_profile_int(TERMINAL_SECTION, TM_AUTO_DESKTOP_KEY, 0, config_file);
    conf.config_ver = read_profile_int(TERMINAL_SECTION, TM_CONFIG_VER_KEY, -2, config_file);

    /* network */
    net->is_dhcp = read_profile_int(NET_SECTION, NET_DHCP_KEY, 1, config_file);
    if(read_profile_string(NET_SECTION, NET_IP_KEY, buf, sizeof(buf), net->ip, config_file))
    {
        memcpy(net->ip, buf, strlen(buf));
    }
    if(read_profile_string(NET_SECTION, NET_NETMASK_KEY, buf, sizeof(buf), net->netmask, config_file))
    {
        memcpy(net->netmask, buf, strlen(buf));
    }
    if(read_profile_string(NET_SECTION, NET_GATEWAY_KEY, buf, sizeof(buf), net->gateway, config_file))
    {
        memcpy(net->gateway, buf, strlen(buf));
    }
    if(read_profile_string(NET_SECTION, NET_DNS1_KEY, buf, sizeof(buf), net->dns1, config_file))
    {
        memcpy(net->dns1, buf, strlen(buf));
    }
    if(read_profile_string(NET_SECTION, NET_DNS2_KEY, buf, sizeof(buf), net->dns2, config_file))
    {
        memcpy(net->dns2, buf, strlen(buf));
    }
    if(read_profile_string(NET_SECTION, NET_MAC_KEY, buf, sizeof(buf), net->mac, config_file))
    {
        memcpy(net->mac, buf, strlen(buf));
    }

    /* server info */
    if(read_profile_string(SERVER_SECTION, SERVER_IP_KEY, buf, sizeof(buf),  server->ip, config_file))
    {
        memcpy(server->ip, buf, strlen(buf));
    }

    server->port = read_profile_int(SERVER_SECTION, SERVER_PORT_KEY, server->port, config_file);

    /* version */
    conf.major_ver = read_profile_int(VERSION_SECTION, VER_MAJOR_KEY, 0, config_file);
    conf.minor_ver = read_profile_int(VERSION_SECTION, VER_MINOR_KEY, 0, config_file);
	
	//print_config(&conf);
}

int update_config(char *buf, int len)
{
    char *data = &buf[HEAD_LEN];
    struct config *c = (struct config *)&data[0];
    memcpy(&conf, c, sizeof(struct config));    
    
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};
    netcard_param *net = &(c->netcard);
	terminal_info *termainl = &(c->terminal);
    if(net->is_dhcp)
    {   
		strcpy(cmd, "udhcpc");
        exec_cmd(cmd, result);
		find_all_netcards();
		send_config_pipe();
    }   
	else
	{
		if(strlen(net->ip) != 0 && strlen(net->netmask) != 0 )
		{
        	sprintf(cmd, "ifconfig eth0 %s netmask %s", net->ip, net->netmask);
        	exec_cmd(cmd, result);
        	sprintf(cmd, "route add default gw %s", net->gateway);
        	exec_cmd(cmd, result);
		}
	}
	DEBUG("dhcp %d set static ip: %s netmask: %s gw: %s mac: %s", net->is_dhcp, net->ip, net->netmask, net->gateway, net->mac);
	DEBUG("termainl->id %d", termainl->id);
	DEBUG("termainl->name %s", termainl->name);
	//DEBGU("");
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

}

#if 0
void update_desktop(desktop_info *desktop)
{
    char buf[128] = {0};
    char section[24] = {0};
    disk_info *disk;

    int i, j;
    for(i = 0; i < MAX_DESKTOP; i++)
    {
        sprintf(section, DESKTOP_SECTION"%02d", i);
        DEBUG("%s", section);
        if(read_profile_string(section, DESKTOP_GROUP_UUID_KEY, buf, sizeof(buf), NULL, DESKTOP_FILE))
        {
            if(strcmp(desktop->desktop_group_uuid, buf))            //不相等
                continue;
        }
        sprintf(buf, "%d", 1);
        write_profile_string(section, DESKTOP_VALID_KEY, buf, DESKTOP_FILE);
        sprintf(buf, "%d", desktop->os_sys_type);
        write_profile_string(section, DESKTOP_OS_TYPE_KEY, buf, DESKTOP_FILE);
        sprintf(buf, "%d", desktop->desktop_group_status);
        write_profile_string(section, DESKTOP_GROUP_STATUS_KEY, buf, DESKTOP_FILE);
        write_profile_string(section, DESKTOP_GROUP_NAME_KEY, desktop->desktop_group_name, DESKTOP_FILE);
        sprintf(buf, "%d", desktop->default_desktop_group);
        write_profile_string(section, DESKTOP_DEFAULT_GROUP_KEY, buf, DESKTOP_FILE);
        write_profile_string(section, DESKTOP_GROUP_UUID_KEY, desktop->desktop_group_uuid, DESKTOP_FILE);

        DEBUG("--------- desktop->disk_count %d -----", desktop->disk_count);
        for(j = 0; j < desktop->disk_count; j++)
        {
            disk = &(desktop->disks[j]);
            switch(disk->type)
            {
                case 0:             //系统盘
                {
                    write_profile_string(section, BASE_DISK_UUID_KEY, disk->uuid, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->type);
                    write_profile_string(section, BASE_DISK_TYPE_KEY, buf, DESKTOP_FILE);
                    write_profile_string(section, BASE_DISK_DIF_LEVEL_KEY, disk->prefix, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->dif_level);
                    write_profile_string(section, BASE_DISK_PREFIX_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%llu", disk->real_size);
                    write_profile_string(section, BASE_DISK_REAL_SIZE_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%llu", disk->reserve_size);
                    write_profile_string(section, BASE_DISK_RESERVE_SIZE_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->restore_flag);
                    write_profile_string(section, BASE_RESTORE_FLAG_KEY, buf, DESKTOP_FILE);
                    break;
                }
                case 1:             //数据盘
                {
                    write_profile_string(section, DATE_DISK_UUID_KEY, disk->uuid, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->type);
                    write_profile_string(section, DATE_DISK_TYPE_KEY, buf, DESKTOP_FILE);
                    write_profile_string(section, DATE_DISK_DIF_LEVEL_KEY, disk->prefix, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->dif_level);
                    write_profile_string(section, DATE_DISK_PREFIX_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%llu", disk->real_size);
                    write_profile_string(section, DATE_DISK_REAL_SIZE_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%llu", disk->reserve_size);
                    write_profile_string(section, DATE_DISK_RESERVE_SIZE_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->restore_flag);
                    write_profile_string(section, DATE_RESTORE_FLAG_KEY, buf, DESKTOP_FILE);
                    break;
                }
                case 2:             //共享盘
                {
                    write_profile_string(section, SHARE_DISK_UUID_KEY, disk->uuid, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->type);
                    write_profile_string(section, SHARE_DISK_TYPE_KEY, buf, DESKTOP_FILE);
                    write_profile_string(section, SHARE_DISK_DIF_LEVEL_KEY, disk->prefix, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->dif_level);
                    write_profile_string(section, SHARE_DISK_PREFIX_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%llu", disk->real_size);
                    write_profile_string(section, SHARE_DISK_REAL_SIZE_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%llu", disk->reserve_size);
                    write_profile_string(section, SHARE_DISK_RESERVE_SIZE_KEY, buf, DESKTOP_FILE);
                    sprintf(buf, "%d", disk->restore_flag);
                    write_profile_string(section, SHARE_RESTORE_FLAG_KEY, buf, DESKTOP_FILE);
                    break;
                }
                default:
                    break;
            }
        }
        break;
    }
}
#endif
