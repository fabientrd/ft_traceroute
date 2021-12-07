#include "../incs/ft_traceroute.h"

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
}

void    manage_env(t_env *env, char **av){
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

int     main(int ac, char **av){
    t_env env;

    init_env(&env);
    manage_env(&env, av);
	if (getuid() != 0 || env.h || env.err || ac == 1){
	    getuid() != 0 ? printf("This program sends raw socket, you must be root or sudoers to use it.\n") : usage(env.h, env.err);
		return env.h ? EXIT_SUCCESS : EXIT_FAILURE;
	}
    printf("env->dest = %s\n", env.dest);
    return (0);
}