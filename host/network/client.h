#ifndef _CLIENT_H
#define _CLIENT_H

#include "../host.h"

/*

Send a message in UDP-fashion.
	(Open connection, Send message, Close Connection) 

Input:
	UC ip[ipaSZ] = ip address (default port 2222)
	UC* msg = message to be sent
	int len = length of message in bytes

Output:
	SUCCESS = message successfully sent
	NOT_CONNECTING = server not alive
	NOT_RESPONDING = server not replying

*/
errCO cl_notify(UC ip[ipaSZ], UC* msg, int len);

/*

Check if server is alive 

Input:
	UC ip[ipaSZ] = ip address

Output:
	SUCCESS = server alive
	NOT_CONNECTING = server not alive

*/
errCO cl_check(UC ip[ipaSZ]);

#endif