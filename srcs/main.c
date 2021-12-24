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
    printf("Usage:\n  ft_traceroute [ flags ] host\nOptions:\n  -f first_ttl"
    "                Start from the first_ttl hop (instead from 1)\n  -m max_ttl                  " 
    "The max no. of hops (max time-to-live value) traceroute will probe. Default=30 hops\n"
    "  -n                          Do not resolve IP addresses to their domain names\n"
    "  -q nqueries                 Set the number of probes per each hop. Default is 3\n  -h"
    "                          print help and exit\n"
    "\nArguments:\n+     host          The host to traceroute to\n");
}

void    init_env(t_env *env){
    env->h = 0;
    env->err = '\0';
    env->dest = NULL;
    env->ip = NULL;
    env->sock = 0;
    env->seq = 1;
    env->pid = getpid();
    env->ttl = 1;
    env->max = 30;
    env->q = 3;
    env->z = 0;
    env->n = 0;
    env->queries = 0;
    memset(&env->hints, 0, sizeof(struct addrinfo));
    env->hints.ai_family = AF_INET;    /* Allow IPv4 or IPv6 */
    env->hints.ai_socktype = SOCK_RAW; /* Datagram socket */
    env->hints.ai_flags = IPPROTO_ICMP;    /* For wildcard IP address */
    env->hints.ai_protocol = 0;          /* Any protocol */
    env->hints.ai_canonname = NULL;
    env->hints.ai_addr = NULL;
    env->hints.ai_next = NULL;
}

int    manage_env(t_env *env, char **av, int ac)
{
    for (int i = 1; av[i] != NULL; i++){
        if (av[i][0] == '-'){
            for (int y = 1; av[i][y] != '\0'; y++){
                switch (av[i][y])
                {
                    case 'h':
                        env->h = 1;
                        break ;
                    case 'm':
                        env->max = ft_atoi(av[i + 1]);
                        if (env->max == 0 && !ft_strisdigit(av[i + 1])){
                            printf("Cannot handle `-m' option with arg `%s' (argc %d)\n", av[i + 1], i + 1);
                            return (-1);
                        }
                        else if (env->max <= 0 || env->max > 255)
                        {
                            env->max == 0 ? printf("first hop out of range\n") : printf("max hops cannot be more than 255\n");
                            return (-1);
                        }
                        break ;
                    case 'f':
                        env->ttl = ft_atoi(av[i + 1]);
                        if (env->ttl == 0 && !ft_strisdigit(av[i + 1])){
                            printf("Cannot handle `-f' option with arg `%s' (argc %d)\n", av[i + 1], i + 1);
                            return (-1);
                        }
                        if (env->ttl <= 0)
                        {
                            printf("first hop out of range\n");
                            return (-1);
                        }
                        break ;
                    case 'q':
                        env->q = ft_atoi(av[i + 1]);
                        if (env->q == 0 && !ft_strisdigit(av[i + 1])){
                            printf("Cannot handle `-q' option with arg `%s' (argc %d)\n", av[i + 1], i + 1);
                            return (-1);
                        }
                        else if (env->q <= 0 || env->q > 10)
                        {
                            printf("no more than 10 probes per hop\n");
                            return (-1);
                        }
                        break ;
                    case 'z':
                        env->z = ft_atoi(av[i + 1]);
                        if (env->z == 0 && !ft_strisdigit(av[i + 1])){
                            printf("Cannot handle `-z' option with arg `%s' (argc %d)\n", av[i + 1], i + 1);
                            return (-1);
                        }
                        else if (env->z < 0){
                            printf("bad sendtime `%s' specified\n", av[i + 1]);
                            return (-1);
                        }
                        break ;
                    case 'n':
                        env->n = 1;
                        break ;
                    default: env->err = av[i][y];
                }
            }
        }
        else 
            if (env->dest == NULL) env->dest = ft_strdup(av[ac-1]);
    }
    return (1);
}

void    free_env(t_env env){
    if (env.dest != NULL) 
        free(env.dest);
    if (env.ip != NULL) 
        free(env.ip); 
}

int     main(int ac, char **av)
{
    t_env               env;

    init_env(&env);
    if (manage_env(&env, av, ac) == -1){
        return (-1);
    }
	if (getuid() != 0 || env.h || env.err || ac == 1){
	    getuid() != 0 ? printf("This program sends raw socket, you must be root or sudoers to use it.\n") : usage(env.h, env.err);
        free_env(env);
		return env.h ? EXIT_SUCCESS : EXIT_FAILURE;
	}
    traceroute(&env);
    free_env(env);
    return (0);
}