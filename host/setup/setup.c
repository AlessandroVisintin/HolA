#include "setup.h"
#include "address.h"
#include "../network/client.h"
#include "../network/monitor.h"
#include <pthread.h>

void* se_monitor(void *s)
{
	mo_open();
	pthread_exit(NULL);
}

errCO se_hidra(UC buf[bufSZ])
{
	UC entry[ipaSZ];
	pthread_t tid;

	memset(buf, 0, bufSZ);

	UC *ptr = bu_getData(buf);

	if (ad_get(RANGE, pa_getIpad(ptr), pa_getMaca(ptr)) == NOT_FOUND)
		errx(1, "se_hidra: network not available");

	unsigned acoolseed;
	memcpy(&acoolseed, pa_getIpad(ptr), sizeof(unsigned));
	srand(acoolseed);
	// printf("S: %u\n", acoolseed);
	// exit(0);

	ad_ipStoN(ENTRY, &entry[0]);

	if (memcmp(pa_getIpad(ptr), entry, ipaSZ) != 0)
	{
		if (cl_check(entry) != SUCCESS)
	 		return NOT_CONNECTING;

	 	pa_setEntr(ptr, entry);
	}
	else
	{
		printf("** HiDRA: starting monitor...\n");
		if (pthread_create(&tid, NULL, se_monitor, NULL) < 0)
			errx(1, "HiDRA: server thread failed");

		printf("** HiDRA: waiting monitor...\n");
		if (!mo_wait())
		{
		 	printf("** HiDRA: monitor not working, closing...\n");
		 	return 0;
		}
	}
	serialize(parSZ, bu_getLeng(buf));

	return SUCCESS;
}
