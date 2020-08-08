#ifndef __PACKET_H__
#define __PACKET_H__

typedef unsigned char  u8_t;
typedef unsigned short u16_t;
typedef unsigned int   u32_t;

#define MAJOR_VER       1
#define MINOR_VER       0   


#define Big_str2u16(s)       ( (((u16_t)(s)[0])<<8) + ((u16_t)(s)[1]) )
#define Big_str2u32(s)       ( (((u32_t)(s)[0])<<24) + (((u32_t)(s)[1])<<16) + \
                               (((u32_t)(s)[2])<<8) + ((u32_t)(s)[3]) )
#define Big_u16ToStr(s, n)    do{  (s)[1] = (u8_t)((n)&0xFF);  (s)[0] = (u8_t)(((n)>>8)&0xFF);  }while(0)
#define Big_u32ToStr(s, n)    do{  (s)[3] = (u8_t)((n)&0xFF);        (s)[2] = (u8_t)(((n)>>8)&0xFF); \
                                   (s)[1] = (u8_t)(((n)>>16)&0xFF);  (s)[0] = (u8_t)(((n)>>24)&0xFF);    }while(0)

#define Little_str2u16(s)      ( (((u16_t)(s)[1])<<8) + ((u16_t)(s)[0]) )
#define Little_str2u32(s)      ( (((u32_t)(s)[3])<<24) + (((u32_t)(s)[2])<<16) + \
                                 (((u32_t)(s)[1])<<8) + ((u32_t)(s)[0]) )
#define Little_u16ToStr(s, n)  do{  (s)[0] = (u8_t)((n)&0xFF);  (s)[1] = (u8_t)(((n)>>8)&0xFF);  }while(0)
#define Little_u32ToStr(s, n)  do{  (s)[0] = (u8_t)((n)&0xFF);        (s)[1] = (u8_t)(((n)>>8)&0xFF); \
                                    (s)[2] = (u8_t)(((n)>>16)&0xFF);  (s)[3] = (u8_t)(((n)>>24)&0xFF);    }while(0)                               

//大端序则为 1，小端序为 0
#if 0
    #define STR_TO_U16(s)    Big_str2u16(s)
    #define STR_TO_U32(s)    Big_str2u32(s)
    #define U16_TO_STR(s, n) Big_u16ToStr(s, n)
    #define U32_TO_STR(s, n) Big_u32ToStr(s, n)
#else
    #define STR_TO_U16(s)    Little_str2u16(s)
    #define STR_TO_U32(s)    Little_str2u32(s)
    #define U16_TO_STR(s, n) Little_u16ToStr(s, n)
    #define U32_TO_STR(s, n) Little_u32ToStr(s, n)
#endif

// 2字节对齐 // request headr size 26
//#pragma pack(push, 1)

typedef enum vYzyProtocolStatus {
    E_YZY_PROTOCOL_SUCCESS   = 0,       //操作成功
    E_YZY_PROTOCOL_PARAMETER_ERR,       //参数错误
    E_YZY_PROTOCOL_INSUFFICIENT_SPACE,  //内存空间不足
    E_YZY_PROTOCOL_PROTOCOL_ERR,        //协议错误
} eYzyProtocolStatus;

typedef enum vYzySrvCode {
    E_SC_KEEP_ONLINE = 0,
} vYzySrvCode;


#define LINUX_TYPE 0x02

struct yzy_packet{
    u16_t  version_chief;       //主版本号       例如  0x0001
    u16_t  version_sub;         //次要版本号     例如  0x0001
    u32_t  service_code;        //请求的服务编号  例如 0xff00ff01
    u32_t  request_code;        //请求编号       例如 0x00000001  用于区别每个链接每次请求的唯一性
    u32_t  dataSize;            //数据域大小     例如 0x00001000
    u8_t   dataType;            //数据段类型      例如 ox00:二进制数据 0x01:json 0x02:protobuf
    u8_t   encoding;            //数据段压缩方式  例如 ox00:无压缩
    u8_t   clientType;          //终端类型       例如 0x01: uefi 0x02:linux  0x03: windows 0x04: server
    u8_t   reqOrRes;            //请求还是响应   例如 0x01: 请求 0x02: 响应
    u16_t  tokenLength;         //token长度     没有就是 0x0000
    u16_t  supplementary;       //补充协议头长度  没有就是 0x0000
}__attribute((aligned (1)));

typedef struct yzy_packet yzy_packet;

#define BYTE_TYPE 0x00
#define JSON_TYPE 0x01
#define PROTOBUF_TYPE 0x02

typedef enum yzy_cmd
{
    WAKEUP = 1000,
    SHUTDONW,   
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
}yzy_cmd;


#pragma pack(1)
struct yzy_torrent{
    char uuid[36]; 
    char type;
    char sys_type;
    int dif_level;
    unsigned long long real_size;
    unsigned long long space_size;
	unsigned long long file_size;
    unsigned long long data_len;
};

typedef struct yzy_torrent yzy_torrent;
#pragma pack()

struct template_info{
    unsigned char template_name[36];
    unsigned int template_desc[128];
    //unsigned int template_size;
};

#endif
