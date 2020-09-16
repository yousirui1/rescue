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
//#include <stdlib.h>
#include <memory.h>
#include <stdint.h>

#include <sys/syscall.h>
#include <sys/epoll.h>
#include <sys/types.h>
#include <sys/socket.h>
//#include <sys/wait.h>
#include <sys/utsname.h>
#include <sys/resource.h>
#include <sys/utsname.h>
#include <sys/ioctl.h>

//#include <netinet/tcp.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <net/if.h>

#include <dirent.h>
#include <assert.h>


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


#define LOG_DIR  "./log"
#define LOG_ERR_FILE "./log/virt-p2v_err.log"

#define SUCCESS 0
#define ERROR 1

#define bool signed char 

#define MAX_FILENAMELEN 256



#endif

