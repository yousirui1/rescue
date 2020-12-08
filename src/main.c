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
    DIE("\nWhile playing:\n"
           "q, ESC              quit\n"
           "f                   toggle full screen\n"
           "p, SPC              pause\n"
           "m                   toggle mute\n"
           "9, 0                decrease and increase volume respectively\n"
           "/, *                decrease and increase volume respectively\n"
           "a                   cycle audio channel in the current program\n"
           "v                   cycle video channel\n"
           "t                   cycle subtitle channel in the current program\n"
           "c                   cycle program\n"
           "w                   cycle video filters or show modes\n"
           "s                   activate frame-step mode\n"
           "left/right          seek backward/forward 10 seconds or to custom interval if -seek_interval is set\n"
           "down/up             seek backward/forward 1 minute\n"
           "page down/page up   seek backward/forward 10 minutes\n"
           "right mouse click   seek to percentage in file corresponding to fraction of width\n"
           "left double-click   toggle full screen\n"
           );
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

    p2v_transform("192.169.27.181", "root", "123qwe,.",
                    "template-voi", "test",
                    dev_info.mini_disk->dev->path);


#if 0
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
#endif

	do_exit();
	close_pipe();
	close_logs();
	return ret;
}
