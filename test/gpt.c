int write_qcow2()
{
    int ret;

    DiskDriver dd;
    DiskDriver *pdd = &dd;
    
    memset(pdd, 0, sizeof(DiskDriver));
    pdd->hd = (void *)linux_new(dst_path);

    read_mbr(pdd);
    read_gpt(pdd);

    QcowEntry* pQe;

    PedDevice *dev = (PedDevice *)pdd->hd;
    //DEBUG("%d", pdd->gpt_parts[pdd->gpt_part_count].end_lba);

    //storeDrv.init();

    storeDrv.setLba(pdd->gpt_parts[pdd->gpt_part_count - 1].end_lba);
    storeDrv.load(dev);
    
    FILE *fp = fopen(src_path, "rb");
    if(!fp)
    {   
        DEBUG("open %s error", src_path);
        return;
    }   
    
    fseek(fp, 0, SEEK_END);
    uint64_t fileSize = ftell(fp);
    DEBUG("fileSize %d", fileSize);

    fseek(fp, 0, SEEK_SET);
    
    uint64_t sizeLba;
    uint64_t realLba;
    sizeLba = fileSize / 512;
    realLba = sizeLba;
    
    storeDrv.alloc("win10-data", sizeLba, realLba, 2, &pQe);
    strcpy(storeDrv.pStoreCfg->currData, "win10-data");

    DEBUG("sizeLba %d startLba %d", sizeLba, pQe->startLba);    
    DEBUG("type %d endLba %d realLba %d", pQe->type, pQe->endLba, pQe->realLba);    
    DEBUG("GetQcowLba(pQe) %d", GetQcowLba(pQe));
    DEBUG("currentBoot name %s", storeDrv.pStoreCfg->currBoot);
    
    _device_seek(dev->fd, GetQcowLba(pQe));

    DEBUG("GetQcowLba(pQe) %d", GetQcowLba(pQe));
    char buf[4096] = {0};

    int count = 0;

#if 0
    for (uint64_t i = 0; i < fileSize + 4095;)
    {   
        //DEBUG("");
        ret = fread(buf, 1, 4096, fp);
        if (ret < 0)
        {   
            DEBUG("fread error");   
        }   
        write_sector(dev->fd, buf, 8); 
        i += 4096;
    
        //if(fileSize < 102400)

    }      
#endif
   DEBUG("write ok");
   
    fseek(fp, 0, SEEK_END);
    fileSize = ftell(fp);
    DEBUG("fileSize %d", fileSize);

    fseek(fp, 0, SEEK_SET);
    
    sizeLba = fileSize / 512;
    realLba = sizeLba;
    
    storeDrv.alloc("win10-share", sizeLba, realLba, 2, &pQe);
    strcpy(storeDrv.pStoreCfg->shareData, "win10-share");

    DEBUG("sizeLba %d startLba %d", sizeLba, pQe->startLba);    
    DEBUG("type %d endLba %d realLba %d", pQe->type, pQe->endLba, pQe->realLba);    
    DEBUG("GetQcowLba(pQe) %d", GetQcowLba(pQe));
    DEBUG("currentBoot name %s", storeDrv.pStoreCfg->currBoot);
    
    _device_seek(dev->fd, GetQcowLba(pQe));

    DEBUG("GetQcowLba(pQe) %d", GetQcowLba(pQe));

#if 0
    for (uint64_t i = 0; i < fileSize + 4095;)
    {   
        //DEBUG("");
        ret = fread(buf, 1, 4096, fp);
        if (ret < 0)
        {   
            DEBUG("fread error");   
        }   
        write_sector(dev->fd, buf, 8); 
        i += 4096;
        //if(fileSize < 102400)
    }      
#endif
	DEBUG("write ok");
    
    storeDrv.save(dev); 
#if 0
    DEBUG("sizeLba %d startLba %d", sizeLba, pQe->startLba);    
    DEBUG("type %d endLba %d realLba %d", pQe->type, pQe->endLba, pQe->realLba);    
    DEBUG("GetQcowLba(pQe) %d", GetQcowLba(pQe));
    DEBUG("currentBoot name %s", storeDrv.pStoreCfg->currBoot);

    
    _device_seek(dev->fd, GetQcowLba(pQe));

    DEBUG("GetQcowLba(pQe) %d", GetQcowLba(pQe));
    char buf[4096] = {0};

    int count = 0;

    for (uint64_t i = 0; i < fileSize + 4095;)
    {   
        //DEBUG("");
        ret = fread(buf, 1, 4096, fp);
        if (ret < 0)
        {   
            DEBUG("fread error");   
        }   
        write_sector(dev->fd, buf, 8); 
        i += 4096;
    
        //if(fileSize < 102400)

    }      
    DEBUG("write ok");
    strcpy(storeDrv.pStoreCfg->currBoot, "win10");
    strcpy(storeDrv.pStoreCfg->currData, "win10-data");
    strcpy(storeDrv.pStoreCfg->currData, "win10-share");
    
#endif
    //storeDrv.save(dev);  
   //uint64_t startLba = storeDrv.getQcowLba(pQe);
#if 0
    storeDrv.init();
    storeDrv.setLba(pdd->gpt_parts[pdd->gpt_part_count - 1].end_lba);
    
    DEBUG(" qcow2 offset %d", pdd->gpt_parts[pdd->gpt_part_count - 1].end_lba);
    FILE *fp = fopen(src_path, "rb");
    if(!fp)
    {   
        DEBUG("open %s error", src_path);
        return;
    }   
    
    fseek(fp, 0, SEEK_END);
    uint64_t fileSize = ftell(fp);
    DEBUG("fileSize %d", fileSize);

    fseek(fp, 0, SEEK_SET);
    
    uint64_t sizeLba;
     uint64_t realLba;
    sizeLba = fileSize / 512;
    realLba = sizeLba;
    
    storeDrv.alloc("win10", sizeLba, realLba, 1, &pQe);
    
    DEBUG("sizeLba %d startLba %d", sizeLba, pQe->startLba);    
    DEBUG("type %d endLba %d realLba %d", pQe->type, pQe->endLba, pQe->realLba);    
    //uint64_t startLba = storeDrv.getQcowLba(pQe);
    
    //PedDevice *dev = (PedDevice *)pdd->hd;

    _device_seek(dev->fd, GetQcowLba(pQe));

    DEBUG("GetQcowLba(pQe) %d", GetQcowLba(pQe));
    char buf[4096] = {0};

    int count = 0;

    for (uint64_t i = 0; i < fileSize + 4095;)
    {   
        //DEBUG("");
        ret = fread(buf, 1, 4096, fp);
        if (ret < 0)
        {   
            DEBUG("fread error");   
        }   
        write_sector(dev->fd, buf, 8); 
        i += 4096;
|    
        //if(fileSize < 102400)

    }      
    DEBUG("write ok");
    
    strcpy(storeDrv.pStoreCfg->currBoot, "win10");
    storeDrv.save(dev); 
    //strcpy_s(storeDrv.pStoreCfg->currBoot, MAX_STORE_NAME_LEN, "win10");
    

    //storeDrv.load()
#endif
#endif

				
}
