#include	"unp.h"


int inet_pton_loose(int family, const char* strptr, void* sockptr)
{
	if (AF_INET == family)
	{
		int result = inet_pton(family,strptr,sockptr);
		if( result == 0)	/*check if strptr is XX.XX.XX.XX,if not return 0.*/
		{	
			/*then try use inet_aton, it is much loose*/
			struct in_addr * tmp = (struct in_addr*)sockptr;
			if(inet_aton(strptr,(struct in_addr*)sockptr)<=0)
			{
				return -1;
			}
			else
			{
				return tmp->s_addr;
			}
		}
		else
		{
			return result;
		}
	}
	else if (AF_INET == family)
	{
		int result = inet_pton(family,strptr,sockptr);
		if( result == 0)	/*check if strptr is XX.XX.XX.XX,if not return 0.*/
		{	
			/*then try use inet_aton, it is much loose*/
			struct in_addr * tmp = (struct in_addr*)sockptr;
			if(inet_aton(strptr,(struct in_addr*)sockptr)<=0)
			{
				return -1;
			}
			else
			{
				return tmp->s_addr;
			}
		}
		else
		{
			return result;
		}
	}
	else
	{
		return -3;
	}
}
int
main(int argc, char **argv)
{
	int					sockfd, n;
	char				recvline[MAXLINE + 1];
	struct sockaddr_in	servaddr;

	if (argc != 2)
		err_quit("usage: a.out <IPaddress>");

	if ( (sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port   = htons(9999);	/* daytime server */
	if (inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	/*-----------------------------------------------------------*/
	struct sockaddr_in cliaddr;
	bzero(&cliaddr,sizeof(cliaddr));
	cliaddr.sin_family = AF_INET;
	cliaddr.sin_addr.s_addr = htons(INADDR_ANY);
	cliaddr.sin_port = htons(7777);
	Bind(sockfd,(SA *)&cliaddr,sizeof(cliaddr));
	/*-----------------------------------------------------------*/
	if (connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	int count = 0;
	while ( (n = read(sockfd, recvline, MAXLINE)) > 0) {
		++count;
		recvline[n] = 0;	/* null terminate */
		if (fputs(recvline, stdout) == EOF)
			err_sys("fputs error");
	}
	if (n < 0)
		err_sys("read error");

	printf("%d\n", count);
	const char* strptr= "0xe";
	struct in_addr nip;
	bzero(&nip,sizeof(nip));
	inet_pton(AF_INET,strptr,&nip);
	printf("%d\n", nip.s_addr);

	printf("%d\n", inet_pton_loose(AF_INET,strptr,&nip));

	exit(0);
}
