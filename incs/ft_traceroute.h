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
# include <sys/socket.h> // getaddrinfo() ; inet_ntoa() ; inet_pton() ; setsockopt() ; recvfrom() ; sendto() ; bind() ; socket()
# include <stdio.h>
# include <linux/icmp.h>
# include "libft.h"

typedef struct	s_env{
	int 		h;
	char 		err;
	char 		*dest;
}				t_env;

typedef struct 	s_icmphdr{
	struct icmphdr 	icmp_hdr;
	time_t			timestamp;
	unsigned char 	padding[48];
}			   	t_icmphdr;

# endif
