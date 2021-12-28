#ifndef _MONITOR_H
#define _MONITOR_H

#include "../host.h"

/*

Start monitor service

*/
void mo_open(void);

/*

Wait monitor service

Output:
	true = server ready
	false = server not alive 

*/
bool mo_wait(void);

/*

Close server service 

*/
void mo_close(void);

/*

Send a message to monitor

Input:
	UC ip[ipaSZ] = ip address (default port 2223)
	UC* msg = message to be sent
	int len = length of message in bytes

Output:
	SUCCESS = message successfully sent
	NOT_CONNECTING = monitor not alive
	NOT_RESPONDING = monitors not replying

*/
errCO mo_notify(UC *msg, int len);

#endif