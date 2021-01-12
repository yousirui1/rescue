#include "base.h"

static int die_if_timed_out = 0;
static unsigned int alarm_timeout = 0;


void alarm_handler(int sig)
{
	if(die_if_timed_out)
		DEBUG("");	
}

void set_alarm(void)
{
	if(alarm_timeout)
	{
		alarm(alarm_timeout);
		die_if_timed_out = 1;
	}
}

