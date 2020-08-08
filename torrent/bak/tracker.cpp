#include "tracker.h"


#include <stdlib.h>
#include <string.h>
#include <errno.h>

#include "peerlist.h"
#include "httpencode.h"

btTracker Tracker;


btTracker::btTracker()
{
	memset(m_host, 0, MAXHOSTNAMELEN);
	memset(m_path, 0, MAXPATHLEN);
	
	m_sock = INVALID_SOCKET;
	m_port = 80;
	m_status = T_FREE;
	m_f_started = m_f_stoped = m_f_pause = 0;
	m_interval = 15;
	
	m_connect_refuse_click = 0;
	m_last_timestamp = (time_t) 0;	
}

btTracker::~btTracker()
{
	if(m_sock != INVALID_SOCKET)
		CLOSE_SOCKET(m_sock);
}

void btTracker::Reset(time_t new_interval)
{


}

int btTracker::_IPsin(char *h, int p, struct sockaddr_in *psin)
{

}

int btTracker::_s2sin(char *h, int p, struct sockaddr_in *psin)
{

}

int btTracker::CheckReponse()
{
#define MAX_LEN_SIZ 32
	char *pdata;
	ssize_t r;
	size_t q, hlen, dlen;

}

int btTracker::Initial()
{
	char ih_buf[20 * 3 + 1], pi_buf[20 * 3 + 1] ,tmppath[MAXPATHLEN];
	

}


