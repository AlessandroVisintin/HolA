#include "address.h"
#include <arpa/inet.h>
#include <linux/if_packet.h>
#include <netdb.h> // NI_MAXHOST, NI_NUMERICHOST
#include <ifaddrs.h> // getifaddrs

void ad_ipStoN(char *ip0, UC *ip1)
{
	struct in_addr ip2;
	
	if (inet_aton(&ip0[0], &ip2) == 0) 
		errx(1, "ad_ipStoN: conversion error");

	memcpy(ip1, (UC*) (&ip2.s_addr), ipaSZ);
}

errCO ad_get(char* range, UC *ip, UC *mac)
{
	char* name;
	char ip1[NI_MAXHOST];
	struct ifaddrs *a, *b;
	struct sockaddr_ll *s;

	if (!range || !ip || !mac)
		errx(1, "ad_get: bad parameters");
	
	if (getifaddrs(&a) < 0) 
		errx(1, "ad_get: get error");

	for (b = a; b != NULL; b = b->ifa_next)
	{
		if (b->ifa_addr == NULL || b->ifa_addr->sa_family != AF_INET) 
			continue;

		if (getnameinfo(
				b->ifa_addr, sizeof(struct sockaddr_in), 
				ip1, NI_MAXHOST, NULL, 0, NI_NUMERICHOST) != 0)
			errx(1, "ad_get: cannot get info");

		if (strstr(ip1, range) == NULL)
			continue;		

		ad_ipStoN(ip1, ip);
		name = b->ifa_name;
		break;
	}

	if (name)
	{
		#if (ENV == 2)
			for (int i = 0; i < macSZ; i++)
				mac[i] = (UC) (rand() % 255);

			freeifaddrs(a);
			return SUCCESS;

		#else

			for (b = a; b != NULL; b = b->ifa_next)
			{
				if (b->ifa_addr == NULL || 
					b->ifa_addr->sa_family != AF_PACKET ||
					strcmp(b->ifa_name, name) != 0)
					continue;
				
				s = (struct sockaddr_ll*) b->ifa_addr;

				memcpy(mac, &s->sll_addr[0], macSZ);
				freeifaddrs(a);

				return SUCCESS;
			}

		#endif	
	}

	freeifaddrs(a);
	return NOT_FOUND;
}