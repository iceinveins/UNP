#include	"unp.h"

int
main(int argc, char **argv)
{
	int sockfd;
	struct sockaddr_in servaddr;
	struct in_addr **pptr, *tmp[2];
	struct hostent*	ht;
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr)==1)
	{
		tmp[0] = &servaddr.sin_addr;
		tmp[1] = nullptr;
		pptr   = tmp;
	}
	else if(ht = gethostbyname(argv[1]))
	{
		pptr = (struct in_addr **)ht->h_addr_list;
	}
	else
	{
		error_quit("host name error for %s:%s", argv[1], hstrerror(h_errno));
	}
	
}
