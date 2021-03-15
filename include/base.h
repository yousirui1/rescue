#ifndef __BASE_H__
#define __BASE_H__

#include <stdio.h>
#include <errno.h>
#include <time.h>
#include <unistd.h>
#include <ctype.h>
#include <signal.h>
#include <pthread.h>
#include <fcntl.h>
#include <math.h>
#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

#include <sys/syscall.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>

#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <sys/reboot.h>
//#include <net/if.h>

#include <dirent.h>
#include <assert.h>
#include <uuid/uuid.h>

#include "configs.h"
#include "socket.h"
#include "global.h"
#include "error.h"


#define __DEBUG__
#ifdef __DEBUG__
#define DEBUG(format,...) \
        do { printf("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
			log_msg("File: "__FILE__", Line: %05d:  " format"\r\n", __LINE__, ##__VA_ARGS__); \
        }while(0)

#define FAIL(format,...) \
        do { printf("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
			err_msg("File: "__FILE__", Line: %05d:  " format"\r\n", __LINE__, ##__VA_ARGS__); \
        }while(0)

#define DIE(format,...) \
        do { printf("File: "__FILE__", Line: %05d: " format"\r\n", __LINE__, ##__VA_ARGS__); \
			err_msg("File: "__FILE__", Line: %05d:  " format"\r\n", __LINE__, ##__VA_ARGS__); \
            pthread_exit((void *)ERROR); \
        }while(0)
#else
#define DEBUG(format,...)
#define FAIL(format,...)
#define DIE(format,...) pthread_exit((void *)ERROR)
#endif      //__DEBUG__

#define STRPREFIX(a,b) (strncmp((a),(b),strlen((b))) == 0)

#define BSWAP_8(x) ((x) & 0xff)
#define BSWAP_16(x) ((BSWAP_8(x) << 8) | BSWAP_8((x) >> 8))
#define BSWAP_32(x) ((BSWAP_16(x) << 16) | BSWAP_16((x) >> 16))
#define BSWAP_64(x) ((BSWAP_32(x) << 32) | BSWAP_32((x) >> 32))

#define SUCCESS 0
#define ERROR 	1
#define TIMEOUT 3

#define SERVER_PORT 50007

#define SCHED_PRIORITY_EVENT    0
#define SCHED_PRIORITY_CLIENT   1
#define SCHED_PRIORITY_TASKT    2
#define SCHED_PRIORITY_UI       3
#define SCHED_PRIORITY_TORRENT  4

#define MAX_BUFLEN      1024 * 10   
#define MAX_FILENAMELEN 256
#define MAX_CONFIGLEN   1024

#define TIME_OUT 20

#define COVERAGE_MODE 1
#define INCRMENT_MODE 2

#ifndef __cplusplus
#define max(a,b)    (((a) > (b)) ? (a) : (b))
#define min(a,b)    (((a) < (b)) ? (a) : (b))
#endif

#endif
