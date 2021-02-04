#include "base.h"
#include "alarm.h"
#include "http.h"

static int got_clean = 0;
static int chunked = 0;
static int use_proxy = 0;
static char protocol[] = "HTTP";
static int usr_header_user_agent = 1;
static char user_agent[] = "";

static int usr_header_auth = 0;
static char *server = NULL, *user = NULL;

static off_t content_len = 0;

static int http_parse_url(const char *url, char *host, char *file, int *port)
{
    char *p1, *p2;  
    int ret, len;

    p1 = url;

    if(STRPREFIX(p1, "http://"))
    {
        p1 += strlen("http://");    
    }
    else
    {
        return ERROR;
    }
    p2 = strchr(p1, '/');

    if(p2)
    {
        len = strlen(p1) - strlen(p2);
        memcpy(host, p1, len);
        host[len] = '\0';
            
        if(*(p2 + 1))
        {
            memcpy(file, p2 + 1,strlen(p2) - 1 );
            file[strlen(p2) - 1] = '\0';
        }
    }
    else
    {
        memcpy(host, p1, strlen(p1));
        host[strlen(p1)] = '\0';
    }
    
    p1 = strchr(host, ':');
    if(p1)
    {
        *p1 ++ = '\0';
        *port = atoi(p1);
    }
    else
    {
        *port = HTTP_PORT;
    }
    return SUCCESS;
}

char *skip_whitespace(const char *s)
{
	while( *s != ' ' || (unsigned char) (*s - 9) <= (13 - 9))
		s++;
	return (char *)s;
}


char *skip_non_whitespace(const char *s)
{
	while(*s != '\0' && *s != ' ' && (unsigned char) (*s - 9) > (13 - 9))
		s++;
	return (char *)s;
}

int index_in_strings(const char *strings, const char *key)
{
	int index = 0;
	while(*strings)
	{
		if(strcmp(strings, key) == 0)
			return index;

		strings += strlen(strings) + 1;
		index++;
	}

	return -1;
}


static char *sanitize_string(char *s)
{
	unsigned char *p = (void *)s;
	while(*p)
	{
		if(*p <' ')
		{
			if(*p != '\t')
				break;

			*p = ' ';
		}
		p++;
	}
	*p = '\0';
	return s;
}

static char *strchrnul(const char *s, int c)
{
	while(*s != '\0' && *s != c)
		s++;
	return (char *)s;
}

static char fgets_trim_sanitize(FILE *fp, const char *fmt, char *http_buf)
{
	char c;
	char *buf_ptr;

	set_alarm();		//set alarm TIMEOUT interrupt 

	/* http 是text 格式用\r\n 分割可以采用fget 很好的读取 */
	if(fgets(http_buf, HTTP_BUFLEN, fp) == NULL)
		DEBUG("error getting response");
	clear_alarm();

	buf_ptr = strchrnul(http_buf, '\n');
	if(!buf_ptr)
	{
		DEBUG("no find ' '");
		return -1;
	}

	c = *buf_ptr;
	sanitize_string(http_buf);

#if 0
	if(fmt)
		fprintf(stderr, fmt, http_buf);	
#endif

	return c;
}


static int retrieve_file_data(FILE *fp, char *pipe_buf, PedDevice *dev, uint64_t file_section)
{
	char buf[HTTP_BUFLEN + 1];
	int ret;

	progress_info *info = (progress_info *)&pipe_buf[HEAD_LEN];
	time_t last_time = current_time;
	uint64_t download_rate = 0;

	strcpy(info->state, "downloading");
	
	if(chunked)
		goto get_clen;
	
	for(;;)
	{
		for(;;)
		{
			int n;
 			unsigned int rdsz;
		
			errno = 0;
			rdsz = HTTP_BUFLEN;
			if(got_clean)
			{
				if(content_len < HTTP_BUFLEN)
				{
					if(content_len <= 0)
						break;
					rdsz = content_len;
				}
			}
			n = fread(buf, 1, rdsz, fp);
			if(n > 0)
			{
				download_rate += n;
				info->total_size += n;
				write_sector(dev->fd, buf, (rdsz + 512 - 1) / 512);
				if(got_clean)
				{
					content_len -= n;
					if(content_len == 0)
						break;
				}	
			}
			else if(errno != EAGAIN)	
			{
				if(ferror(fp))
				{
					DEBUG("fread error !!");
				}
				ret = ERROR;
				break;
			}

			(void *)time(&current_time);
        	if(current_time - last_time >= 1)
        	{
            	info->download_rate = download_rate;
            	info->progress = info->total_size * 100 / info->file_size;      
            	DEBUG("HTTP download info->download_rate %lu KB/s, info->total_size %lu KB, info->progress %d",info->download_rate/1000, info->total_size/1000, info->progress);
            	send_pipe(pipe_buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
            	download_rate = 0;
            	last_time = current_time;
        	}
		}

		fgets_trim_sanitize(fp, NULL, buf);
get_clen:
		fgets_trim_sanitize(fp, NULL, buf);
		errno = 0;
		content_len = strtoull(buf, NULL, 16);

		if(content_len < 0 || errno)
			DEBUG("bad chunk length '%s'", buf);
		if(content_len == 0)
		{
			info->progress = 99;
			ret = SUCCESS;
			break;	
		}
		got_clean = 1;	
	}

	chunked = 0;
	got_clean = 1;
		
	return ret;
}

static char *get_sanitized_hdr(FILE *fp, char *buf)
{
	char *s, *hdrval;
	int c;
	
	c = fgets_trim_sanitize(fp, "  %s\n", buf);
	
	if(buf[0] == '\0')
		return NULL;

	for(s = buf; isalnum(*s) || *s == '-' || *s == '.' || *s == '_'; ++s)
	{

		*s |= 0x20;
	}

	if(*s != ':')
		DEBUG("bad header line: %s", buf);

	*s++ = '\0';
	hdrval = skip_whitespace(s);

	if(c != '\n')
	{
		while(c = getc(fp), c != EOF && c != '\n')
			continue;
	}

	return hdrval;
}


/*
 * GET file -->
 * HTTP/1.1 200 OK <---
 * chunked 分片 
 * |- length  -|- CRLF -|
 * |- chunked data  -|- CRLF -|
 * |- 0  -|- CRLF -|
 * |- CRLF -|
 */

int http_get(const char *url, char *pipe_buf, uint64_t offset, PedDevice *dev, uint64_t file_section)
{
	int port = 0;
	char host_ip[32];
	char file[MAX_FILENAMELEN];
	int beg_range = 0;
	char buf[HTTP_BUFLEN];
	char *str = buf;
	int status = 0;
	int ret;
	FILE *fp = NULL;
    int fd;
    
    if(!url)
    {
        DEBUG("url is NULL");
        return ERROR;
    }

    if(http_parse_url(url, host_ip, file, &port))
    {
        DEBUG("http parse url error");
        return ERROR;
    }

    fd = socket(AF_INET, SOCK_STREAM, 0); 
    if (fd == -1) 
    {   
        DEBUG("unable to create socket");
        return NULL;
    }   

    ret = connect_server(fd, host_ip, port, TIME_OUT);
    if(ret != SUCCESS)
    {    
        DEBUG("connect http ip: %s port: %d timeout 10s", host_ip, port);
        close_fd(fd);
        return NULL;
    }    

	/* fp 和 sockfd 必须同一个文件 
     * fprintf Segmentation fault */
    fp = fdopen(fd, "r+");
	
	got_clean = 0;
	chunked = 0;

	if(use_proxy)
	{
		fprintf(fp, "GET %s://%s/%s HTTP/1.1\r\n",
			protocol, host_ip, file);
	}
	else
	{
		DEBUG("file %s", file);
		fprintf(fp, "GET /%s HTTP/1.1\r\n", file);
	}
	
	fprintf(fp, "Host: %s:%d\r\n", host_ip, port);
	
	if(usr_header_user_agent)
		fprintf(fp, "User-Agent: %s\r\n", user_agent);
	
	
    /* Ask server to close the connection as soon as we are done
     * (IOW: we do not intend to send more requests)
     */
	fprintf(fp, "Connection: close\r\n");

	if(user && !usr_header_auth)
	{
#if 0
		fprintf(fp, "Proxy-Authorization: Basic %s\r\n"+6,
			base64enc(server.user));
#endif
	}

	if(use_proxy && server && !usr_header_auth)
	{
#if 0
		fprintf(fp, "Proxy-Authorization: Basic %s\r\n",
			base64enc(server.user));
#endif
	}

	/* 文件偏移头 续传 */
	if(beg_range != 0)
		fprintf(fp, "Range: bytes=%llu-\r\n", beg_range);	
	
	fprintf(fp, "\r\n");
	fflush(fp);

read_response:
	fgets_trim_sanitize(fp, "  %s\n", buf);
	str = skip_non_whitespace(str);	
	str = skip_whitespace(str);	

	status = atoi(str);

	DEBUG("code %d", status);
	switch(status)
	{
		case 0:
		case 100:
		
		case 201:
		case 202:
		case 203:
		case 204:
		case 300:
		case 301:
		case 302:
		case 303:

		case 206:
			DEBUG("http error ");
			return ERROR;
		case 200:
		default:
			break;
	}

	while((str = get_sanitized_hdr(fp, buf)) != NULL)		// 一行一行读取
	{
		static const char keywords[] = "content-length\0""transfer-encoding\0""location\0";
		enum { KEY_content_length = 1, KEY_transfer_encoding, KEY_location	};

		unsigned char key;

		char *s = strchrnul(str, '\0') - 1;
		while(s > str && (*s == ' ' || *s == '\t'))
		{
			*s = '\0';
			s--;
		}
		
		key = index_in_strings(keywords, buf) + 1;
		if(key == KEY_content_length)
		{
			DEBUG("KEY_content_length");
			content_len = strtoull(str, NULL, 10);
			if(content_len < 0 || errno)
				DEBUG("content-length %s is garbage", str);	

			got_clean = 1;
			continue;
		}
		if(key == KEY_transfer_encoding)
		{
			//DEBUG("KEY_transfer_encoding %s", str);
			
			//if (strcmp(str, "chunked") != 0)
//				DEBUG("transfer encoding '%s' is not supported", str);
		
			if(!STRPREFIX(str, "chunked"))
				;//DEBUG("transfer encoding '%s' is not supported", str);

			chunked = 1;
		}
	}
	_device_seek(dev->fd, offset);
	ret = retrieve_file_data(fp, pipe_buf, dev, file_section);
	return  ret;
}


