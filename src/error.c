#include "base.h"


const char err_msg_desc[][128] = { 
                "安装失败",
				"没有找到可用的磁盘",
				"未检测到安装U盘",
				"格式化硬盘失败"
                "P2V 上传系统失败,\n请关闭Window硬盘快速启动.",
				"P2V 磁盘错误,上传失败",
				"P2V 失败"
                "硬盘容量不足下载失败",
            };  


int send_error_msg(int err_type)
{
	char head[HEAD_LEN + 4] = {0}; 
	*(int *)(&head[HEAD_LEN]) = err_type;
	return send_pipe(head, ERROR_MSG_PIPE, 4, PIPE_UI);
}

