#include "base.h"
#include "torrent.h"

#if 0
#if 0
#define TIMEOUT 5		//secs between rexmt's
#define LBUFLEN 200		//size of input buffer

struct modes {
	const char *m_name;
	const char *m_mode;
	int m_openflags;
};

static const struct modes modes[] = {
	{"", "", O_TEXT},
	{"ascii", "", O_TEXT},
	{"binary", "octet", O_BINARY},
	{"image", "octet", O_BINARY},
	{0, 0, 0}
};

#define MODE_OCTET 		(&modes[2])
#define MODE_NETASCII 	()
#define MODE_DEFAULT	MODE_NETASCII

#ifdef HAVE_IPV6
int ai_fam = AF_INSPEC;
int ai_fam_sock = AF_UNSPEC;
#else
int ai_fam = AF_INET;
int ai_fam_sock = AF_INET;
#endif

union sock_addr peeraddr;
int f = -1;
ushort port;
int trace;
int verbose;
int literal;
int connected;
const struct modes *mode;
#ifdef WITH_READLINE
char *line = NULL;
#else
char line[LBUFLEN];
#endif
#endif


void tftp_recvfile(int fd, const char *name, const char *mode)
{
	struct tftphdr *ap;
	struct tftphdr *dp;
	int n;
	volatile u_short block;
	volatile int size, firsttrip;
	volatile unsigned long amount;
	union sock_addr from;
	socklen_t fromlen;
	FILE *file;
	volatile int convert;		//true if converting crlf->lf 
	u_short dp_opcode, dp_block;

	startclock();
	dp = w_init();
	ap = (struct tftphdr *)ackbuf;
	convert = !strcmp(mode, "netascii");
	file = fdopen(fd, convert ? "wt" : "wb");
	block = 1;
	firsttrip = 1;
	amount = 0;
	
	bsd_signal(SIGALRM, timer);
	do {


			
	
	

	

}
#endif





#include "tftp.h"

static char *tftp_get_option(const char *option, char *buf, int len)
{
	int opt_val = 0;
	int opt_found = 0;
	int k;
	
	/* buf points to:
	 * "opt_name<NUL>opt_val<NUL>opt_name2<NUL><opt_val2<NUL>.." */

	while(len > 0)
	{
		/* make sure options are terminated correctly */
		for(k = 0; k < len; k++)
		{
			if(buf[k] == '\0')
				goto nul_found;
		}
		return NULL;
 nul_found:
		if(opt_val == 0)		//it's "name" part
		{
			if(strcasecmp(buf, option) == 0)
				opt_found = 1;	
		}
		else if(opt_found)
		{
			return buf;
		}
		k++;
		buf += k;
		len -= k;
		opt_val ^= 1;
	}
	return NULL;
}

int tftp_get(char *server_ip, char *remote_file, char *local_file, char *pipe_buf, int type)
{
	struct tftp_packet send_packet, recv_packet;
	const int blocksize = 512;
    int ret = 0;
    int time_wait_data;
    unsigned short block = 1;
	char *buf = (char *)&recv_packet;
	uint64_t file_size = 0;
	
	progress_info *info = (progress_info *)&pipe_buf[HEAD_LEN];

	struct sock_udp udp = create_udp(server_ip, 69, 0);
	socklen_t addr_len = sizeof(struct sockaddr_in);

	DEBUG("tftp_get server_ip %s remote_file %s local_file %s ", server_ip, remote_file, local_file);
    
    /* send request. */
    send_packet.cmd = htons(CMD_RRQ);
    sprintf(send_packet.file_name, "%s%c%s%c%s%c%d%c%s%c%d", remote_file, 0, "octet", 0, "tsize", 0, 0, 0, "blksize", 0, blocksize); 
    sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.send_addr, sizeof(struct sockaddr_in));
	
    FILE *fp = fopen(local_file, "w");
    if(fp == NULL){
        printf("Create file \"%s\" error.\n", local_file);
        return;
    }   
	time_t last_time = current_time;	
    /* receive data. */
    send_packet.cmd = htons(CMD_ACK);
    do{ 
        for(time_wait_data = 0; time_wait_data < PKT_RECV_TIMEOUT * PKT_MAX_RXMT; time_wait_data += 10000){
            ret = recvfrom(udp.fd, &recv_packet, sizeof(struct tftp_packet), MSG_DONTWAIT,
                    (struct sockaddr *)&udp.recv_addr, &addr_len);

            if(ret > 0 && ret < 4)
			{ 
                printf("Bad packet: ret=%d\n", ret);
            }   
            if(ret >= 4 && recv_packet.cmd == htons(CMD_DATA) && recv_packet.block == htons(block))
			{
                send_packet.block = recv_packet.block;
                sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.recv_addr, addr_len);
				info->total_size += ret - 4;
				
				(void)time(&current_time);	
				if(current_time - last_time > 1)
				{
					if(type == 3)
					{
						info->progress = (info->total_size  * 100)/ info->file_size;
					}
					else
					{
						info->progress = (info->total_size  * 100)/ info->file_size / 2;
						DEBUG("info->progress %d", info->progress);
						if(type == 2)
						{
							info->progress += 50;
						}
					
						if(info->progress < 2)
							info->progress = 2;

						if(info->progress >= 98)
							info->progress = 98;
					}
					send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);	
					last_time = current_time;
				}
			
                fwrite(recv_packet.data, 1, ret - 4, fp);
                break;
            }   
			if(ret >= 4 && recv_packet.cmd == htons(CMD_OACK))
			{
				file_size = atol(tftp_get_option("tsize", &buf[2], ret - 2));
				memset(&send_packet, 0, sizeof(struct tftp_packet));
				info->file_size = file_size;
				
    			send_packet.cmd = htons(CMD_ACK);
                sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.recv_addr, addr_len);
			}
            usleep(5);
        }   
        if(time_wait_data >= PKT_RECV_TIMEOUT * PKT_MAX_RXMT){
            DEBUG("Wait for DATA #%d timeout.", block);
			close_fd(udp.fd);	
			fclose(fp);
			return ERROR;
        }   
        block ++; 
    }while(ret == blocksize + 4); 
	close_fd(udp.fd);	
    fclose(fp);
	return SUCCESS;
}


#if 0
int tftp_get(char *server_ip, char *remote_file, char *local_file, char *pipe_buf, int type)
{
	struct tftp_packet send_packet, recv_packet;
	const int blocksize = 512;
    int ret = 0;
    int time_wait_data;
    unsigned short block = 1;
	char *buf = (char *)&recv_packet;
	uint64_t file_size = 0;
	
	progress_info *info = (progress_info *)&pipe_buf[HEAD_LEN];

	struct sock_udp udp = create_udp(server_ip, 69, 0);
	socklen_t addr_len = sizeof(struct sockaddr_in);
    
    /* send request. */
    send_packet.cmd = htons(CMD_RRQ);
    sprintf(send_packet.file_name, "%s%c%s%c%s%c%d%c%s%c%d", remote_file, 0, "octet", 0, "tsize", 0, 0, 0, "blksize", 0, blocksize); 
    sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.send_addr, sizeof(struct sockaddr_in));
	
    FILE *fp = fopen(local_file, "w");
    if(fp == NULL){
        printf("Create file \"%s\" error.\n", local_file);
        return;
    }   
	time_t last_time = current_time;	
    /* receive data. */
    send_packet.cmd = htons(CMD_ACK);
    do{ 
        for(time_wait_data = 0; time_wait_data < PKT_RECV_TIMEOUT * PKT_MAX_RXMT; time_wait_data += 10000){
            ret = recvfrom(udp.fd, &recv_packet, sizeof(struct tftp_packet), MSG_DONTWAIT,
                    (struct sockaddr *)&udp.recv_addr, &addr_len);

            if(ret > 0 && ret < 4)
			{ 
                printf("Bad packet: ret=%d\n", ret);
            }   
            if(ret >= 4 && recv_packet.cmd == htons(CMD_DATA) && recv_packet.block == htons(block))
			{
				DEBUG("recv_packet.block %d", recv_packet.block);
                send_packet.block = recv_packet.block;
                sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.recv_addr, addr_len);
				info->total_size += ret - 4;
				
				(void)time(&current_time);	
				if(current_time - last_time > 1)
				{
					if(type == 3)
					{
						info->progress = (info->total_size  * 100)/ info->file_size;
					}
					else
					{
						info->progress = (info->total_size  * 100)/ info->file_size / 2;
					}
					DEBUG("info->progress %d", info->progress);
					if(type == 2)
					{
						info->progress += 50;
					}
					
					if(info->progress < 2)
						info->progress = 2;

					if(info->progress >= 98)
						info->progress = 98;
						
					send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);	
					last_time = current_time;
				}
                fwrite(recv_packet.data, 1, ret - 4, fp);
                break;
            }   
			if(ret >= 4 && recv_packet.cmd == htons(CMD_OACK))
			{
				file_size = atol(tftp_get_option("tsize", &buf[2], ret - 2));
				memset(&send_packet, 0, sizeof(struct tftp_packet));
				info->file_size = file_size;
				
    			send_packet.cmd = htons(CMD_ACK);
                sendto(udp.fd, &send_packet, sizeof(struct tftp_packet), 0, (struct sockaddr*)&udp.recv_addr, addr_len);
			}
            usleep(5);
        }   
        if(time_wait_data >= PKT_RECV_TIMEOUT * PKT_MAX_RXMT){
            DEBUG("Wait for DATA #%d timeout. time_wait_data %d PKT_RECV_TIMEOUT * PKT_MAX_RXMT %d", block);
			close_fd(udp.fd);	
			fclose(fp);
			return ERROR;
        }   
        block ++; 
    }while(ret == blocksize + 4); 
	close_fd(udp.fd);	
    fclose(fp);
	return SUCCESS;
}
#endif

int tftp_put()
{


}