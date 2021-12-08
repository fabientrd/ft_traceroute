#include "ft_traceroute.h"

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

void    init_env(t_env *env){
    env->h = 0;
    env->err = '\0';
    env->dest = NULL;
    env->seq = 1;
    env->pid = getpid();
    env->ttl = 1;
}

int resolve_ip(char *addr_host, struct sockaddr_in *addr_con) // IP string vers IP decimale (utilisable), et hostname vers IP decimale
{
	struct hostent 	*host_entity;
	
	if ((host_entity = gethostbyname(addr_host)) == NULL) // pas le droit, mais en attendant...
		return (-1);
	(*addr_con).sin_family = host_entity->h_addrtype;
	(*addr_con).sin_port = htons(0);
	(*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;
	return (1);
}