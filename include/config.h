#ifndef __CONFIG_H__
#define __CONFIG_H__

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

#define DEFAULT_IP_VALUE "127.0.0.1"
#define DEFAULT_PORT_VALUE 50007

#define CONFIG_DEFAULT_NULL ""

#define LOG_DIR  "./log"
#define LOG_ERR_FILE "./log/virt-p2v_err.log"


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

typedef struct terminal_info terminal_info;

struct netcard_param{
    char is_dhcp;                   //0 不启动 1 启动
    char name[32];
    char ip[32];
    char mac[32];
    char gateway[32];
    char boardcast_addr[32];
    char netmask[32];
    char dns1[32];
    char dns2[32];
    //char desc[128];               //网卡描述 厂商信息
};

typedef struct netcard_param netcard_param;

struct server_info {
    char ip[32];
    int port;
};

typedef struct server_info server_info;

struct config{
    int install_flag;
    int disk_ready;                 //裸磁盘qcow2 结构体初始化完成 

    terminal_info terminal;
    netcard_param netcard;

    server_info server;

    int major_ver;
    int minor_ver;
    int config_ver;
};



#endif
