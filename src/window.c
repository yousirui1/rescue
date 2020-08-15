#include "base.h"

void *thread_qt(void *param)
{
    int ret;
    pthread_attr_t st_attr;
    struct sched_param sched;

    ret = pthread_attr_init(&st_attr);
    if(ret)
    {   
        DEBUG("Thread FFmpeg Decode attr init error ");
    }   
    ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
    if(ret)
    {   
        DEBUG("Thread FFmpeg Decode set SCHED_FIFO error");
    }   
    sched.sched_priority = SCHED_PRIORITY_EVENT;
    ret = pthread_attr_setschedparam(&st_attr, &sched);

   	set_pipe(pipe_qt);
	set_config(&conf);
	DEBUG("%s", conf.netcard.ip);
    create_window(0);
    return (void *)0;
}



