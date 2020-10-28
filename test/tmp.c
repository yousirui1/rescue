#if 0
                        if(update_flag)
                        { if(!strcmp(uuid->valuestring, current_uuid))
                            {
                                DEBUG("update qcow2 ");
                                DEBUG("dif_level:%d prefix %s real_size %s reserve_size %s \n"
                                    "type: %d uuid: %s",dif_level->valueint, prefix->valuestring, real_size->valuestring,
                                                    reserve_size->valuestring, type->valueint, uuid->valuestring );
                                send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 
                                                        dif_level->valueint );
                            }
                            else
                            {
                                update_flag = 0;
                                current_diff = -1;
                            }
                        }

                        /* 存在不同更新 */
                        if(dif_level->valueint == 0 && 
                            max_diff->valueint != (current_diff = get_max_diff_qcow2(uuid->valuestring)) &&
                            current_diff != -1)
                            
                        {
                            DEBUG("current_diff %d", current_diff);
                            DEBUG("max_diff->value %d", max_diff->valueint);
                            DEBUG("del current uuid: %s all diff", uuid->valuestring);
                            del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);     //删除当前的不同的驱动
                            strcpy(current_uuid, uuid->valuestring);
                            update_flag = 1;
                        }
#endif


							if(scan_qcow2(uuid->valuestring, 0) && !scan_qcow2(uuid->valuestring, 1))
							{
								DEBUG("update qcow2 %s download diff all", uuid->valuestring);
								del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);
								send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 1);
								memset(m_desktop_group_name, 0, sizeof(m_desktop_group_name));
								if(desktop_group_name)
									strcpy(m_desktop_group_name, desktop_group_name->valuestring);
							}
							else if(operate_id->valueint != get_operate_qcow2(uuid->valuestring, 1))
							{
							if(diff_mode->valueint)	//模式1 diff 1 full disk and commit diff 1,2 -> 1
							{
								if(operate_id->valueint == get_operate_qcow2(uuid->valuestring, 1) + 1)		
								{
									item = cJSON_GetArrayItem(disks, 2);
									if(!item)
										continue;
									dif_level = cJSON_GetObjectItem(item, "dif_level");
									operate_id = cJSON_GetObjectItem(item, "operate_id");
									if(!dif_level || !operate_id)
										continue;
									if(operate_id->valueint == get_operate_qcow2(uuid->valuestring, 2) + 1 && !(scan_qcow2(uuid->valuestring, 3)))
									{
										DEBUG("update qcow2 %s download diff 3 and commit ", uuid->valuestring);
										del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 4);
										del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 5);
										send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 3);
										memset(m_desktop_group_name, 0, sizeof(m_desktop_group_name));
										if(desktop_group_name)
											strcpy(m_desktop_group_name, desktop_group_name->valuestring);
										//;		下载合并
										continue;
									}
								}							
							}
							DEBUG("uuid->valuestring %s ", uuid->valuestring);
							if(scan_qcow2(uuid->valuestring, 0)) // 模式0  update diff del 1,2 download 1, 2
							{
								DEBUG("update qcow2 %s download diff all", uuid->valuestring);
								del_diff_qcow2(dev_info.mini_disk->dev, uuid->valuestring);
								send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 1);
								memset(m_desktop_group_name, 0, sizeof(m_desktop_group_name));
								if(desktop_group_name)
									strcpy(m_desktop_group_name, desktop_group_name->valuestring);
							}
							}
						}
						if(dif_level->valueint == 2)
						{
							if(scan_qcow2(uuid->valuestring, 0)  && !(scan_qcow2(uuid->valuestring, 2))) 
							{
								del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 4);
								del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 5);
								send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 2);
								memset(m_desktop_group_name, 0, sizeof(m_desktop_group_name));
								if(desktop_group_name)
									strcpy(m_desktop_group_name, desktop_group_name->valuestring);
							}
							
							if(operate_id->valueint > (get_operate_qcow2(uuid->valuestring, 2) + 1))
							{
								if(scan_qcow2(uuid->valuestring, 0) && scan_qcow2(uuid->valuestring, 1)) 
								{
									del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 4);
									del_qcow2(dev_info.mini_disk->dev, uuid->valuestring, 5);
									send_get_diff_torrent(cli, desktop_group_uuid->valuestring, uuid->valuestring, 2);
									memset(m_desktop_group_name, 0, sizeof(m_desktop_group_name));
									if(desktop_group_name)
										strcpy(m_desktop_group_name, desktop_group_name->valuestring);
								}
							}
						}	


#if 0
    struct torrent_task task = {0}; 
    memcpy(task.torrent_file, "123", 3);
    task.offset = 10001;
    en_queue(&task_queue, (char *)&task, sizeof(struct torrent_task) , 0x0);


    memcpy(task.torrent_file, "654", 3);
    task.offset = 10002;
    en_queue(&task_queue, (char *)&task, sizeof(struct torrent_task) , 0x0);
#endif



#if 0
#endif

#if 0
    int ret;
    uuid_t uuid;
    char str[36];
    uuid_generate(uuid);
    uuid_unparse(uuid, str);

    DEBUG("%s", str);
    
    //format_disk(argv[1]);
    uint64_t sizeLba;
    uint64_t realLba;
    sizeLba = (uint64_t )20 * 1024 * 1024 * 1024 / 512;
    realLba = sizeLba;
    init_qcow2(dev, 0);
    uint64_t offset = add_qcow2(dev, str, 0, sizeLba, realLba);

    DEBUG("offset %lld", offset);
    time_t current_time;
    time_t last_time;
    (void *)time(&current_time);

    last_time = current_time;
    if(offset != -1)
    {   
#if 0
        DEBUG("offset %lld", offset);
        //start_torrent("win10.torrent", dev_info.mini_disk->dev->path, offset * 512);
        FILE *fp = fopen("win10_base", "rb");
        if(!fp)
        {
            DEBUG("fopem error");
            return;
        }

        fseek(fp, 0, SEEK_END);
        uint64_t fileSize = ftell(fp);
        DEBUG("fileSize %ul", fileSize);

        fseek(fp, 0, SEEK_SET);

        _device_seek(dev->fd, offset); 
        
        char buf[4096] = {0};
        for (uint64_t i = 0; i < fileSize + 4095;)
        {   
            ret = fread(buf, 1, 4096, fp);
            if (ret < 0)
            {   
                DEBUG("fread error");   
            }   
            write_sector(dev->fd, buf, 8); 
            i += 4096;
    
            (void *)time(&current_time);
            if(current_time - last_time > 2)
            {
                DEBUG("fileSize %lld", fileSize - i);
                last_time = current_time;   
            }
        }
#endif
    }
#if 1
    DEBUG("%s", str);
    
    sizeLba = (uint64_t )10 * 1024 * 1024 * 1024 / 512;
    realLba = sizeLba;
    offset = add_qcow2(dev, str, 1, sizeLba, realLba);
    DEBUG("offset %lld", offset);
#endif
    if(offset != -1)
    {   
#if 0
        DEBUG("offset %lld", offset);
        FILE *fp = fopen("win10_diff", "rb");
        if(!fp)
        {
            DEBUG("fopem error");
            return;
        }

        fseek(fp, 0, SEEK_END);
        uint64_t fileSize = ftell(fp);
        DEBUG("fileSize %d", fileSize);


        fseek(fp, 0, SEEK_SET);

        _device_seek(dev->fd, offset); 
        
        char buf[4096] = {0};
        for (uint64_t i = 0; i < fileSize + 4095;)
        {   
            ret = fread(buf, 1, 4096, fp);
            if (ret < 0)
            {   
                DEBUG("fread error");   
            }   
            write_sector(dev->fd, buf, 8); 
            i += 4096;
            (void *)time(&current_time);
            if(current_time - last_time > 2)
            {
                DEBUG("fileSize %lld", fileSize - i);
                last_time = current_time;   
            }
        }
#endif
    }
    DEBUG("offset %lld", offset);
    if(change_back_file_qcow2(dev, str ,1) != SUCCESS)
    {
        DEBUG("back file error");
    }

#if 0
    int ret;
    srandom(time(NULL) + getpid());
    (void)time(&current_time);

    signal(SIGPIPE, SIG_IGN);
    //signal(SIGINT, SIG_IGN);
    struct sigaction act;
    act.sa_handler = sig_quit_listen;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGUSR1, &act, 0); 

    init_logs();
    init_pipe();
    init_config();
    init_device();
    
    network_fd = get_netcard_state();

    ret = pthread_create(&pthread_client, NULL, thread_client, NULL);
    if(0 != ret)
    {
        DIE("create  tcp thread ret: %d error: %s", ret, strerror(ret));
    }   

    ret = pthread_create(&pthread_event, NULL, thread_event, (void *)&network_fd);
    if(0 != ret)
    {
        DIE("create  tcp thread ret: %d error: %s", ret, strerror(ret));
    }

    ret = pthread_create(&pthread_qt, NULL, thread_qt, NULL);
    if(0 != ret)
    {
        DIE("create qt thread ret: %d error: %s", ret, strerror(ret));
    }
#endif
    
#if 0
    uuid_t uuid;
    char str[36];
 
    uuid_generate(uuid);
    uuid_unparse(uuid, str);
 
   DEBUG("%s", str);

    uint64_t sizeLba;
    uint64_t realLba;
    sizeLba = (uint64_t )30 * 1024 * 1024 * 1024 / 512;
    realLba = sizeLba;
    DEBUG("%lu", sizeLba);

    uint64_t offset = add_qcow2(dev_info.mini_disk->dev, str, 0, sizeLba, realLba);
        
    DEBUG("offset %lld", offset);
    if(offset != -1)
    {   
        DEBUG("offset %lld", offset);
        //start_torrent("win10.torrent", dev_info.mini_disk->dev->path, offset * 512);
    }   

    uuid_generate(uuid);
    uuid_unparse(uuid, str);
 
    offset = add_qcow2(dev_info.mini_disk->dev, str, 0, sizeLba, realLba);
        
    DEBUG("offset %lld", offset);
    if(offset != -1)
    {   
        DEBUG("offset %lld", offset);
        start_torrent("win10.torrent", dev_info.mini_disk->dev->path, offset * 512);
    }   


    PedDevice *dev = linux_new(argv[1]);
    //format_disk(argv[1]);

    uint64_t sizeLba;
    uint64_t realLba;
    sizeLba = (uint64_t )20 * 1024 * 1024 * 1024 / 512;
    realLba = sizeLba;

    uuid_t uuid;
    char str[36];
    uuid_generate(uuid);
    uuid_unparse(uuid, str);

    DEBUG("%s", str);
    init_qcow2(dev, 0);
 
    uint64_t offset = add_qcow2(dev, str, 0, realLba, realLba, 3, 0);
    if(offset != -1)
    {
        start_torrent("win10_base.torrent", dev->path, offset * 512);
    }
    DEBUG("-----------!!!!!!!!!!!!!!!!!!------------");

    sizeLba = (uint64_t )5 * 1024 * 1024 * 1024 / 512;
    realLba = sizeLba;

    offset = add_qcow2(dev, str, 1,  realLba, realLba, 3, 0);
    if(offset != -1)
    {
        start_torrent("win10_diff.torrent", dev->path, offset * 512);
    }
    else
    {
        DEBUG("offset %d", offset);
    }

    if(change_back_file_qcow2(dev, str , 1) != SUCCESS)
    {    
        DEBUG("back file error");
    }   
#endif

    //p2v_transform("192.169.27.215", "voi_guest", "qwe123,.", "template-voi", "win10-test-ysr");

    //atexit(reboot(RB_AUTOBOOT));
    #endif

 //p2v_transform("192.169.27.216", "root", "123qwe,.", "template-voi", "test");
