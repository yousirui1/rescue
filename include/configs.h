#ifndef __CONFIGS_H__
#define __CONFIGS_H__

#define CONFIG_FILE "config.ini"

#define BASE_SECTION "base"
#define BASE_INSTALL_KEY "install_flag"

#define NET_SECTION "network"
#define NET_DHCP_KEY "dhcp"
#define NET_IP_KEY "ip"
#define NET_GATEWAY_KEY "gateway"
#define NET_NETMASK_KEY "netmask"
#define NET_DNS1_KEY "dns1"
#define NET_DNS2_KEY "dns2"
#define NET_MAC_KEY "mac"

#define SERVER_SECTION "server"
#define SERVER_IP_KEY   "server_ip"
#define SERVER_PORT_KEY "server_port"

#define TERMINAL_SECTION "system"
#define TM_ID_KEY "id"
#define TM_NAME_KEY "name"
#define TM_CONFIG_VER_KEY "conf_version"
#define TM_DESKTOP_TYPE_KEY "show_desktop_type"
#define TM_AUTO_DESKTOP_KEY "auto_desktop"
#define TM_PLATFORM_KEY "platform"

#define VERSION_SECTION "version"
#define VER_MAJOR_KEY "major_ver"
#define VER_MINOR_KEY "minor_ver"
#define VER_LINUX_KEY "linux"

#define DEFAULT_IP_VALUE "127.0.0.1"
#define DEFAULT_PORT_VALUE 50007

#define CONFIG_DEFAULT_NULL ""

#define LOG_DIR  "./log"
#define LOG_ERR_FILE "./log/virt-p2v_err.log"

#define DESKTOP_FILE "/boot/conf/desktop_config.ini"

#define DESKTOP_SECTION "desktop_"
#define DESKTOP_GROUP_UUID_KEY "group_uuid"
#define DESKTOP_VALID_KEY "valid"
#define DESKTOP_DHCP_KEY "desktop_is_dhcp"
#define DESKTOP_IP_KEY "desktop_ip"
#define DESKTOP_MASK_KEY "desktop_mask"
#define DESKTOP_GATEWAY_KEY "desktop_gateway"
#define DESKTOP_DNS1_KEY "desktop_dns1"
#define DESKTOP_DNS2_KEY "desktop_dns2"
#define DESKTOP_DEFAULT_GROUP_KEY "default_group"
#define DESKTOP_GROUP_NAME_KEY "group_name"
#define DESKTOP_OS_TYPE_KEY "os_sys_type"
#define DESKTOP_DESC_KEY "desktop_desc"
#define DESKTOP_NAME_KEY "desktop_name"

#define BASE_UUID_KEY "base_uuid"
#define BASE_DIF_KEY "base_dif"
#define BASE_PREFIX_KEY "base_prefix"
#define BASE_REAL_SIZE_KEY "base_real"
#define BASE_RESERVE_SIZE_KEY "base_resver"

#define DATA_UUID_KEY "data_uuid"
#define DATA_DIF_KEY "data_dif"
#define DATA_PREFIX_KEY "data_prefix"
#define DATA_REAL_SIZE_KEY "data_real"
#define DATA_RESERVE_SIZE_KEY "data_resver"

#define SHARE_UUID_KEY "share_uuid"
#define SHARE_DIF_KEY "share_dif"
#define SHARE_PREFIX_KEY "share_prefix"
#define SHARE_REAL_SIZE_KEY "share_real"
#define SHARE_RESERVE_SIZE_KEY "share_resver"

struct terminal_info{
    unsigned int id;
    unsigned char name[128];
    unsigned char platform[12];     //平台 x86 arm 
    unsigned int desktop_type;      //1, 2, 个人桌面教学桌面
    unsigned int auto_desktop;      //自动进入桌面

    char cpu[128];
    char netcard[128];
    unsigned long long memory;
    unsigned long long disk_size;
};

struct netcard_param{
    char is_dhcp;                   //0 不启动 1 启动
    char name[32];
    unsigned char ip[32];
    unsigned char mac[32];
    unsigned char gateway[32];
    unsigned char boardcast_addr[32];
    unsigned char netmask[32];
    unsigned char dns1[32];
    unsigned char dns2[32];
    //char desc[128];               //网卡描述 厂商信息
};

struct server_info {
    unsigned char ip[32];
    unsigned int port;
};

typedef struct terminal_info terminal_info;
typedef struct netcard_param netcard_param;
typedef struct server_info server_info;

struct config{
    int install_flag;
    int disk_ready;                 //裸磁盘qcow2 结构体初始化完成 

    terminal_info terminal;
    netcard_param netcard;

    server_info server;

    unsigned char tftp_ip[32];

    int major_ver;
    int minor_ver;
    int config_ver;
};



#endif
