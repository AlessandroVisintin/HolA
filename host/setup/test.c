#include "address.h"
#include "setup.h"

int main(int argc, char const *argv[])
{
	UC ip[ipaSZ], mac[macSZ], buf[bufSZ];

	printf("Searchin in range %s...\n", RANGE);

	memset(ip, 0, ipaSZ);
	if (ad_get(RANGE, ip, mac) == SUCCESS)
	{
		printf("Your IP is: ");
		memprint(ip, ipaSZ);
		
		printf("Your MAC is: ");
		memprint(mac, macSZ);
	}
	else
		printf("No data found\n");

	printf("Trying to prepare SETUP message...\n");

	if (se_hidra(buf) == NOT_CONNECTING)
		printf("Entry not found\n");
	else
	{
		printf("Printing message...\n");
		memprint(buf, bufSZ);
	}

	return 0;
}