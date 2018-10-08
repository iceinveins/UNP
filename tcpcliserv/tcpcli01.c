#include	"unp.h"

void str_cli_select(FILE* fp, int sockfd);
void str_cli_poll(FILE* fp, int sockfd);
int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr;

	if (argc != 2)
	{
		err_quit("usage: tcpcli <IPaddress>");
	}

	sockfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(SERV_PORT);
	Inet_pton(AF_INET, argv[1], &servaddr.sin_addr);

	Connect(sockfd, (SA *) &servaddr, sizeof(servaddr));

	str_cli_select(stdin, sockfd);		/* do it all */

	exit(0);
}

void str_cli_select(FILE* fp, int sockfd)
{
	int maxfdp1;
	fd_set rset;
	char	sendline[MAXLINE], recvline[MAXLINE];
	bool eofflag = 0;

	FD_ZERO(&rset);
	for( ; ; )
	{
		if(eofflag == 0)
		{
			FD_SET(fileno(fp), &rset);
		}
		FD_SET(sockfd, &rset);
		maxfdp1 = max(fileno(fp), sockfd) + 1;
		Select(maxfdp1, &rset, NULL, NULL, NULL);
		
		if(FD_ISSET(sockfd, &rset))
		{
			if(Read(sockfd, recvline, MAXLINE) == 0)
				if(eofflag == 1)
				{
					return;
				}
				else
				{
					err_quit("server terminated prematurely");
				}
			Writen(fileno(stdout), recvline, strlen(recvline));
		}

		if(FD_ISSET(fileno(fp), &rset))
		{
			if(Read(fileno(fp), sendline, MAXLINE) == 0)	// receive EOF ,close WRITE-HALF on client and don't select it again
			{
				eofflag = 1;
				Shutdown(sockfd, SHUT_WR);	// close WRITE-HALF on client
				FD_CLR(fileno(fp), &rset);	// don't select it again
				continue;
			}
			Writen(sockfd, sendline, strlen(sendline));
		}
	}
}

