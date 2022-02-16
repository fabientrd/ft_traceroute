#include "ft_traceroute.h"
int loop = 1;

void handler()
{
	loop = 0;
	return ;
}

char    *find_local_address(){
    struct ifaddrs *ifap, *ifa;
	struct sockaddr_in *sa;
    char *addr;

	if (getifaddrs(&ifap) != 0){
		perror("getifaddrs()");
		return (NULL);
	}
	for (ifa = ifap; ifa != NULL; ifa = ifa->ifa_next){
	    sa = (struct sockaddr_in *) ifa->ifa_addr;
        addr = inet_ntoa(sa->sin_addr);
		if (!ft_strncmp(addr, "192", 3) == 0){
			continue ;
		}

		break ;
	}
	freeifaddrs(ifap);
    return (addr);
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