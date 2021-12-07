#include "../incs/ft_traceroute.h"

unsigned short checksum(void *b, int len)
{    
    unsigned short  *buf;
    unsigned int    sum;
    unsigned short  result;
  
    buf = b;
    sum = 0;
    result = 0;
    for ( sum = 0; len > 1; len -= 2 )
        sum += *buf++;
    if ( len == 1 )
        sum += *(unsigned char*)buf;
    sum = (sum >> 16) + (sum & 0xFFFF);
    sum += (sum >> 16);
    result = ~sum;
    return result;
}

void fill_icmp_hdr(t_icmphdr *icmp, u_int16_t pid, time_t timestamp)
{
    bzero(icmp, sizeof(t_icmphdr));
    // REPRENDRE POUR L'OPTION S OU REMETTRE LE PADDING EN PADDING[] DNAS LE .H LE PADDING FAIT TOUJOURS 8 EN SIZE
	icmp->icmp_hdr.type = 8;
	icmp->icmp_hdr.un.echo.id = ((pid << 8) & 0xff00) | ((pid >> 8) & 0x00ff);
	icmp->icmp_hdr.un.echo.sequence = 1;
    icmp->icmp_hdr.un.echo.sequence = ((icmp->icmp_hdr.un.echo.sequence << 8) & 0xff00) | ((icmp->icmp_hdr.un.echo.sequence >> 8) & 0x00ff);
	icmp->timestamp = timestamp;
    icmp->icmp_hdr.checksum = checksum(icmp, sizeof(t_icmphdr));
}

int resolve_ip(char *addr_host, struct sockaddr_in *addr_con) // IP string vers IP decimale (utilisable), et hostname vers IP decimale
{
	struct hostent 	*host_entity;
	
	if ((host_entity = gethostbyname(addr_host)) == NULL)
		return (-1);
	(*addr_con).sin_family = host_entity->h_addrtype;
	(*addr_con).sin_port = htons(0);
	(*addr_con).sin_addr.s_addr = *(long*)host_entity->h_addr;
	return (0);
}

void sent_request(int sock, struct sockaddr_in *addr_con, t_icmphdr icmp, u_int16_t pid, struct timeval tv_seq_start)
{
    fill_icmp_hdr(&icmp, pid, tv_seq_start.tv_sec);
    if (sendto(sock, &icmp, sizeof(icmp), 0, (struct sockaddr*)addr_con, sizeof(*addr_con)) <= 0)
        printf("\nPacket Sending Failed!\n");
}

int main(int argc, char **argv)
{
    int 	        sock;
    int             ttl;
    t_icmphdr 		icmp;
    struct timeval 	tv_seq_start;
    u_int16_t       pid;
    struct 	sockaddr_in addr_con;

    if (argc != 2)
        return (-1);
    pid = getpid();
    ttl = 0;
    if (resolve_ip(argv[1], &addr_con) != 0)
		exit(1);
    sock = socket(AF_INET, SOCK_RAW, IPPROTO_ICMP);
    if (sock < 0)
        return (-1);
    setsockopt(sock, SOL_IP, IP_TTL, &ttl, sizeof(ttl));
    if (gettimeofday(&tv_seq_start, NULL) == -1) // on sort de la fonction ? Pas de probleme avec un return ; tout est free apres
        return (-1); // gerer erreur normalement
    sent_request(sock, &addr_con, icmp, pid, tv_seq_start);
}