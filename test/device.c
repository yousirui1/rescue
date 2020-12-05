#if 0
    //if(SUCCESS == tftp_get(server->ip, "vmlinuz-5.2.8-lfs-9.0", "/boot/linux/vmlinuz-5.2.8-lfs-9.0", buf , 1))
    {
        if(SUCCESS == tftp_get(server->ip, "voi.zip", "/root/voi.zip", buf, 2))
        {
            DEBUG("install programe ok");
            conf.install_flag = 1;
            memset(result, 0, MAX_BUFLEN);
            exec_cmd(upgrad_sh, result);
            if(strstr(result, "successd"))
            {
                info->progress = 100;
                send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);

                exec_cmd("mkdir -p /boot/conf", result);
                strcpy(config_file, "/boot/conf/config.ini");
                DEBUG("install_flag %d", conf.install_flag);
                return SUCCESS;
            }
        }
    }
    
    DEBUG("install programe error %s", result);
    conf.install_flag = 0;
    umount_boot();
    send_error_msg(INSTALL_ERR);
    DEBUG("install_flag %d", conf.install_flag);
    return ERROR;
#endif


#if 0
    sprintf(cmd, upgrad_sh, server->ip);
    DEBUG("%s", cmd);
    exec_cmd(cmd, result);

    info->progress = 50;

    send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);

    memset(result, 0, MAX_BUFLEN);
    sprintf(cmd, upgrad_sh_1, server->ip);
    DEBUG("%s", cmd);
    exec_cmd(cmd, result);


    //DEBUG("dev_info.mini_disk->name %s", dev_info.mini_disk->name);   
    //DEBUG("dev_info.usb_disk->name %s", dev_info.usb_disk->name); 
    //sprintf(cmd, install_sh, dev_info.mini_disk->name, dev_info.usb_disk->name);

    //exec_cmd(cmd, result);
//  DEBUG("%s", cmd);

void test_programe()
{
    char result[MAX_BUFLEN] = {0};
    char cmd[MAX_BUFLEN] = {0};
    char buf[HEAD_LEN + sizeof(progress_info) + 1] = {0};
    progress_info *info = (progress_info *)&buf[HEAD_LEN];
    struct server_info *server = &(conf.server);
    int ret;

    if(!dev_info.mini_disk)
    {  
        DEBUG("no found ready disk");
        send_error_msg(DISK_NO_FOUND_ERR);
        return ERROR;
    }  

    info->progress = 2;
    info->type = 2;
    char version[32] = {0};
    sprintf(version, "V%d.0.0.%d", conf.major_ver, conf.minor_ver);
    strcpy(info->file_name, version);

    send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);

    umount_boot();
    if(mount_boot() != SUCCESS)
    {  
        DEBUG("mount error");
        send_error_msg(INSTALL_ERR);
        return ERROR;
    }  

usb:
    if(!dev_info.usb_disk)
    {  
        DEBUG("no found usb flash disk");
        sleep(1);
        exec_cmd("mdev -s", result);
        find_all_disks();
        goto usb;
        //send_error_msg(U_DISK_NO_FOUD_ERR);
    }

    sprintf(cmd, install_sh_1, dev_info.mini_disk->name, dev_info.usb_disk->name);
    exec_cmd(cmd, result);
    if(strstr(result, "successd"))
    {
        DEBUG("install programe ok");
        conf.install_flag = 1;
        mount_boot();

        exec_cmd("mkdir -p /boot/conf", result);
        strcpy(config_file, "/boot/conf/config.ini");
        DEBUG("install_flag %d", conf.install_flag);

        info->progress = 100;
        send_pipe(buf, PROGRESS_PIPE ,sizeof(progress_info), PIPE_QT);
        save_config();
        return SUCCESS;
    }
    else
    {
        DEBUG("install programe error %s", result);
        conf.install_flag = 0;
        umount_boot();
        DEBUG("install_flag %d", conf.install_flag);
        send_error_msg(INSTALL_ERR);
        return ERROR;
    }
}
#endif

