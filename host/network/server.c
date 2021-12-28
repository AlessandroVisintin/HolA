#include "server.h"
#include "client.h"
#include "monitor.h"
#include "network.h"
#include <pthread.h>
#include <netinet/in.h>

static int p_sock;
static fPRO func;
static fd_set wait_set, read_set;
static volatile bool se_active = false;

void* se_send(void *s)
{
	UC *b = (UC*) s;
	UC *adr = bu_getAddr(b);
	UC *len = bu_getLeng(b);

	if (
		cl_notify(adr, len, 4+deserialize(len,true)) == SUCCESS &&
		(ro_getMoni(bu_getData(b)) == 1)
	)
	{
		mo_notify((UC*) (&(b[bu_ofsData + ro_ofsMoni])), monSZ);
	}

	free(b);
	pthread_exit(NULL);
}

void se_exe(int sockfd)
{
	int n;
	pthread_t tid;
	UC *b = (UC*) malloc(bufSZ);

	n = read(sockfd, bu_getLeng(b), 4);
	if (n < 0)
	{
		printf("se_exe: %s\n", strerror(errno));
		close(sockfd);
		return;
	}
	else if (n == 0)
	{
		close(sockfd);
		return;
	}

	n = read(sockfd, bu_getData(b), deserialize(bu_getLeng(b), true));
	if (n < 0)
	{
		close(sockfd);
		printf("se_exe: %s\n", strerror(errno));
		return;
	}
	else if (n == 0)
	{
		close(sockfd);
		return;
	}

	close(sockfd);
	func(b);

	if (!memisnull(bu_getAddr(b), ipaSZ))
	{
		if (pthread_create(&tid, NULL, se_send, (void*) b) < 0)
			errx(1, "HiDRA: server client thread failed");
	}
	else
		free(b);
}

void se_open(fPRO f)
{
	socklen_t addr_size;
	struct sockaddr_in addr;

	p_sock = ne_serverCreate(22222);
	func = f;

	FD_ZERO(&wait_set);
	FD_SET(p_sock, &wait_set);

	se_active = true;
	while (1)
	{
		read_set = wait_set;
		if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0)
			errx(1, "se_open: select failed");

		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &read_set))
			{
				if (i == p_sock)
				{
					int sockfd = accept(p_sock, (struct sockaddr*) &addr, &addr_size);

					if(sockfd < 0)
						errx(1, "se_open: accept failed");

					FD_SET (sockfd, &wait_set);
				}
				else
				{
					se_exe(i);
					FD_CLR (i, &wait_set);
				}
			}
		}
	}
}

bool se_wait(void)
{
	for (int i = 0; i < 10; i++)
	{
		if (se_active)
			return true;
		sleep(1);
	}
	return false;
}

void se_close(void)
{
	if (se_active)
	{
		printf("HiDRA: closing server...\n");

		se_active = false;
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &wait_set))
				close(i);
		}
	}
}
