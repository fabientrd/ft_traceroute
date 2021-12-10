#include "../incs/ft_traceroute.h"

int     init_traceroute(t_env *env, struct sockaddr_in *src)
{
    struct sockaddr_in *res;
    struct addrinfo *addr;
    int result;
    
    result = getaddrinfo(env->dest, NULL, &env->hints, &addr);
    if (result != 0) {
        printf("Error from getaddrinfo: %s\n", gai_strerror(result));
        return 1;
    }
    res = (struct sockaddr_in *)addr->ai_addr;
    (*src) = (*res);
    env->ip = ft_strsub(inet_ntoa(src->sin_addr), 0, ft_strlen(inet_ntoa(src->sin_addr)));
    if ((env->sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP)) < 0)
        return (-1);
    else
        printf("%s is at: %s\n", env->dest,inet_ntoa(src->sin_addr));
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
    struct sockaddr_in  src/*, dest*/;
    struct timeval 	    timeout;
    struct timeval 	    tv_seq_start;
    t_icmphdr 		    icmp;

    if (!(init_traceroute(env, &src)))
        return ;
    timeout.tv_sec = 1;
    timeout.tv_usec = 0;
    printf("ft_traceroute to %s (%s), %d hops max, 60 byte packets\n", env->dest, env->ip, MAX_HOP); // resoudre l'ip du dexieme env->dest
    while (env->ttl <= env->max /* i <= 7 */)
    {  
        if (gettimeofday(&tv_seq_start, NULL) == -1)
            return ;
        setsockopt(env->sock, SOL_IP, IP_TTL, &env->ttl, sizeof(env->ttl));
        setsockopt(env->sock, SOL_SOCKET, SO_RCVTIMEO, &timeout, sizeof(timeout));
        fill_icmp_hdr(env, &icmp, tv_seq_start.tv_sec, env->ttl);
        if (sendto(env->sock, &icmp, sizeof(icmp), 0, (struct sockaddr*)&src, sizeof(src)) <= 0)
            printf("\nPacket Sending Failed!\n");
        receive(env, &icmp, tv_seq_start);
        env->ttl++;
    }
}