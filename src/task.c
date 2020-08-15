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
            struct torrent_task * task = (struct torrent_task *)index->pBuf;
            DEBUG("index->torrent_file %s", task->torrent_file);
            DEBUG("index->torrent_file %d", task->offset);
            DEBUG("dev_info.mini_disk->dev->path %s", dev_info.mini_disk->dev->path);
            start_torrent(task->torrent_file, dev_info.mini_disk->dev->path, task->file_name, (uint64_t)task->offset * 512); 
            if(task->diff != 0)
            {   
                if(change_back_file_qcow2(dev_info.mini_disk->dev, task->uuid ,task->diff) == SUCCESS)
                {   
					set_boot_qcow2(dev_info.mini_disk->dev, task->diff, task->disk_type, task->uuid);
					DEBUG("change_back_file_qcow2 ok !!!");
                }   
            }   
			else
				set_boot_qcow2(dev_info.mini_disk->dev, task->diff, task->disk_type, task->uuid);
			DEBUG("task->diff %d", task->diff);
			DEBUG("task->uuid %s", task->uuid);
        }   
        else
        {   
            struct p2v_task * task = (struct p2v_task *)index->pBuf;
            //if(dev_info.boot_disk)
            {   
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
        de_queuePos(&task_queue);
    }  
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
