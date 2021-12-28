#include "network.h"
#include <sys/socket.h>
#include <netinet/in.h>

static int ne_createClientSocket(void)
{
	int s = socket(AF_INET, SOCK_STREAM, 0);

	if (s < 0)
		errx(1, "network_createClientSocket: failed");

	struct timeval connection = {0, conTI * 1000L};
	struct timeval exchange = {0, excTI * 1000L};

	// TImeout for connect
	if (setsockopt(s, SOL_SOCKET, SO_SNDTIMEO,
			(const char*) &connection, sizeof connection) == -1)
		errx(1, "network_createClientSocket: set send failed");

	// TImeout for responses
	if (setsockopt(s, SOL_SOCKET, SO_RCVTIMEO,
			(const char*) &exchange, sizeof exchange) == -1)
		errx(1, "network_createClientSocket: set recv failed");

	return s;
}

static int ne_createServerSocket(void)
{
	int reuse = 1;
	int sendbuff = 200000;
	int s = socket(AF_INET, SOCK_STREAM, 0);

	if (s < 0)
		errx(1, "network_createServerSocket: failed");

	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(int)) == -1)
		errx(1, "network_createServerSocket: no reuse");

	if (setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &sendbuff, sizeof(sendbuff)) == -1)
		errx(1, "network_createServerSocket: no extended buffer");

	return s;
}

static struct sockaddr_in ne_createAddress(UI ip, unsigned short port)
{
	struct sockaddr_in addr;
	memset(&addr, 0, sizeof(addr));

	if (ip == 0)
		addr.sin_addr.s_addr = INADDR_ANY;
	else
		addr.sin_addr.s_addr = ip;

	addr.sin_family = AF_INET;
	addr.sin_port = htons(port);

	return addr;
}

int ne_serverCreate(unsigned short port)
{
	int sockfd = ne_createServerSocket();
	struct sockaddr_in addr = ne_createAddress(0, port);

	if (bind(sockfd, (struct sockaddr*) &addr, sizeof(addr)) < 0)
		errx(1, "network_serverCreate : port binding failed");

	if (listen(sockfd, 500) < 0)
		errx(1, "network_serverCreate: listen failed");

	return sockfd;
}

errCO ne_clientCreate(UC ip[ipaSZ], unsigned short port)
{
	int sockfd = ne_createClientSocket();

	struct sockaddr_in addr = ne_createAddress(deserialize(ip, false), port);

	if (connect(sockfd, (struct sockaddr*) &addr, sizeof(addr)) != 0)
	{
		close(sockfd);
		return NOT_CONNECTING;
	}
	return sockfd;
}
