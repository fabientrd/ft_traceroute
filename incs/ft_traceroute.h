#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

# include <arpa/inet.h> // inet_ntoa() ; inet_pton() ; htons() ; ntohs()
# include <netdb.h> // getaddrinfo()
# include <netinet/in.h> // inet_ntoa() ; inet_pton()
# include <unistd.h> // getpid() ; getuid()
# include <stdlib.h> // exit()
# include <sys/time.h> // gettimeofday() 
# include <sys/types.h> // getaddrinfo() ; getpid() ; getuid() ; setsockopt() ; recvfrom() ; sendto() ; bind() ; socket()
# include <sys/select.h> // select()
# include <sys/socket.h> // getaddrinfo() ; inet_ntoa() ; inet_pton() ; setsockopt() ; recvfrom() ; sendto() ; bind() ; socket()
# include "libft.h"

typedef struct	s_env{
	int h;
}				t_env;

# endif
