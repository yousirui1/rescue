#ifndef __EXTERNAL_H__
#define __EXTERNAL_H__


#ifdef __cplusplus
extern "C"
{
#endif


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

	unsigned char tftp_ip[32];
    unsigned char nfs_ip[32];

    int major_ver;
    int minor_ver;
    int config_ver;
};

int create_window(int window_flag);
int set_pipe(int pipe[2]);
int set_config(struct config *conf);

#ifdef __cplusplus
}

#endif
#endif
