#ifndef __HTTP_H__
#define __HTTP_H__


#define HTTP_PORT 80
#define HTTP_BUFLEN 4 * 1024
//#define HTTP_BUFLEN 512

#define HTTP_POST "POST /%s HTTP/1.1\r\nHOST: %s:%d\r\nAccept: */*\r\n"\
    "Content-Type:application/x-www-form-urlencoded\r\nContent-Length: %d\r\n\r\n%s"

#define HTTP_GET_RANGE "GET /%s HTTP/1.1\r\n"\
				"Host: %s:%d\r\n"\
				"Range: bytes=%s\r\n"\
				"Accept:*/*\r\nAccept-Language: zh-cn\r\n"\
				"Connection: keep-alive\r\n"\
				"User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n"\
				"\r\n"

					
#define HTTP_GET "GET /%s HTTP/1.1\r\n"\
				"Host: %s:%d\r\n"\
				"Accept:*/*\r\nAccept-Language: zh-cn\r\n"\
				"Connection: keep-alive\r\n"\
				"User-Agent: Mozilla/4.0 (compatible; MSIE 5.01; Windows NT 5.0)\r\n"\
				"\r\n"

#if 0
struct host_info
{
	char *allocated;
	const char *path;
	char *user;
	const char *protocol;
	const *host;
	int port;
};
#endif

#endif
