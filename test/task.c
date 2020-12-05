#if 0
static void task_bt(char *data, int length)
{
    int i, ret;
    uint64_t offset = 0;
    struct desktop_task *task = (struct desktop_task *)data;
    char file_name[128] = {0};
    
    
    for(i = 0; i < task->size; i++)
    {
        DEBUG("torrent_file: %s", task->bt[i].torrent_file);
        DEBUG("uuid: %s dif_level: %d real_size: %llu space_size: %llu file_size %llu operate_id %d type %d", 
            task->bt[i].uuid,task->bt[i].diff, task->bt[i].real_size, task->bt[i].space_size, task->bt[i].file_size, 
            task->bt[i].operate_id,task->bt[i].disk_type);

        if (task->bt[i].disk_type == 0)
        {    
            sprintf(file_name, "%s_系统盘_%d", task->group_name, task->bt[i].diff);
        }    
        else if (task->bt[i].disk_type == 1)
        {    
            sprintf(file_name, "%s_数据盘_%d", task->group_name, task->bt[i].diff);
        }    
        else if (task->bt[i].disk_type == 2)
        {    
#if 0
            if(torrent->operate_id == get_operate_qcow2(uuid, 0)) //共享盘已存在不再下载
            {    
                DEBUG("share disk data found operated equal: %d", torrent->operate_id);
                return SUCCESS;
            }    
            else 
            {    
                strcpy(task.file_name, "共享盘");
                del_qcow2(dev_info.mini_disk->dev, uuid, 0);
                del_diff_qcow2(dev_info.mini_disk->dev, uuid);
            }    
#endif
        }  
        if(task->bt[i].diff == 1 && !task->diff_mode)
        {
            offset  = add_qcow2(dev_info.mini_disk->dev, task->bt[i].uuid, task->bt[i].diff, (uint64_t)(task->bt[i].real_size),
                         task->bt[i].real_size, 1, task->bt[i].disk_type, task->bt[i].operate_id, INCRMENT_MODE);
        }
        else    
        {
            offset  = add_qcow2(dev_info.mini_disk->dev, task->bt[i].uuid, task->bt[i].diff,
                                (uint64_t)(task->bt[i].file_size) + 1024 * 2, task->bt[i].real_size, 1, 
                                task->bt[i].disk_type, task->bt[i].operate_id, INCRMENT_MODE);
        }
        if(offset != 0)
        {
            ret = start_torrent(task->bt[i].torrent_file, dev_info.mini_disk->dev->path, file_name, task->diff_mode,
                    (uint64_t)offset * 512);
            if(SUCCESS == ret)
            {
                save_qcow2(dev_info.mini_disk->dev);
                if(task->bt[i].diff != 0)
                {
                    if(change_back_file_qcow2(dev_info.mini_disk->dev, task->bt[i].uuid ,task->bt[i].diff) == SUCCESS)
                    {
                        set_boot_qcow2(dev_info.mini_disk->dev, task->bt[i].diff, task->bt[i].disk_type, task->bt[i].uuid);
                        DEBUG("uuid: %s change_back_file_qcow2  %d -> %d ok !!!", task->bt[i].uuid, task->bt[i].diff, 
                                task->bt[i].diff - 1);
                    }
                    else
                    {
                        del_qcow2(dev_info.mini_disk->dev, task->bt[i].uuid, task->bt[i].diff);
                        save_qcow2(dev_info.mini_disk->dev);
                        DEBUG("");
                        break;
                    }
                }   
                else
                    set_boot_qcow2(dev_info.mini_disk->dev, task->bt[i].diff, task->bt[i].disk_type, task->bt[i].uuid);
            }
            else
            {
                DEBUG("download fail ");
                del_qcow2(dev_info.mini_disk->dev, task->bt[i].uuid, task->bt[i].diff);
                save_qcow2(dev_info.mini_disk->dev);
                break;
            }
        }
        else
        {
            DEBUG("alloc qcow2 space error %d ", offset);
            break;
        }
    }       
    init_qcow2(dev_info.mini_disk->dev, 1);     //下载完一个桌面load 一下节点
    DEBUG("bt download desktop_name %s finish", task->group_name);
}

