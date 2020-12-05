#include <linux/netlink.h>
#include <arpa/inet.h>
#include <linux/rtnetlink.h>
#include <sys/reboot.h>

#include "base.h"
#include "socket.h"
#include "client.h"
#include "device.h"

#define IFF_LOWER_UP    0x10000

extern struct device_info dev_info;

static pthread_t pthread_client;
static time_t last_time;

static void process_event_msg(char *buf, int length)
{
	switch(read_msg_order(buf))
	{
		case CLIENT_CONNECT_PIPE:
		{
			void *tret = NULL;
			pthread_cancel(pthread_client);
			pthread_join(pthread_client, &tret);
			DEBUG("pthread_exit client ret:%d", (int *)tret);
			pthread_create(&pthread_client, NULL, thread_client, NULL);
			break;
		}
		case PROGRESS_PIPE:
		{
			send_progress(&m_client, &buf[HEAD_LEN]);
			send_pipe(buf, PROGRESS_PIPE, length, PIPE_UI);
			break;
		}
		case REBOOT_PIPE:
		{
			DEBUG("server send msg reboot");
			stop_torrent();
			send_upload_log(&m_client);
			sync();
			reboot(RB_AUTOBOOT);				
			break;
		}
		case SHUTDOWN_PIPE:
		{
			DEBUG("server send msg shutdown");
			stop_torrent();
			send_upload_log(&m_client);
			sync();
			reboot(RB_POWER_OFF);				
			break;
		}
	}	
}

static void process_ui_msg(char *buf, int length)
{
	int ret;
	
	switch(read_msg_order(buf))
	{
		case INSTALL_PIPE:
		{
			install_programe();
			break;
		}
		case INIT_PIPE:
		{
			init_qcow2(dev_info.mini_disk->dev, 0);
			client_reconnect();
			remove("/boot/conf/config.ini");
			sync();
			reboot(RB_AUTOBOOT);				
		}
		case UPDATE_CONFIG_PIPE:
		{
			update_config(buf, length);		
			client_reconnect();
			break;
		}
		case P2V_OS_PIPE:
		{
			send_p2v_transform(&m_client, &buf[HEAD_LEN]);
			break;
		}
		case EXIT_PROGRESS_PIPE:
		{
			char s = 'S';
    		write(pipe_event[1], &s, sizeof(s));
			break;
		}
		case REBOOT_PIPE:
		{
			DEBUG("qt send pipe reboot msg");
			stop_torrent();
			send_upload_log(&m_client);
			client_reconnect();
			sync();
			reboot(RB_AUTOBOOT);				
		}
		default:
			break;
	}
}

void event_loop()
{
	int netfd, maxfd = -1;
	int i, nready, ret;
    char buf[DATA_SIZE] = {0};

	struct timeval tv;
	tv.tv_sec = 1;
	tv.tv_usec = 0;	
	
	fd_set reset, allset;
	FD_ZERO(&allset);

	netfd = get_netcard_state();
	
	FD_SET(pipe_tcp[1], &allset);
	FD_SET(pipe_task[1], &allset);
	FD_SET(pipe_ui[1], &allset);
	FD_SET(pipe_event[0], &allset);
	FD_SET(netfd, &allset);	

	maxfd = maxfd > pipe_tcp[1] ? maxfd : pipe_tcp[1];
	maxfd = maxfd > pipe_task[1] ? maxfd : pipe_task[1];
	maxfd = maxfd > pipe_ui[1] ? maxfd : pipe_ui[1];
	maxfd = maxfd > pipe_event[0] ? maxfd : pipe_event[0];
	maxfd = maxfd > netfd ? maxfd : netfd;

	last_time = current_time;
    pthread_create(&pthread_client, NULL, thread_client, NULL);

    for(;;)
    {
        reset = allset;
        tv.tv_sec = 1;
        tv.tv_usec = 0;

        ret = select(maxfd + 1, &reset, NULL, NULL, &tv);
        if(ret == -1)
        {
            if(errno == EINTR)
                continue;
            else if(errno != EBADF)
            {
                FAIL("select %s error", strerror(ret));
                continue;
            }
        }
		nready = ret;
        (void)time(&current_time);

		if(current_time - last_time >= TIME_OUT)
		{
			ret = send_heartbeat(&m_client);
			if(ret != SUCCESS || m_client.online <= 0)
			{
				client_reconnect();					
			}
			m_client.online --;		//3个包
			last_time = current_time;
		}
	
		if(FD_ISSET(pipe_event[0], &reset))
		{
    		ret = read(pipe_event[0], (void *)buf, sizeof(buf));
            if(ret >= HEAD_LEN)
            {   
                process_event_msg(buf, ret);
            }   
            else if(ret == 1)
            {   
                if(buf[0] == 'S')
                {   
					void *tret = NULL;
					pthread_cancel(pthread_client);
					pthread_join(pthread_client, &tret);
					
					write(pipe_ui[1], buf, 1); 
                	write(pipe_task[1], buf, 1);
                    DEBUG("event thread pipe msg exit");
                    break;
                }    
            }   
            if(--nready <= 0)
                continue;
		}	
		if(FD_ISSET(pipe_ui[1], &reset))
		{
			ret = read(pipe_ui[1], (void *)buf, sizeof(buf));
			if(ret >= HEAD_LEN)
			{
				process_ui_msg(buf, ret);
			}
			if(--nready <= 0)
				continue;			
		}
		if(FD_ISSET(netfd, &reset))
		{
			int len = 1024;
		    struct nlmsghdr *nh;
    		struct ifinfomsg *ifinfo;
    		struct rtattr *attr;

			ret = read(netfd, (void *)buf, sizeof(buf));
			for(nh = (struct nlmsghdr *)buf; NLMSG_OK(nh, ret); nh = NLMSG_NEXT(nh,ret))
			{
				if(nh->nlmsg_type == NLMSG_DONE) 
					break;
				else if(nh->nlmsg_type == NLMSG_ERROR)
					break;
				else if(nh->nlmsg_type != RTM_NEWLINK)
					continue;
				ifinfo = NLMSG_DATA(nh);

				DEBUG("%u: %s", ifinfo->ifi_index,
						(ifinfo->ifi_flags & IFF_LOWER_UP) ? "up" : "down" );
				if(ifinfo->ifi_flags & IFF_LOWER_UP)		//UP
				{
					client_reconnect();	
				}
				else										//down
				{
					send_pipe(buf, CLIENT_DOWN_PIPE, 0, PIPE_UI);
				}
				attr = (struct rtattr*)(((char*)nh) + NLMSG_SPACE(sizeof(*ifinfo)));
				for(; RTA_OK(attr, len); attr = RTA_NEXT(attr, len))
				{
					if(attr->rta_type == IFLA_IFNAME)
					{
						DEBUG("%s", (char *)RTA_DATA(attr));
						break;
					}
				}
			}
            if(--nready <= 0)
                continue;
		}
	}
	close_fd(netfd);
}

void *thread_event(void *param)
{
    int ret = SUCCESS;
    pthread_attr_t st_attr;
    struct sched_param sched;
    
    ret = pthread_attr_init(&st_attr);
    if(ret)
    {    
        DEBUG("thread event attr init warning ");
    }    
    ret = pthread_attr_setschedpolicy(&st_attr, SCHED_FIFO);
    if(ret)
    {    
        DEBUG("thread event set SCHED_FIFO warning");
    }    
    sched.sched_priority = SCHED_PRIORITY_EVENT;
    ret = pthread_attr_setschedparam(&st_attr, &sched);

    event_loop();
    return (void *)ret;
}
