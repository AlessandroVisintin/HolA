#ifndef _NETWORK_H
#define _NETWORK_H

#include "../host.h"

/*

Create a server on a given port and start to listen on it 

Input:
	unsigned short port = port for the server

Output:
	file descriptor of opened server	

*/
int ne_serverCreate(unsigned short port);

/*

Create a client connected to a specific IP and PORT

Input:
	UC ip[ipaSZ] = ip address
	unsigned short port = port

Output:
	file descriptor of opened client
	NOT_CONNECTING = server not responding 


*/
errCO ne_clientCreate(UC ip[ipaSZ], unsigned short port);

#endif