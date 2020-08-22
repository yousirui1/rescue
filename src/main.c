#include "base.h"

/* pipe */
int pipe_event[2];
int pipe_tcp[2];
int pipe_udp[2];
int pipe_qt[2];

struct config conf;
time_t current_time;
const char program_name[] = "rescue";
char config_file[128] = "config.ini";

pthread_t pthread_qt, pthread_event, pthread_task;

static void do_exit()
{
    void *tret = NULL;

    pthread_join(pthread_qt, &tret);
    DEBUG("pthread_exit %d qt", (int *)tret);

    pthread_join(pthread_event, &tret);
    DEBUG("pthread_exit %d event", (int *)tret);
}

static void sig_quit_listen(int e)
{
    char s = 'S';
    write(pipe_qt[1], &s, sizeof(s));
    DEBUG("kill use signal end !");

    write(pipe_event[1], &s, sizeof(s));
    DEBUG("kill use signal end !");
}

int init_pipe()
{
    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_qt) < 0)
    {   
        DEBUG("create client qt err %s", strerror(errno));
        return ERROR;
    }   

    fcntl(pipe_qt[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_qt[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_tcp) < 0)
    {   
        DEBUG("create server pipe err %s", strerror(errno));
        return ERROR;
    }   

    fcntl(pipe_tcp[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_tcp[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_udp) < 0)
    {   
        DEBUG("create client pipe err %s", strerror(errno));
        return ERROR;
    }   

    fcntl(pipe_udp[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_udp[1], F_SETFL, O_NONBLOCK);

    /* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_event) < 0)
    {   
        DEBUG("create client pipe err %s", strerror(errno));
        return ERROR;
    }   

    fcntl(pipe_event[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_event[1], F_SETFL, O_NONBLOCK);
    return SUCCESS;
}

void close_pipe()
{
    close_fd(pipe_event[0]); 
    close_fd(pipe_event[1]);
    close_fd(pipe_tcp[0]);
    close_fd(pipe_tcp[1]);
    close_fd(pipe_udp[0]);
    close_fd(pipe_udp[1]);
    close_fd(pipe_qt[0]);
    close_fd(pipe_qt[1]);
}

int main(int argc, char *argv[])
{
	tftp_get("192.169.27.251", "voi.zip", "/root/voi.zip");
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
	init_device();
	init_config();

	ret = pthread_create(&pthread_event, NULL, thread_event, NULL);
	if(0 != ret)
	{
		DIE("create  tcp thread ret: %d error: %s", ret, strerror(ret));
	}

	ret = pthread_create(&pthread_qt, NULL, thread_qt, NULL);
	if(0 != ret)
	{
		DIE("create qt thread ret: %d error: %s", ret, strerror(ret));
	}

	ret = pthread_create(&pthread_task, NULL, thread_task, NULL);
	if(0 != ret)
	{
		DIE("create qt thread ret: %d error: %s", ret, strerror(ret));
	}	

	client_connect();
	do_exit();
	close_pipe();
	close_logs();
	return 0;
#endif
}

