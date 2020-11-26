#include "base.h"

/* pipe */
int pipe_event[2];
int pipe_tcp[2];
int pipe_udp[2];
int pipe_qt[2];
int pipe_bt[2];

struct config conf = {0};
time_t current_time;
const char program_name[] = "rescue";
char config_file[128] = "config.ini";

pthread_t pthread_qt, pthread_event, pthread_task, pthread_bt;
int online = 0;

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

void parse_options(int argc, char *argv[])
{
	int ch;
	while((ch = getopt(argc, argv, "t:h")) != -1)
	{
		switch(ch)
		{
			case 't':
				DEBUG("tftp server:%s", optarg);
				strcpy(conf.tftp_ip, optarg);
				break;
			case 'h':
				DEBUG("help");
				break;
			default:
				break;
		}
	}
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

	/* create pipe to give main thread infomation */
    if(socketpair(AF_UNIX, SOCK_SEQPACKET, 0, pipe_bt) < 0)
    {   
        DEBUG("create client pipe err %s", strerror(errno));
        return ERROR;
    }   

    fcntl(pipe_bt[0], F_SETFL, O_NONBLOCK);
    fcntl(pipe_bt[1], F_SETFL, O_NONBLOCK);

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
	close_fd(pipe_bt[0]);
	close_fd(pipe_bt[1]);
}

int main(int argc, char *argv[])
{
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

	parse_options(argc, argv);

	init_pipe();
	init_device();
	init_config();

	bt_client();

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
	
#if 0
	ret = pthread_create(&pthread_bt, NULL, thread_bt, NULL);
	if(0 != ret)
	{
		DIE("create bt thread ret: %d error: %s", ret, strerror(ret));
	}
#endif

	client_connect();
	do_exit();
	close_pipe();
	close_logs();
	return 0;
}
