#include "client.h"
#include "server.h"
#include <pthread.h>

pthread_mutex_t m_print;

static void process_msg(UC *ptr)
{
	pthread_mutex_lock(&m_print);
	printf("Message received...\n");
	memprint(ptr, bufSZ);
	pthread_mutex_unlock(&m_print);
	memset(ptr, 0, bufSZ);
}

static void* start_server(void *s)
{
	se_open(process_msg);
	pthread_exit(NULL);
}

int main(int argc, char const *argv[])
{
	pthread_t tid;
	UC addr[ipaSZ] = {127, 0, 0, 1};
	UC buff[bufSZ];

	if (pthread_mutex_init(&m_print, NULL) != 0)
		errx(1, "main: mutex init failed");

	printf("Creating message...\n");

	memcpy(bu_getAddr(buff), addr, ipaSZ);
	serialize(datSZ, bu_getLeng(buff));
	memset(bu_getData(buff), 10, datSZ);

	printf("Printing message...\n");
	memprint(buff, bufSZ);

	printf("Trying to open server...\n");
	if (pthread_create(&tid, NULL, start_server, NULL) < 0)
	 	errx(1, "main: server thread failed");

	printf("Waiting for server...\n");
	if(!se_wait())
	{
		printf("Server not available!\n");
		return 0;
	}

	printf("Trying to send message...\n");
	errCO r = cl_notify(bu_getAddr(buff), bu_getLeng(buff), 4+datSZ); 

	pthread_mutex_lock(&m_print);

	if (r == NOT_CONNECTING)
		printf("Server not connecting!\n");
	else if (r == NOT_RESPONDING)
		printf("Server not responding!\n");
	else
		printf("Message sent successfully!!\n");

	printf("Trying to close server...\n");
	
	pthread_mutex_unlock(&m_print);

	se_close();

	return 0;
}