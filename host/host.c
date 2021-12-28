#include "host.h"
#include "setup/setup.h"
#include "network/server.h"
#include "network/client.h"
#include "network/monitor.h"
#include "gateway/gateway.h"
#include <signal.h>
#include <pthread.h>
#include <sys/time.h>

static volatile bool hidra_running = true;
pthread_mutex_t m_msg;

#if (TEST==0)

	void* ho_server(void *s)
	{
		se_open(ga_incoming);
		pthread_exit(NULL);
	}

	void ho_closing(int dummy)
	{
			printf("** HiDRA: I am closing! ");
	    hidra_running = false;
	    printf("\n");
	}

	int main(int argc, char const *argv[])
	{
		pthread_t tid;
		suseconds_t s;
		UC buf[bufSZ];

		signal(SIGINT, ho_closing);
		signal(SIGTERM, ho_closing);
		signal(SIGKILL, ho_closing);
		signal(SIGQUIT, ho_closing);

		for (int i = 0; i < 5; ++i)
		{
			printf("** HiDRA: start-up (%d tries left)...\n", 5-i);
			if (se_hidra(buf) == SUCCESS)
			{
				ga_incoming(buf);
				break;
			}

			if (i < 4)
			{
				printf("** HiDRA: not connecting, wait 5 seconds...\n");
				sleep(5);
			}
			else
			{
				printf("** HiDRA: not responding, closing...\n");
				return 0;
			}
		}

		if (pthread_mutex_init(&m_msg, NULL) != 0)
			errx(1, "se_open: mutex init failed");

		printf("** HiDRA: starting server...\n");
		if (pthread_create(&tid, NULL, ho_server, NULL) < 0)
			errx(1, "HiDRA: server thread failed");

		printf("** HiDRA: waiting server...\n");
		if (!se_wait())
		{
		 	printf("** HiDRA: server not working, closing...\n");
		 	return 0;
		}

		printf("** HiDRA: starting protocol\n");
		while(hidra_running)
		{
			ga_outgoing(buf);

			UC *adr = bu_getAddr(buf);
			UC *len = bu_getLeng(buf);
			if (
				(!memisnull(adr, ipaSZ)) &&
				(cl_notify(adr, len, 4+deserialize(len,true)) == SUCCESS) &&
				(ro_getMoni(bu_getData(buf)) == 1)
			)
			{
				mo_notify((UC*) (&(buf[bu_ofsData + ro_ofsMoni])), monSZ);
			}

			s = ((rand() % (MAXSLEEP-MINSLEEP)) + MINSLEEP) * 1000L;
			usleep(s);
		}

		se_close();
		mo_close();
		return 0;
	}
#endif
