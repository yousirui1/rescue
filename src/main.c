#include "base.h"

time_t current_time;
const char program_name[] = "rescue";

static pthread_t pthread_event, pthread_task, pthread_ui;

static void do_exit()
{
	void *tret = NULL;
	
	pthread_join(pthread_task, &tret);
	DEBUG("pthread_exit %d task", (int *)tret);

	pthread_join(pthread_ui, &tret);
	DEBUG("pthread_exit %d ui", (int *)tret);

	pthread_join(pthread_event, &tret);
	DEBUG("pthread_exit %d event", (int *)tret);
}

static void sig_quit_listen(int e)
{
	char s = 'S';
    write(pipe_ui[1], &s, sizeof(s));
    write(pipe_task[1], &s, sizeof(s));
    write(pipe_event[1], &s, sizeof(s));
	
    DEBUG("kill use signal end !");
}

static void usage()
{
    DIE("\nYZY RESCUE LINUX:\n"
           "t,               	set tftp server ip\n"
           "s                   set YZY server ip\n"
           "n,              	set nfs server rootfs \n"
           "p,              	set http server ip \n"
           "help                show this helo\n"
           );
}

void parse_options(int argc, char *argv[])
{
	int ch;
	while((ch = getopt(argc, argv, "t:s:n:p:h")) != -1)
	{
		switch(ch)
		{
			case 't':
				if(strlen(optarg) > 8)
				{
					DEBUG("tftp ip:%s", optarg);
					strcpy(conf.tftp_ip, optarg);
				}
				break;
			case 's':
				if(strlen(optarg) > 8)
				{
					DEBUG("server ip:%s", optarg);
					strcpy(conf.server.ip, optarg);
				}
				break;
			case 'n':
				if(strlen(optarg) > 8)
				{
					DEBUG("nfs ip:%s", optarg);
					strcpy(conf.nfs_ip, optarg);
				}
				break;
			case 'p':					//httpd
				if(strlen(optarg) > 8)
				{
					DEBUG("http ip:%s", optarg);
					strcpy(conf.http_ip, optarg);
				}
				break;
			case 'h':
				usage();	
				break;
			default:
				break;
		}
	}
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
	init_configs();
	bt_client();

	ret = pthread_create(&pthread_event, NULL, thread_event, NULL);
	if(SUCCESS != ret)
	{
		DIE("create event thread ret: %d error: %s", ret, strerror(ret));
	}

	ret = pthread_create(&pthread_ui, NULL, thread_ui, NULL);
	if(SUCCESS != ret)
	{
		DIE("create ui thread ret: %d error: %s", ret, strerror(ret));
	}

	ret = pthread_create(&pthread_task, NULL, thread_task, NULL);
	if(SUCCESS != ret)
	{
		DIE("create task thread ret: %d error: %s", ret, strerror(ret));
	}

	do_exit();
	close_pipe();
	close_logs();
	return ret;
}
