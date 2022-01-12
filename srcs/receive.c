#include "../incs/ft_traceroute.h"


int     receive(t_env *env, struct timeval tv_seq_start){
    struct timeval      tv;
    double              spent_time;
    t_rcvmem            *buf;
    char                host[NI_MAXHOST], service[NI_MAXSERV]; 
    struct sockaddr_in  recv;
    socklen_t           recv_len;
    ssize_t             nread;
    int                 s;

    bzero(&recv, sizeof(recv));
    recv_len = sizeof(recv);
    buf = (t_rcvmem*)malloc(sizeof(t_rcvmem));
    ft_bzero((void*)buf, sizeof(t_rcvmem));
    if ((nread = recvfrom(env->sock, buf, sizeof(t_rcvmem), 0, (struct sockaddr*)&recv, &recv_len)) <= 0 || (gettimeofday(&tv, NULL) == -1))
    {
        if (errno == 11){
            free_env(*env);
            env->dest = ft_strsub(OWN_IP, 0, ft_strlen(OWN_IP));
            env->badhost = 1;
            traceroute(env);
            free(buf);
            free_env(*env);
            exit(0);
        }
        free(buf);
        if (env->ttl < 10) printf(" "); 
        if (env->z){
            env->z > 10 ? wait_interval(env->z / 1000) : wait_interval(env->z);
        }
        env->queries == 0 ? printf("%d  *", env->ttl) : printf(" *");
        if (env->queries == env->q - 1)
            printf("\n");
        return (0);
    }    
    s = getnameinfo((struct sockaddr *)&recv, recv_len, host, NI_MAXHOST, service, NI_MAXSERV, NI_NUMERICSERV);
    if (s == 0)
    {
        spent_time = ((double)(tv.tv_usec - tv_seq_start.tv_usec) / 1000000 + (double)(tv.tv_sec - tv_seq_start.tv_sec)) * 1000;
        if (env->ttl < 10) printf(" ");
        if (env->z){
            env->z > 10 ? wait_interval(env->z / 1000) : wait_interval(env->z);
        }
        if (!env->n)    env->queries == 0 ? printf("%d  %s (%s) %.3f ms", env->ttl, host, inet_ntoa(buf->ip_hdr.ip_src), spent_time) : printf(" %.3f ms", spent_time);
        else env->queries == 0 ? printf("%d  %s %.3f ms", env->ttl, inet_ntoa(buf->ip_hdr.ip_src), spent_time) : printf(" %.3f ms", spent_time);
        if (env->badhost)   printf(" !H");
        if (env->queries == env->q - 1)
            printf("\n");
        if (ft_strcmp(inet_ntoa(buf->ip_hdr.ip_src), env->ip) == 0x0)
        {
            free(buf); 
            return (2);
        }
    }
    free(buf);
    return (0);
}