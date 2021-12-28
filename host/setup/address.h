#ifndef _ADDRESS_H
#define _ADDRESS_H

#include "../host.h"

/*

Convert IP from string format to network array

Input:
	char *ip0 = ip string
	unCH *ip1 = conversion result	

Example:
char *ip0 = "192.168.42.1" --> UC *ip1 = {192,168,42,1} 

*/
void ad_ipStoN(char *ip0, UC *ip1);

/*

Get IP and MAC address of current device from given 
network range

Input:
	char* maskCk = network range to search for 
	UC ip[ipaSZ] = IP value found 
	UC mac[macSZ] = MAC value found

Output:
	SUCCESS = values retrieved
	NOT_FOUND = network range not found

*/
errCO ad_get(char* range, UC *ip, UC *mac);

#endif