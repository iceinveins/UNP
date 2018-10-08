#include	"unp.h"

void dg_cli_direct(FILE *fp, int sockfd, struct sockaddr *, socklen_t);
void Bind2(int sockfd);

int main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: udpcli <IPaddress>");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	Bind2(sockfd);
	dg_cli_direct(stdin, sockfd, (SA *) &servaddr, sizeof(servaddr));

	exit(0);
}

void dg_cli_direct(FILE *fp, int sockfd, struct sockaddr *servaddr, socklen_t socklen)
{
	int n;
	char sendline[MAXLINE];
	char recvline[MAXLINE+1];
	socklen_t servlen;
	struct sockaddr *reply_addr;
	reply_addr = Malloc(socklen);
	
	while(Fgets(sendline, MAXLINE, fp) > 0)
	{
		Sendto(sockfd, sendline, strlen(sendline), 0, servaddr, socklen);

		servlen = socklen;
		n = Recvfrom(sockfd, recvline, MAXLINE, 0, reply_addr, &servlen);
		if(servlen != socklen || memcmp(servaddr, reply_addr, servlen) != 0 )
		{
			printf("reply from %s (ignored)\n", Sock_ntop(reply_addr, servlen));
			continue;
		}
		recvline[n] = 0;
		Fputs(recvline, stdout);
	}
}

void Bind2(int sockfd)
{
	struct sockaddr_in cliaddr;
	bzero(&cliaddr,sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	char ipaddr[10] = "127.0.0.1";		// try bind actual ip on different device
	Inet_pton(AF_INET, ipaddr, &cliaddr.sin_addr);
	cliaddr.sin_port = htons(7777);
	Bind(sockfd,(SA *)&cliaddr,sizeof(cliaddr));
}