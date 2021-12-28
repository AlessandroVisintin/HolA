#include "client.h"
#include "network.h"
#include <sys/socket.h>

errCO cl_notify(UC ip[ipaSZ], UC* msg, int len)
{
	errCO sockfd = ne_clientCreate(ip, 22222);

	if (sockfd == NOT_CONNECTING)
		return NOT_CONNECTING;

	if(send(sockfd, msg, len, 0) < 0)
	{
		close(sockfd);
		errx(1, "cl_notify: send error");
	}

	UC dummy[1];
	if(recv(sockfd, dummy, 1, 0) < 0)
	{
		close(sockfd);
		return NOT_RESPONDING;
	}

	close(sockfd);
	return SUCCESS;
}


errCO cl_check(UC ip[ipaSZ])
{
	errCO sockfd = ne_clientCreate(ip, 22222);

	if (sockfd == NOT_CONNECTING)
		return NOT_CONNECTING;
	
	close(sockfd);
	return SUCCESS;	
}