#include "base.h"
#include "queue.h"
#include "task.h"
#include "device.h"
#include "torrent.h"

QUEUE task_queue;

extern struct device_info dev_info;

void task_loop()
{
    QUEUE_INDEX *index = NULL;
	int ret;
    for(;;)
    {   
        if(empty_queue(&task_queue))
        {   
            sleep(1);
            continue;
        }   

        index = de_queue(&task_queue);
        if(index->ucType == 0x00)
        {   
    		char buf[HEAD_LEN + sizeof(progress_info) + 1] = {0};
    		progress_info *info = (progress_info *)&buf[HEAD_LEN];
            struct torrent_task * task = (struct torrent_task *)index->pBuf;
            DEBUG("index->torrent_file %s", task->torrent_file);
            DEBUG("index->torrent_file %d", task->offset);
            DEBUG("dev_info.mini_disk->dev->path %s", dev_info.mini_disk->dev->path);
            ret = start_torrent(task->torrent_file, dev_info.mini_disk->dev->path, task->file_name, (uint64_t)task->offset * 512); 
			if(ret != SUCCESS)			//下载失败
			{
				//DEBUG("del qcow2 uuid %s diff %d", task->uuid, task->diff);	
				del_qcow2(dev_info.mini_disk->dev, task->uuid, task->diff);
				//del_diff_qcow2(dev_info.mini_disk->dev, task->uuid);
				DEBUG("clear_task !!!!!!!!!!!!!!!");
				clear_task(&task_queue);		
				continue;
			}
			save_qcow2(dev_info.mini_disk->dev);

            if(task->diff != 0)
            {   
                if(change_back_file_qcow2(dev_info.mini_disk->dev, task->uuid ,task->diff) == SUCCESS)
                {   
					set_boot_qcow2(dev_info.mini_disk->dev, task->diff, task->disk_type, task->uuid);
					DEBUG("change_back_file_qcow2 ok !!!");
                }   
				else
				{
					DEBUG("change_back_file error del_qcow2 uuid %s diff %d", task->uuid, task->diff);
					del_qcow2(dev_info.mini_disk->dev, task->uuid, task->diff);
				}
            }   
			else
				set_boot_qcow2(dev_info.mini_disk->dev, task->diff, task->disk_type, task->uuid);
			
			strcpy(info->state, "finished");
			info->type = 0;

			strcpy(info->file_name, task->file_name);
			sscanf(task->torrent_file, "/root/%s", info->image_name);
            info->progress = 100;
            send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_EVENT);
			DEBUG("task->diff %d", task->diff);
			DEBUG("task->uuid %s", task->uuid);
        }   
        else if(index->ucType == 0x02)
        {   
            struct p2v_task * task = (struct p2v_task *)index->pBuf;
            //if(dev_info.boot_disk)
            {   
				DEBUG("p2v_transform");
                p2v_transform(task->server_ip, task->user, task->passwd, 
                            task->storage, task->image_name, 
							dev_info.mini_disk->dev->path);
                            //dev_info.boot_disk->dev->path);
	
            }   
			//else
			{
				//DEBUG("no find window boot disk ");
			}
        }   
		else if(index->ucType == 0x03)
		{
			char result[MAX_BUFLEN] = {0};
    		char cmd[MAX_BUFLEN] = {0};
    		char buf[HEAD_LEN + sizeof(progress_info) + 1] = {0};
    		progress_info *info = (progress_info *)&buf[HEAD_LEN];
			info->type = 2;
			struct tftp_task *task = (struct tftp_task *)index->pBuf;

			if(strlen(task->file_name) > 0)
				strcpy(info->file_name, task->file_name);
			
			ret = tftp_get(task->server_ip, task->remote_file, task->local_file, buf, task->type);
			if(ret != SUCCESS)
			{
				send_error_msg(INSTALL_ERR);
				clear_task(&task_queue);		
				continue;
			}
			if(STRPREFIX (task->remote_file, "vmlinuz-5.2.8-lfs-9.0"))
			{
				
				DEBUG("/boot/linux/vmlinuz-5.2.8-lfs-9.0_new /boot/linux/vmlinuz-5.2.8-lfs-9.0");
				exec_cmd("mv /boot/linux/vmlinuz-5.2.8-lfs-9.0_new /boot/linux/vmlinuz-5.2.8-lfs-9.0",result);
				if(task->type == 3)
				{
            		info->progress = 100;
            		send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);
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
            	}
				else
				{
					send_error_msg(INSTALL_ERR);
				}

            	info->progress = 100;
            	send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);
			}
		}
        de_queuePos(&task_queue);
    }  
}

void clear_task()
{
	clear_queue(&task_queue);
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
		DEBUG("thread task attr init warning ");
	}	
	sched.sched_priority = SCHED_PRIORITY_TASKT;
	ret = pthread_attr_setschedparam(&st_attr, &sched);
	
	unsigned char *task_buf = (unsigned char *)malloc(MAX_VIDSBUFSIZE * sizeof(unsigned char)); 
	
	if(!task_buf)
		DIE("task_buf malloc size: %d error: %s", MAX_VIDSBUFSIZE, strerror(errno) );

	init_queue(&task_queue, task_buf, MAX_VIDSBUFSIZE);

	task_loop();
	return (void *)ret;	
}
