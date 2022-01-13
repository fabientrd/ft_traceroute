#include "ft_traceroute.h"
int loop = 1;

void handler()
{
	loop = 0;
	return ;
}

void    wait_interval(int interval){
	struct timeval tv_current;
	struct timeval tv_next;

    signal(SIGINT, handler);
	if (gettimeofday(&tv_current, NULL) < 0)
		return ;
	tv_next = tv_current;
	tv_next.tv_sec += interval;
	while ((tv_current.tv_sec < tv_next.tv_sec || tv_current.tv_usec < tv_next.tv_usec) && loop)
	{
		if (gettimeofday(&tv_current, NULL) < 0)
			return ;
	}
}

unsigned short checksum(void *b, int len)
{    
    unsigned short  *buf;
    unsigned int    sum;
    unsigned short  result;
  
    buf = b;
    sum = 0;
    result = 0;
    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}