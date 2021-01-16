#include <stdio.h>
#include <direct.h>
#include <stdio.h>  
#include <stdlib.h>  
#include <string.h>  
#define MAXPATH  1024
  
/** 
 * 统计key在data中出现的次数 
 * @param data 待查找的字符串 
 * @param key  要查找的字符串 
 * @return key在data中出现的次数 
 */  
int _count_string(char *data, char *key)  
{  
    int count = 0;  
    int klen = strlen(key);  
    char *pos_start = data, *pos_end;  
    while (NULL != (pos_end = strstr(pos_start, key))) {  
        pos_start = pos_end + klen;  
        count++;  
    }  
    return count;  
}  
  
/** 
 * 将data中的rep字符串替换成to字符串，以动态分配内存方式返回新字符串 
 * 这个函数不需要保证data能保证容量。 
 * @param data 待替换某些字符串的数据 
 * @param rep  待替换的字符串 
 * @param to   替换成的字符串 
 * @param free_data 不为0时要释放data的内存 
 * @return 返回新分配内存的替换完成的字符串，注意释放。 
 */  
char *malloc_replace(char *data, char *rep, char *to, int free_data)  
{  
    int rep_len = strlen(rep);  
    int to_len  = strlen(to);  
    int counts  = _count_string(data, rep);  
    printf("counts = %d\n", counts);  
    int m = strlen(data) + counts * (to_len - rep_len);  
    char *new_buf = (char *) malloc(m + 1);  
    if (NULL == new_buf) {  
        free(data);  
        return NULL;  
    }  
    memset(new_buf, 0, m + 1);  
    char *pos_start = data, *pos_end, *pbuf = new_buf;  
    int copy_len;  
    while (NULL != (pos_end = strstr(pos_start, rep))) {  
        copy_len = pos_end - pos_start;  
        strncpy(pbuf, pos_start, copy_len);  
        pbuf += copy_len;  
        strcpy(pbuf, to);  
        pbuf += to_len;  
        pos_start  = pos_end + rep_len;  
    }  
    strcpy(pbuf, pos_start);  
    if (free_data)  
        free(data);  
    return new_buf;  
}  
  
/** 
 * 将data中的rep字符串替换成to字符串 
 * 注意保证data空间足够替换完成后的字符串长度 
 * @param data 待替换某些字符串的数据 
 * @param rep  待替换的字符串 
 * @param to   替换成的字符串 
 * @return 无 
 */  
void normal_replace(char *data, char *rep, char *to)  
{  
    char *new_buf = malloc_replace(data, rep, to, 0);  
    if (NULL != new_buf) {  
        strcpy(data, new_buf);  
        free(new_buf);  
    }  
}  

char * strrepl(const char * src, char * dst, size_t dst_size, const char * search, const char * replace_with) {
    char * replace_buf = (char *)malloc(dst_size);
    if (replace_buf) {
        replace_buf[0] = 0;
        char * p = (char *)src;
        char * pos = NULL;
        while ( (pos = strstr(p, search)) != NULL ) {
            size_t n = (size_t)(pos-p);
            strncat(replace_buf, p, n > dst_size ? dst_size : n);
            strncat(replace_buf, replace_with, dst_size-strlen(replace_buf)-1);
            p = pos + strlen(search);
        }
        snprintf(dst, dst_size, "%s%s", replace_buf, p);
        free(replace_buf);
    }
    return dst;
}


int main()
{
   char* buffer;
	char path[128] = {0};
   if ( (buffer = _getcwd( NULL, 0 )) == NULL )
      perror( "_getcwd error" );
   else
   {
		printf( "%s \nLength: %zu\n", buffer, strlen(buffer) );

		//normal_replace(buffer, "\\", "//");
		//buffer = strrepl(buffer, "\\",)
		//printf("path %s\n",path);
		printf( "%s \nLength: %zu\n", buffer, strlen(buffer) );
		//free(buffer);
   }
}

