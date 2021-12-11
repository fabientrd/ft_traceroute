#include "../incs/ft_traceroute.h"

void print_bytes(void *ptr, int size) // pour afficher le contenu d'une zone memoire (genre la trame)
{
    unsigned char *p = ptr;
    int i;
    for (i=0; i<size; i++) {
        if (i > 1 && i % 16 == 0)
            printf("\n");
        printf("%02hhX ", p[i]);
    }
    printf("\n");
}

int     receive(t_env *env, t_icmphdr *icmp, struct timeval tv_seq_start){
    (void)env;
    (void)icmp;
    (void)tv_seq_start;
    t_rcvmem           *buf;
    char                host[NI_MAXHOST],
                        service[NI_MAXSERV]; 
    struct sockaddr_in  recv;
    socklen_t           recv_len;
    ssize_t             nread;
    int s;

    bzero(&recv, sizeof(recv));
    recv_len = sizeof(recv);
    buf = (t_rcvmem*)malloc(sizeof(t_rcvmem));
    ft_bzero((void*)buf, sizeof(t_rcvmem));
    if ((nread = recvfrom(env->sock, buf, sizeof(t_rcvmem), 0, (struct sockaddr*)&recv, &recv_len)) <= 0)
    {
        free(buf);
        return (0);
    }
    // print_bytes(&buf, 128);
    s = getnameinfo((struct sockaddr *)&recv, recv_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
    if (s == 0)
    {
        printf("%d %s (%s)\n", env->ttl, host, inet_ntoa(buf->ip_hdr.ip_src));
        if (ft_strcmp(inet_ntoa(buf->ip_hdr.ip_src), env->ip) == 0x0) // si on a atteint l'ip dest on quitte
        {
            free(buf); 
            return (2);
        }
    }
    else
        printf("error a fixer\n");
    free(buf);
    return (0);
}