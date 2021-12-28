#include "monitor.h"
#include "network.h"
#include "../setup/address.h"
#include <sys/socket.h>
#include <netinet/in.h>

static int p_sock;
static FILE *log_file;
static fd_set wait_set, read_set;
static volatile bool mo_active = false;

void mo_exe(int sockfd)
{
	int n;
	UC b[monSZ];

  	n = read(sockfd, b, monSZ);
  	if (n < 0)
  		printf("mo_exe: %s\n", strerror(errno));

  	else if (n > 0)
  	{
  		fwrite(b, monSZ, 1, log_file);

  	// 	UC *ptr = (UC*) (&(b[monSZ-ro_heaSZ-ro_bodSZ]));

  	// 	if (
  	//  		(ro_getOpco(ptr) >= 165 && ro_getOpco(ptr) <= 170) ||
  	//  		(ro_getOpco(ptr) >= 173 && ro_getOpco(ptr) <= 175)
  	//  	)
  	//  	{
	 	// 	printf("INCOMING: \n");
			// printf("Overlay: %u\n", ro_getOver(ptr));
			// printf("Opcode: %u\n", ro_getOpco(ptr));
			// printf("Process:\n");
			// memprint(ro_getProc(ptr), 4);
			// printf("Sequence: %u\n", ro_getSequ(ptr));
			// printf("Source: ");
			// memprint(ro_getSour(ptr), plaSZ);
			// printf("Sender: ");
			// memprint(ro_getSend(ptr), plaSZ);
			// printf("Recipient: ");
			// memprint(ro_getReci(ptr), plaSZ);
			// printf("Body:\n");
			// memprint(ro_getPlat(ptr, 0), ro_bodLE);
			// printf("\n");
  	//  	}
  	}

	close(sockfd);
}

void mo_open(void)
{
	socklen_t addr_size;
	struct sockaddr_in addr;

	log_file = fopen("./log.txt", "wb");
	p_sock = ne_serverCreate(22223);

	FD_ZERO(&wait_set);
	FD_SET(p_sock, &wait_set);

	mo_active = true;
	while (1)
	{
		read_set = wait_set;
		if (select(FD_SETSIZE, &read_set, NULL, NULL, NULL) < 0)
			errx(1, "mo_open: select failed");

		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &read_set))
			{
				if (i == p_sock)
				{
					int sockfd = accept(p_sock, (struct sockaddr*) &addr, &addr_size);

					if(sockfd < 0)
						errx(1, "mo_open: accept failed");

					FD_SET (sockfd, &wait_set);

					printf("Request from\t");
					memprint((UC*) &addr.sin_addr, ipaSZ);
				}
				else
				{
					mo_exe(i);
					FD_CLR (i, &wait_set);
				}
			}
		}
	}
}

bool mo_wait(void)
{
	for (int i = 0; i < 10; i++)
	{
		if (mo_active)
			return true;
		sleep(1);
	}
	return false;
}

void mo_close(void)
{
	if (mo_active)
	{
		printf("HiDRA: closing monitor...\n");
		mo_active = false;
		for (int i = 0; i < FD_SETSIZE; i++)
		{
			if (FD_ISSET(i, &wait_set))
				close(i);
		}
		fclose(log_file);
	}
}

errCO mo_notify(UC *msg, int len)
{
	UC entry[ipaSZ];
	ad_ipStoN(ENTRY, (UC*) (&(entry[0])));

	errCO sockfd = ne_clientCreate(entry, 22223);

	if (sockfd == NOT_CONNECTING)
		return NOT_CONNECTING;

	if(send(sockfd, msg, len, 0) < 0)
	{
		close(sockfd);
		errx(1, "mo_notify: send error");
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
