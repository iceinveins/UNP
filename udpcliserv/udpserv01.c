#include	"unp.h"

void dg_echo_direct(int, struct sockaddr *, socklen_t);
int
main(int argc, char **argv)
{
	int					sockfd;
	struct sockaddr_in	servaddr, cliaddr;

	sockfd = Socket(AF_INET, SOCK_DGRAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(sockfd, (SA *) &servaddr, sizeof(servaddr));

	// printf("sizeof	\n sockaddr_in=%ld\n sockaddr_in6=%ld\n sockaddr_un=%ld\n sockaddr_storage=%ld\n", sizeof(struct sockaddr_in), sizeof(struct sockaddr_in6), sizeof(struct sockaddr_un), sizeof(struct sockaddr_storage));
	// sizeof	
    // sockaddr_in=16
    // sockaddr_in6=28
    // sockaddr_un=110
    // sockaddr_storage=128

	// printf("cliaddr=%ld\n", sizeof(cliaddr));
	dg_echo_direct(sockfd, (SA *) &cliaddr, sizeof(cliaddr));
}

void dg_echo_direct(int sockfd, struct sockaddr * cliaddr, socklen_t socklen)	// socklen :sizeof(cliaddr) depend on it's IP protocol type
{
	int n;
	socklen_t clilen;
	char mesg[MAXLINE];

	for( ; ; )
	{
		clilen = socklen;
		n = Recvfrom(sockfd, mesg, MAXLINE, 0, cliaddr, &clilen);
		// printf("actual clilen=%d\n", clilen);
		Sendto(sockfd, mesg, n, 0, cliaddr, clilen);
	}
}