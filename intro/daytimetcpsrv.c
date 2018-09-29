#include	"unp.h"
#include	<time.h>

int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	struct sockaddr_in	servaddr;
	char				buff[MAXLINE];
	time_t				ticks;

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(9999);	/* daytime server */

	// Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	struct sockaddr_in cliaddr;
	socklen_t addrlen = sizeof(cliaddr);
	for ( ; ; ) {
		connfd = Accept(listenfd, (SA *) &cliaddr, &addrlen);
		printf("Connection from %s, port %d\n", inet_ntop(AF_INET, &cliaddr.sin_addr, buff, sizeof(buff)), ntohs(cliaddr.sin_port));
        ticks = time(NULL);
        snprintf(buff, sizeof(buff), "%.24s\r\n", ctime(&ticks));
        int i=0,siezeperwrite = 2;
        for (i = 0; i < strlen(buff); i=i+siezeperwrite)
        {
        	Write(connfd, buff+i, siezeperwrite);
        }

		Close(connfd);
	}
}
