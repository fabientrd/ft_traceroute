#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <arpa/inet.h> // inet_ntoa() ; inet_pton() ; htons() ; ntohs()
# include <netdb.h> // getaddrinfo()
# include <netinet/in.h> // inet_ntoa() ; inet_pton()
# include <unistd.h> // getpid() ; getuid()
# include <stdio.h> // printf()
# include <stdlib.h> // exit()
# include <sys/time.h> // gettimeofday() 
# include <sys/types.h> // getaddrinfo() ; getpid() ; getuid() ; setsockopt() ; recvfrom() ; sendto() ; bind() ; socket()
# include <sys/select.h> // select()
# include <sys/socket.h> // getaddrinfo() ; inet_ntoa() ; inet_pton() ; setsockopt() ; recvfrom() ; to() ; bind() ; socket()
# include <stdio.h>
# include <linux/icmp.h>
# include "libft.h"

# define MAX_HOP 30

typedef struct	s_env{
	int 			h;
	int				ttl;
	int				max;
	int				nqueries;
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

// Functions

int 			resolve_ip(char *addr_host, struct sockaddr_in *addr_con);
unsigned short	checksum(void *b, int len);
void    		traceroute(t_env *env);
int			    init_sock(t_env *env, struct sockaddr_in *src);
int				receive(t_env *env, t_icmphdr *icmp, struct timeval tv_seq_start);

# endif
