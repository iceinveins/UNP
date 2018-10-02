#include	"unp.h"

void sig_chld233(int signo);
void str_echo233(int connfd);
int
main(int argc, char **argv)
{
	int					listenfd, connfd;
	pid_t				childpid;
	socklen_t			clilen;
	struct sockaddr_in	cliaddr, servaddr;
	void sig_chld233(int);

	listenfd = Socket(AF_INET, SOCK_STREAM, 0);

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port        = htons(SERV_PORT);

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));

	Listen(listenfd, LISTENQ);

	Signal(SIGCHLD, sig_chld233);		// handle zombie progress

	for ( ; ; ) {
		clilen = sizeof(cliaddr);
		if ( (connfd = accept(listenfd, (SA *) &cliaddr, &clilen)) < 0)	// handle accept error:EINTR(caused by SIGCHLD) on some machine
		{	
			if(errno == EINTR)
				continue;
			else
				err_sys("accept error");
		}

		if ( (childpid = Fork()) == 0) {	/* child process */
			Close(listenfd);	/* close listening socket */
			str_echo233(connfd);	/* process the request */
			exit(0);
		}
		Close(connfd);			/* parent closes connected socket */
	}
}

void sig_chld233(int signo)
{
	pid_t pid;
	int stat;

	while( (pid = waitpid(-1, &stat, WNOHANG)) > 0)	// must call waitpid ,cause Unix signals don't have queue, 
	{													// so if kernal get N SIGCHLD, it only handle once 
		printf("child %d terminated\n", pid);
	}
	return;
}

void str_echo233(int connfd)
{
	ssize_t		n;
	char		buf[MAXLINE];

	for( ; ; )
	{
		if( (n = read(connfd, buf, MAXLINE)) > 0)
		{
			Writen(connfd, buf, n);
		}
		

	}
// again:
// 	while ( (n = read(connfd, buf, MAXLINE)) > 0)
// 	{
// 		printf("call read ,size = %d\n", n);
// 		Writen(connfd, buf, n);
// 		printf("Writen successul\n");
// 	}

// 	if (n < 0 && errno == EINTR)
// 		goto again;
// 	else if (n < 0)
// 		err_sys("str_echo233: read error");
}