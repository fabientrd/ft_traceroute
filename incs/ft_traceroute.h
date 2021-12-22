#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <netinet/ip_icmp.h>
#include <time.h>
#include <fcntl.h>
#include <signal.h>
#include <errno.h>
#include <time.h>
#include <sys/time.h>
# include "libft.h"


typedef struct	s_env{
	int 			h;
	int				ttl;
	int				max;
	int				q;
	double 			z;
	int				n;
	int				queries;
	int				sock;
	int				seq;
	char 			err;
	char 			*dest;
	char			*ip;
	u_int16_t		pid;
	struct addrinfo	hints;
}				t_env;

typedef struct 	s_icmphdr{
	struct icmphdr 	icmp_hdr;
	time_t			timestamp;
	unsigned char 	padding[48];
}			   	t_icmphdr;

typedef struct 	s_rcvmem{
	struct ip 		ip_hdr;
	struct icmphdr 	icmp_hdr;
}				t_rcvmem;

// Functions

int 			resolve_ip(char *addr_host, struct sockaddr_in *addr_con);
unsigned short	checksum(void *b, int len);
void    		traceroute(t_env *env);
int			    init_traceroute(t_env *env, struct sockaddr_in *src);
int				receive(t_env *env, struct timeval tv_seq_start);
void    wait_interval(int interval);

# endif
