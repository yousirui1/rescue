#include "base.h"
#include "http.h"

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



static int32_t http_packet_size(int sockfd)
{
    char buf[HTTP_BUFLEN] = {0};
    uint64_t file = 0;
    uint32_t pos = 0;
    for(;;)
    {
        if(recv(sockfd, buf + pos, 1, 0) < 0)
        {
            DEBUG("http send error sockfd %d error pos %d", sockfd, pos);   
            close_fd(sockfd);
            return -1;
        }   
        if(pos >= 1 && buf[pos - 1] =='\r' && buf[pos] == '\n')
            break;
    
        pos ++ ;

    }

    sscanf(buf, "%8X", &file);
    if(file == 0)
    {
        if(recv(sockfd, buf, 2, 0) < 0)
        {
            DEBUG("http send error sockfd %d error %s", sockfd, strerror(errno));   
            close_fd(sockfd);
            return -1;
        }   
        if(buf[0] == '\r' && buf[1] == '\n')
        {
            DEBUG("\r\n");
            return 0;
        }
        else
            DEBUG("file size error");
    }
    return -1;
}

static int http_parse_code(int sockfd)
{
#if 1
    int mem_size = 4096;
    int length = 0;
    int len;
    char *buf = (char *) malloc(mem_size * sizeof(char));
    char *response = (char *) malloc(mem_size * sizeof(char));

    //每次单个字符读取响应头信息
    puts("6: 正在解析http响应头...");
    while ((len = read(sockfd, buf, 1)) != 0)
    {
        if (length + len > mem_size)
        {
            //动态内存申请, 因为无法确定响应头内容长度
            mem_size *= 2;
            char * temp = (char *) realloc(response, sizeof(char) * mem_size);
            if (temp == NULL)
            {
                printf("动态内存申请失败\n");
                exit(-1);
            }
            response = temp;
        }

        buf[len] = '\0';
        strcat(response, buf);

        //找到响应头的头部信息
        int flag = 0;
        for (int i = strlen(response) - 1; response[i] == '\n' || response[i] == '\r'; i--, flag++);
        if (flag == 4)//连续两个换行和回车表示已经到达响应头的头尾, 即将出现的就是需要下载的内容
            break;

        length += len;
    }
    
    long file_size = 0;
    /*获取响应头的信息*/
    //struct HTTP_RES_HEADER resp;
    int code;

    char *pos = strstr(response, "HTTP/");
    if (pos)//获取返回代码
        sscanf(pos, "%*s %d", &code);

#if 0
    pos = strstr(response, "Content-Type:");
    if (pos)//获取返回文档类型
        sscanf(pos, "%*s %s", resp.content_type);
            pos = strstr(response, "Content-Length:");
    if (pos)//获取返回文档长度
        sscanf(pos, "%*s %ld", &resp.content_length);

    return resp;
#endif

    DEBUG("code %d",code);

    free(buf);
    free(response);

    
    return code;
#endif
#if 0
    char buf[HTTP_BUFLEN + 1];
    int ret;
    if((ret = recv(sockfd, buf, HTTP_BUFLEN, 0)) <= 0)
    {    
        close_fd(sockfd);
        return ERROR;
    }
    return SUCCESS;
#endif
}

int http_get(const char *url, char *pipe_buf, uint64_t offset, PedDevice *dev, uint64_t file_section)
{
    int sockfd = -1, ret;
    char host_ip[32];
    char file[MAX_FILENAMELEN];
    char buf[HTTP_BUFLEN + 1]; 
    int port = 0;
    int len = 0;
    time_t last_time = current_time;
    uint64_t file_blk =  (uint64_t)(file_section  / (HTTP_BUFLEN / 512));

    char range[32];
    uint64_t count = 0;
    int pos = 0;
    uint64_t download_rate = 0;
    uint32_t packet_len = 0;
    uint64_t total_want = 0;
    uint64_t total_done = 0;

    progress_info *info = (progress_info *)&pipe_buf[HEAD_LEN];
    
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

    DEBUG("host_ip %s file %s port %d", host_ip, file, port);

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1)
    {   
        DEBUG("unable to create socket");
        return ERROR;
    }

    ret = connect_server(sockfd, host_ip, port, TIME_OUT);
    if(ret != SUCCESS)
    {    
        DEBUG("connect http ip: %s port: %d timeout 10s", host_ip, port);
        close_fd(sockfd);
        return ERROR; 
    }    

       sprintf(buf, HTTP_GET, file, host_ip, port);    
    if(send_msg(sockfd, buf, strlen(buf)) !=  SUCCESS)
    {
        DEBUG("http send error sockfd %d error %s", sockfd, strerror(errno));   
        close_fd(sockfd);
        return ERROR;
    }

    int code = http_parse_code(sockfd);
    
    info->file_size = (uint64_t)file_section * 512;

    strcpy(info->state, "downloading");
    _device_seek(dev->fd, offset);

    //FILE *fp = fopen("test", "wb");
    uint32_t data_len = 0;
    int first = 1;

    for(;;)
    {
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

        if(total_want <= 0)
        {
            if(first)
            {
                first = 0;  
            }
            else
            {
                ret = recv(sockfd, buf, 2, 0);
            }

            total_want = http_packet_size(sockfd);
            pos = 0;
            if(total_want == 0)
            {
                info->progress = 99;
                ret = SUCCESS;
                DEBUG("download finish");
                break;
            }
                        //packet = (total_want + HTTP_BUFLEN - 1) / HTTP_BUFLEN;
            data_len = min(total_want, HTTP_BUFLEN);
        }

        /* recv 4K */
        if((ret = recv(sockfd, buf + pos, data_len - pos, 0)) <= 0)
        {    
            if(ret < 0) 
            {    
                if(errno == EINTR || errno == EAGAIN)
                {    
                    continue;
                }    
            }    
            DEBUG("close client fd: %d", sockfd);
            info->progress = 0;
            ret = ERROR;
            break;
        }
        total_want -= ret;
        download_rate += ret;
        info->total_size += ret;    
        pos += ret;
        
        if(pos == data_len)
        {
            if(pos != HTTP_BUFLEN)
            {
                DEBUG("123123!!!!!!!!!!!!!!!_____________________________");    
            }

            write_sector(dev->fd, buf, HTTP_BUFLEN / 512);
            data_len = min(total_want, HTTP_BUFLEN);
            pos = 0;    
        }
        if(pos > data_len)
        {
            data_len = min(total_want, HTTP_BUFLEN);
            pos = 0;
        }

        //DEBUG("ret %d", ret);
    }
    //fclose(fp);

    close_fd(sockfd);
    return ret;
}
