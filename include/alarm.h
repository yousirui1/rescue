#ifndef __ALARM_H__
#define __ALARM_H__

static int die_if_timed_out = 0;
static unsigned int alarm_timeout = 0;


# define clear_alarm() ((void)(die_if_timed_out = 0))

#endif
