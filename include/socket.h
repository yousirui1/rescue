#ifndef __SOCKET_H__
#define __SOCKET_H__

#define PACKET_LEN_OFFSET 12
#define PACKET_ORDER_OFFSET 4
#define PACKET_TOKEN_OFFSET 20
#define PACKET_SUPPLEMENTARY_OFFSET 22

#define DATA_SYN 0xFF
#define DATA_SYN_OFFSET 0
#define DATA_ENCRYPT 1
#define DATA_ORDER_OFFSET 2
#define DATA_LEN_OFFSET 4

#define DATA_SIZE 1452
#define HEAD_LEN 	8
#define PACKET_LEN 24

typedef unsigned char  u8_t;
typedef unsigned short u16_t;
typedef unsigned int   u32_t;

typedef enum PACKET_TYPE
{
	BYTE_TYPE = 0,
	JSON_TYPE,
	PROTOBUF_TYPE,
}PACKET_TYPE;
/* socket */
struct sock_udp
{
    unsigned int fd; 
    unsigned int port;
    
    struct sockaddr_in recv_addr;
    struct sockaddr_in send_addr;
};

typedef struct _req_head
{
    unsigned char syn;
    unsigned char encrypt_flag;
    unsigned short cmd;
    unsigned int data_size;
}req_head;


struct sock_udp create_udp(char *ip, int port, int mreq_flag);


/* -----pipe---------- */
typedef enum PIPE_TYPE {
    PIPE_TCP = 0,
    PIPE_UDP,
    PIPE_UI,
    PIPE_EVENT,
}PIPE_TYPE;

typedef enum PIPE_MSG{
    CLIENT_CONNECT_PIPE = 1,
    CLIENT_DISCONNECT_PIPE,
    CLIENT_ONLINE_PIPE,
    CLIENT_DOWN_PIPE,
	INIT_PIPE,
    INSTALL_PIPE,
    UPDATE_CONFIG_PIPE,
    P2V_OS_PIPE,
    PROGRESS_PIPE,
    PROGRESS_ERROR_PIPE,
    EXIT_PROGRESS_PIPE,
    REBOOT_PIPE,
    SHUTDOWN_PIPE,
    ERROR_MSG_PIPE,
}PIPE_MSG;

typedef enum YZY_CMD
{
    WAKEUP = 1000,
	SHUTDOWN,
    RESTART,
    DELETE,
    UPDATE_NAME,
    UPDATE_CONFIG,
    ENTER_MAINTENANCE_MODE,
    CLEAR_ALL_DESKTOP,
    DOWNLOAD_DESKTOP,
    CANCEL_DOWNLOAD_DESKTOP,
    ADD_DATA_DISK,
    ORDER,
    UPDATE_IP,
    UPDATE_DIFF_DISK,
    PXE_START,
        
    SEND_DOWN_TORRENT = 1015,
    UPLOAD_DESKTOP,
    SEND_DESKTOP,
    UPDATA_DESKTOP_GROUP_INFO = 1024,
    CANCEL_SEND_DESKTOP,
    CANCEL_P2V,

    UPGRAD = 1030,
    //SHUT  

    HEARTBEAT = 9000,
    TERMINAL_LOGIN,
    TERMINAL_LOGOUT,
    GET_DATA_TIME,
    GET_CONFIG_VERSION,
    GET_CONFIG_INFO,
    UPDATE_CONFIG_INTO,
    GET_DESKTOP_GROUP_LIST,
    P2V_OS_TRANSFORM = 9020,
    P2V_PROGRESS = 9021,
    DIFF_DOWN_TORRENT = 9023,
    BT_TASK_STATE = 9025,

    UPLOAD_LOG = 9999,
}YZY_CMD;

#pragma pack(1)
typedef struct yzy_packet{
    u16_t  version_chief;       //���汾��       ����  0x0001
    u16_t  version_sub;         //��Ҫ�汾��     ����  0x0001
    u32_t  service_code;        //����ķ�����  ���� 0xff00ff01
    u32_t  request_code;        //������       ���� 0x00000001  ��������ÿ������ÿ�������Ψһ��
    u32_t  dataSize;            //�������С     ���� 0x00001000
    u8_t   dataType;            //���ݶ�����      ���� ox00:���������� 0x01:json 0x02:protobuf
    u8_t   encoding;            //���ݶ�ѹ����ʽ  ���� ox00:��ѹ��
    u8_t   clientType;          //�ն�����       ���� 0x01: uefi 0x02:linux  0x03: windows 0x04: server
    u8_t   reqOrRes;            //��������Ӧ   ���� 0x01: ���� 0x02: ��Ӧ
    u16_t  tokenLength;         //token����     û�о��� 0x0000
    u16_t  supplementary;       //����Э��ͷ����  û�о��� 0x0000
}yzy_packet;

typedef struct yzy_torrent{
    char uuid[36];
    char type;
    char sys_type;
    int dif_level;
    unsigned long long real_size;
    unsigned long long space_size;
    unsigned long long file_size;
    unsigned long long data_len;
    char task_uuid[36];
    char group_uuid[36];
    int operate_id;
}yzy_torrent;

#pragma pack()

struct progress_info{
    char state[12];
    unsigned int long progress;
    unsigned long long download_rate;
    unsigned long long upload_rate;
    unsigned long long total_size;      //�����ܴ�С
    unsigned long long file_size;
	unsigned long long download_size;
    
    int type;
    
    char file_name[128];
    char image_name[128];
    char storage[128];
};

typedef struct progress_info progress_info;



struct template_info{
    unsigned char template_name[36];
    unsigned int template_desc[128];
    //unsigned int template_size;
};




#endif
