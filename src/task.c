#include "base.h"
#include "task.h"
#include "device.h"
#include "socket.h"
#include "queue.h"
#include "bt_client.h"



QUEUE task_queue;
unsigned char *task_queue_buf = NULL;

static int task_bt(char *data, int length)
{
	int i, ret = ERROR;
	uint64_t offset = 0;
	struct torrent_task *task = (struct torrent_task *)data;

	char buf[HEAD_LEN + sizeof(progress_info) + 1] = {0};
    progress_info *info = (progress_info *)&buf[HEAD_LEN];
	
	strcpy(info->file_name, task->file_name);
	
	if(task->diff == 1 && task->diff_mode == INCRMENT_MODE)
	{
		offset  = add_qcow2(dev_info.mini_disk->dev, task->uuid, task->diff, (uint64_t)(task->real_size),
					 task->real_size, 1, task->disk_type, task->operate_id, INCRMENT_MODE);
	}
	else	
	{
		offset  = add_qcow2(dev_info.mini_disk->dev, task->uuid, task->diff,
							(uint64_t)(task->file_size) + 1024 * 2, task->real_size, 1, 
							task->disk_type, task->operate_id, COVERAGE_MODE);
	}

	if(offset != 0)
	{
		ret = start_torrent(task->torrent_file, dev_info.mini_disk->dev->path, buf, task->diff_mode,
				(uint64_t)offset * 512);
		if(SUCCESS == ret)
		{
			save_qcow2(dev_info.mini_disk->dev);
			if(task->diff != 0)
			{
				if(change_back_file_qcow2(dev_info.mini_disk->dev, task->uuid ,task->diff) == SUCCESS)
				{
					set_boot_qcow2(dev_info.mini_disk->dev, task->diff, task->disk_type, task->uuid);
					DEBUG("uuid: %s change_back_file_qcow2  %d -> %d ok !!!", task->uuid, task->diff, task->diff - 1);
					info->progress = 100;
					ret = SUCCESS;
				}
				else
				{
					info->progress = 0;
					del_qcow2(dev_info.mini_disk->dev, task->uuid, task->diff);
					save_qcow2(dev_info.mini_disk->dev);
				}
			}	
			else
			{
				info->progress = 100;
				ret = SUCCESS;
				set_boot_qcow2(dev_info.mini_disk->dev, task->diff, task->disk_type, task->uuid);
			}
		}
		else
		{
			info->progress = 0;
			del_qcow2(dev_info.mini_disk->dev, task->uuid, task->diff);
			save_qcow2(dev_info.mini_disk->dev);
		}
	}
	else
	{
		DEBUG("alloc qcow2 space error %d ", offset);
		info->progress = 0;
		send_error_msg(BT_DISK_FULL_ERR);
	}

    strcpy(info->state, "finished");
    send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);

	return ret;
}
	
static void task_tftp(char *data, int length)
{
	int ret;
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};
    char buf[HEAD_LEN + sizeof(progress_info) + 1] = {0};
    progress_info *info = (progress_info *)&buf[HEAD_LEN];
    info->type = 2;
    struct tftp_task *task = (struct tftp_task *)data;

    if(strlen(task->file_name) > 0)
        strcpy(info->file_name, task->file_name);
        
    ret = tftp_get(task->server_ip, task->remote_file, task->local_file, buf, task->type);
    if(ret != SUCCESS)
    {   
        DEBUG("tftp tftp_get timeout");
        send_error_msg(INSTALL_ERR);
    }   
	else
	{	
	    if(STRPREFIX (task->remote_file, "vmlinuz-5.2.8-lfs-9.0"))
	    {   
	        DEBUG("mv /boot/linux/vmlinuz-5.2.8-lfs-9.0_new /boot/linux/vmlinuz-5.2.8-lfs-9.0");
	        exec_cmd("mv /boot/linux/vmlinuz-5.2.8-lfs-9.0_new /boot/linux/vmlinuz-5.2.8-lfs-9.0",result);
	        if(task->type == 3)
	        {   
	            info->progress = 100;
	            send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_UI);
	        }   
	    }   
	
	    if(task->type == 2)
	    {   
	        DEBUG("install programe ok");
	        conf.install_flag = 1;
	        exec_cmd(upgrad_sh, result);
	        if(strstr(result, "successd"))
	        {   
	            exec_cmd("mkdir -p /boot/conf", result);
	            strcpy(config_file, "/boot/conf/config.ini");
	            DEBUG("install_flag %d", conf.install_flag);
	            save_config();

	        	info->progress = 100;
	        	send_pipe(buf, INSTALL_DONE ,sizeof(progress_info), PIPE_EVENT);
	        }   
	        else
	        {   
	            send_error_msg(INSTALL_ERR);
				reboot(RB_AUTOBOOT);	
	        }   
	    }   
	}
    DEBUG("tftp_get end ");
}

static void task_p2v(char *data, int length)
{
	struct p2v_task * task = (struct p2v_task *)data;
    p2v_transform(task->server_ip, task->user, task->passwd,
                    task->storage, task->image_name,
                    dev_info.mini_disk->dev->path);
}

void task_loop()
{
	int ret, maxfd = -1;
	char buf[DATA_SIZE] = {0};
	
	struct timeval tv;	

	fd_set reset, allset;
	FD_ZERO(&allset);
	FD_SET(pipe_task[0], &allset);
	
	maxfd = maxfd > pipe_task[0] ? maxfd : pipe_task[0];
	QUEUE_INDEX *index = NULL;
	
	for(;;)
	{
		reset = allset;
		tv.tv_sec = 1;		
		tv.tv_usec = 0;
	
		ret = select(maxfd + 1, &reset, NULL, NULL, &tv);	

        if(-1 == ret)
        {   
            if(ret == EINTR)
                continue;
            else if(ret != EBADF)
            {   
                FAIL("task select ret: %d errno: %s", ret, strerror(ret));
                continue;
            }   
        }  
		/* pipe msg */
		if(FD_ISSET(pipe_task[0], &reset))
		{
			ret = read(pipe_task[0], (void *)buf, sizeof(buf));	
			if(ret == 1 && buf[0] == 'S')
			{
				DEBUG("");
				break;
			}
			if(ret >= HEAD_LEN)
			{
				//process();
			}
		}			
		
		/* task */
		{
			if(empty_queue(&task_queue))
				continue;

			index = de_queue(&task_queue);
			switch(index->ucType)
			{
				case TASK_BT:
					ret = task_bt(index->pBuf, index->uiSize);
					break;
				case TASK_P2V:
					task_p2v(index->pBuf, index->uiSize);
					break;
				case TASK_TFTP:
					task_tftp(index->pBuf, index->uiSize);
					break;	
				default:
					break;
			}
			de_queuePos(&task_queue);

			if(ret != SUCCESS)
			{
				clear_task_queue();
			}
		}
	}
	free(task_queue_buf);
	task_queue_buf = NULL;
}


void clear_task_queue()
{
	clear_queue(&task_queue);
}

int init_task()
{
	task_queue_buf = (unsigned char *)malloc(MAX_TASKBUFSIZE * sizeof(unsigned char));	

	if(!task_queue_buf)
		DIE("task_queue_buf malloc size: %d error: %s", MAX_TASKBUFSIZE, strerror(errno));
	
	init_queue(&task_queue, task_queue_buf, MAX_TASKBUFSIZE);
	return SUCCESS;
}

void *thread_task(void *param)
{
    int ret;
    pthread_attr_t st_attr;
    struct sched_param sched;
    
    ret = pthread_attr_init(&st_attr);
    if(ret)
    {    
        DEBUG("thread task attr init warning ");
    }    
    ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
    if(ret)
    {    
        DEBUG("thread task set SCHED_FIFO warning");
    }    
    sched.sched_priority = SCHED_PRIORITY_EVENT;
    ret = pthread_attr_setschedparam(&st_attr, &sched);

	init_task();
	task_loop();
	
	return (void *)SUCCESS;
}
