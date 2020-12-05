#if 0
static int recv_down_torrent(struct client *cli)
{
    int ret;
    char torrent_file[128] = {0};
    char task_uuid[36 + 1] = {0};
    char uuid[36 + 1] = {0};
    char group_uuid[36 + 1] = {0};
    struct desktop_group *group = NULL;

    yzy_torrent *torrent = (yzy_torrent *)&cli->recv_buf[read_packet_supplementary(cli->recv_head) +
                                read_packet_token(cli->recv_head)];
    
    memcpy(uuid, torrent->uuid, 36);
    memcpy(task_uuid, torrent->task_uuid, 36);
    memcpy(group_uuid, torrent->group_uuid, 36);

    char *torrent_data = &cli->recv_buf[read_packet_supplementary(cli->recv_head) + 
                                        read_packet_token(cli->recv_head) + sizeof(yzy_torrent)];
    sprintf(torrent_file, "/root/voi_%d_%s.torrent", torrent->dif_level, uuid);
    FILE *fp = fopen(torrent_file, "wb");
    if(fp)
    {
        ret = fwrite(torrent_data, 1, torrent->data_len, fp);
        fflush(fp);
        fclose(fp);

        if(ret == torrent->data_len)
        {
            group = get_desktop_group(group_uuid);  
            
            if(group->task.bt)
            {
                int offset = group->task.offset;
                memcpy(group->task.bt[offset].uuid, torrent->uuid, 36);
                memcpy(group->task.bt[offset].torrent_file, torrent_file, strlen(torrent_file));

                group->task.total_size += torrent->file_size;
                
                group->task.bt[offset].diff = torrent->dif_level;
                group->task.bt[offset].disk_type = torrent->type;
                group->task.bt[offset].real_size = torrent->real_size;
                group->task.bt[offset].space_size = torrent->space_size;
                group->task.bt[offset].file_size = torrent->file_size;
                group->task.bt[offset].operate_id = torrent->operate_id;

                if(++group->task.offset == group->task.size)
                {
                    en_queue(&task_queue, (char *)&group->task, sizeof(group->task), TASK_BT);
                }
            }
            ret = send_down_torrent(cli, task_uuid);  
        }
    }
    return ret;
}
#endif

