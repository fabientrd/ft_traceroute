#include "ft_traceroute.h"
int loop = 1;

void handler()
{
	loop = 0;
	return ;
}

void    wait_interval(int interval) // sleep() alternative
{
	struct timeval tv_current;
	struct timeval tv_next;

    signal(SIGINT, handler);
	if (gettimeofday(&tv_current, NULL) < 0)
		return ; // gerer erreur exit(42)
	tv_next = tv_current;
	tv_next.tv_sec += interval;
	while ((tv_current.tv_sec < tv_next.tv_sec || tv_current.tv_usec < tv_next.tv_usec) && loop)
	{
		if (gettimeofday(&tv_current, NULL) < 0)
			return ; // gerer erreur exit(42)
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

// int resolve_ip(char *addr_host, struct sockaddr_in *addr_con) // IP string vers IP decimale (utilisable), et hostname vers IP decimale
// {
// 	struct hostent 	*host_entity;
	
// 	if ((host_entity = gethostbyname(addr_host)) == NULL) // pas le droit, mais en attendant...
// 		return (-1);
//     printf("LES VALEURS DE RESOLVE IP AVEC GETHOSTBYNAME (interdite)\n\n");
//     printf("addr_host = %s\n", addr_host);
//     printf("addr_con.siin family = %d\n", host_entity->h_addrtype);
//     printf("addr_con.sin port = %d\n", htons(0));
//     printf("addr_con.siin family = %ld\n\n", *(long*)host_entity->h_addr);
// 	(*addr_con).sin_family = host_entity->h_addrtype;
// 	(*addr_con).sin_port = htons(0);
// 	(*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;
// 	return (1);
// }