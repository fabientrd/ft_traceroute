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
    unsigned char       buf[128]; 
    char                host[NI_MAXHOST],
                        service[NI_MAXSERV]; 
    struct sockaddr_in  recv;
    socklen_t           recv_len;
    ssize_t             nread;
    int s;

    bzero(&recv, sizeof(recv));
    recv_len = sizeof(recv);
    ft_memset(buf, 0x0, 128);
    if ((nread = recvfrom(env->sock, buf, sizeof(buf), 0, (struct sockaddr*)&recv, &recv_len)) <= 0)
        return (0);
    // print_bytes(&buf, 128);
    s = getnameinfo((struct sockaddr *)&recv, recv_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
    if (s == 0)
        printf("%zd bytes from %s\n", nread, host);
    else
        printf("error a fixer\n");
    return (0);
}