#include "../incs/ft_traceroute.h"

/*
    To-Do:

    -Condition d'arret a la loop traceroute au cas ou on a atteint la machine cible:
        Peut-etre faire un resolve ip au prealable et stop la loop une fois que l'ip dans le time-exceeded 
        correspond a l'ip de la machine cible ?
*/

void    usage(int i, char c){
    if (!i && c != '\0')
        printf("ft_traceroute: invalid option -- '%c'\n\n", c);
    printf("Usage:\n  ft_traceroute [ flags ] host\nOptions:\n  -h                          print help and exit\n"
    "\nArguments:\n+     host          The host to traceroute to\n");
}

void    init_env(t_env *env){
    env->h = 0;
    env->err = '\0';
    env->dest = NULL;
    env->sock = 0;
    env->seq = 1;
    env->pid = getpid();
    env->ttl = 1;
    env->max = 30;
    env->nqueries = 3;
    memset(&env->hints, 0, sizeof(struct addrinfo));
    env->hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    env->hints.ai_socktype = SOCK_RAW; /* Datagram socket */
    env->hints.ai_flags = IPPROTO_ICMP;    /* For wildcard IP address */
    env->hints.ai_protocol = 0;          /* Any protocol */
    env->hints.ai_canonname = NULL;
    env->hints.ai_addr = NULL;
    env->hints.ai_next = NULL;
}

void    manage_env(t_env *env, char **av, int ac)
{
    for (int i = 1; av[i] != NULL; i++){
        if (av[i][0] == '-'){
            for (int y = 1; av[i][y] != '\0'; y++){
                switch (av[i][y])
                {
                    case 'h':
                        env->h = 1;
                        break;
                    case 'm':
                        env->max = ft_atoi(av[i+1]);
                        break;
                    case 'f':
                        env->ttl = ft_atoi(av[i+1]);
                        break;
                    default: env->err = av[i][y];
                }
            }
        }
        else 
            if (env->dest == NULL) env->dest = ft_strdup(av[ac-1]);
    }
}

void    free_env(t_env env){
    if (env.dest) free(env.dest);
}

int     main(int ac, char **av)
{
    t_env               env;

    init_env(&env);
    manage_env(&env, av, ac);
	if (getuid() != 0 || env.h || env.err || ac == 1){
	    getuid() != 0 ? printf("This program sends raw socket, you must be root or sudoers to use it.\n") : usage(env.h, env.err);
        free_env(env);
		return env.h ? EXIT_SUCCESS : EXIT_FAILURE;
	}
    traceroute(&env);
    free_env(env);
    return (0);
}