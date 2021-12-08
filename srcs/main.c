#include "../incs/ft_traceroute.h"

/*
    To-Do:

    -Condition d'arret a la loop traceroute au cas ou on a atteint la machine cible:
        Peut-etre faire un resolve ip au prealable et stop la loop une fois que l'ip dans le time-exceeded 
        correspond a l'ip de la machine cible ?
*/

void    free_env(t_env env){
    if (env.dest) free(env.dest);
}

void    usage(int i, char c){
    if (!i && c != '\0')
        printf("ft_traceroute: invalid option -- '%c'\n\n", c);
    printf("Usage:\n  ft_traceroute [ flags ] host\nOptions:\n  -h                          print help and exit\n"
    "\nArguments:\n+     host          The host to traceroute to\n");
}

void    manage_env(t_env *env, char **av)
{
    for (int i = 1; av[i] != NULL; i++){
        if (av[i][0] == '-'){
            for (int y = 1; av[i][y] != '\0'; y++){
                if (av[i][y] == 'h') env->h = 1;
                else env->err = av[i][y];
            }
        }
        else 
            if (env->dest == NULL) env->dest = ft_strdup(av[i]);
    }
}

int     init_sock(t_env *env)
{
    int sock;

    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
        return (-1);
    else
        env->sock = sock;
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

void    traceroute(t_env *env, struct sockaddr_in *addr_con)
{
    int             i;
    struct timeval 	tv_seq_start;
    t_icmphdr 		icmp;

    i = env->ttl;
    while (i <= 3) // pour tester. Sinon i <= MAX_HOP (30)
    {
        if (gettimeofday(&tv_seq_start, NULL) == -1)
            return ; // gerer erreur normalement
        setsockopt(env->sock, SOL_IP, IP_TTL, &i, sizeof(i));
        fill_icmp_hdr(env, &icmp, tv_seq_start.tv_sec, i);
        if (sendto(env->sock, &icmp, sizeof(icmp), 0, (struct sockaddr*)addr_con, sizeof(*addr_con)) <= 0)
            printf("\nPacket Sending Failed!\n");
        i++;
    }
}

int     main(int ac, char **av)
{
    t_env               env;
    struct sockaddr_in  addr_con;

    init_env(&env);
    manage_env(&env, av);
	if (getuid() != 0 || env.h || env.err || ac == 1){
	    getuid() != 0 ? printf("This program sends raw socket, you must be root or sudoers to use it.\n") : usage(env.h, env.err);
        free_env(env);
		return env.h ? EXIT_SUCCESS : EXIT_FAILURE;
	}
    printf("env->dest = %s\n", env.dest);
    if (!resolve_ip(env.dest, &addr_con))
        return (-1);
    if (!init_sock(&env))
        return (-1);
    traceroute(&env, &addr_con);
    free_env(env);
    return (0);
}