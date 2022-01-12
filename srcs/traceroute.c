#include "../incs/ft_traceroute.h"

int     init_traceroute(t_env *env, struct sockaddr_in *src)
{
    struct sockaddr_in *res;
    struct addrinfo *addr;
    int result;
    
    addr = NULL;
    res = NULL;
    result = getaddrinfo(env->dest, NULL, &env->hints, &addr);
    if (result != 0) {
        printf("Error from getaddrinfo: %s\n", error_addrinfo(result)); // FAIRE UNE FONCTION QUI CHECK LES RETOURS D'ERREURS
        freeaddrinfo(addr);
        return (-1);
    }
    res = (struct sockaddr_in *)addr->ai_addr;
    (*src) = (*res);
    env->ip = ft_strsub(inet_ntoa(src->sin_addr), 0, ft_strlen(inet_ntoa(src->sin_addr)));
    if ((env->sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0){
        freeaddrinfo(addr);
        return (-1);
    }   
    freeaddrinfo(addr);
    return (1);
}

void fill_icmp_hdr(t_env *env, t_icmphdr *icmp, time_t timestamp, int seq)
{
    bzero(icmp, sizeof(t_icmphdr));
	icmp->icmp_hdr.type = 8;
	icmp->icmp_hdr.un.echo.id = ((env->pid << 8) & 0xff00) | ((env->pid >> 8) & 0x00ff);
	icmp->icmp_hdr.un.echo.sequence = seq;
    icmp->icmp_hdr.un.echo.sequence = ((icmp->icmp_hdr.un.echo.sequence << 8) & 0xff00) | ((icmp->icmp_hdr.un.echo.sequence >> 8) & 0x00ff);
	icmp->timestamp = timestamp;
    icmp->icmp_hdr.checksum = checksum(icmp, sizeof(t_icmphdr));
}

void    traceroute(t_env *env)
{
    struct sockaddr_in  src;
    struct timeval 	    timeout;
    struct timeval 	    tv_seq_start;
    t_icmphdr 		    icmp;
    int                 ret;

    if (init_traceroute(env, &src) < 0)
        return ;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    if (!(env->badhost))  printf("ft_traceroute to %s (%s), %d hops max, 60 byte packets\n", env->dest, env->ip, env->max);
    while (env->ttl <= env->max)
    {  
        if (gettimeofday(&tv_seq_start, NULL) == -1)
            return ;
        setsockopt(env->sock, SOL_IP, IP_TTL, &env->ttl, sizeof(env->ttl));
        setsockopt(env->sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        fill_icmp_hdr(env, &icmp, tv_seq_start.tv_sec, env->ttl);
        if (sendto(env->sock, &icmp, sizeof(icmp), 0, (struct sockaddr*)&src, sizeof(src)) <= 0){
            printf("connect: Permission denied\n");
            break ;
        }
        ret = receive(env, tv_seq_start);
        env->queries++;
        if (env->queries != env->q)
            continue ;
        if (ret == 2)
            break;
        env->ttl++;
        env->queries = 0;
    }
}