#ifndef __TFTP_H__
#define __TFTP_H__

#include <poll.h>
#include <sys/stat.h>

#define ENABLE_TFTPD 1
#define TFTP_BLKSIZE_DEFAULT       512  /* according to RFC 1350, don't change */
#define TFTP_BLKSIZE_DEFAULT_STR "512"
/* Was 50 ms but users asked to bump it up a bit */
#define TFTP_TIMEOUT_MS            100
#define TFTP_MAXTIMEOUT_MS        2000
#define TFTP_NUM_RETRIES            12  /* number of backed-off retries */

/* opcodes we support */
#define TFTP_RRQ   1
#define TFTP_WRQ   2
#define TFTP_DATA  3
#define TFTP_ACK   4
#define TFTP_ERROR 5
#define TFTP_OACK  6

/* error codes sent over network (we use only 0, 1, 3 and 8) */
/* generic (error message is included in the packet) */
#define ERR_UNSPEC   0
#define ERR_NOFILE   1
#define ERR_ACCESS   2
/* disk full or allocation exceeded */
#define ERR_WRITE    3
#define ERR_OP       4
#define ERR_BAD_ID   5
#define ERR_EXIST    6
#define ERR_BAD_USER 7
#define ERR_BAD_OPT  8

/* masks coming from getopt32 */
enum {
    TFTP_OPT_GET = (1 << 0),
    TFTP_OPT_PUT = (1 << 1),
    /* pseudo option: if set, it's tftpd */
    TFTPD_OPT = (1 << 7) * ENABLE_TFTPD,
    TFTPD_OPT_r = (1 << 8) * ENABLE_TFTPD,
    TFTPD_OPT_c = (1 << 9) * ENABLE_TFTPD,
    TFTPD_OPT_u = (1 << 10) * ENABLE_TFTPD,
    TFTPD_OPT_l = (1 << 11) * ENABLE_TFTPD,
};


#endif
