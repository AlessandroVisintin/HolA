#ifndef _SERVER_H
#define _SERVER_H

#include "../host.h"

// Function for processing message buffer
typedef void (*fPRO)(UC*);

/*

Start server service

Input:
	fPRO f = function for processing messages 

*/
void se_open(fPRO f);

/*

Wait server service

Output:
	true = server ready
	false = server not alive 

*/
bool se_wait(void);

/*

Close server service 

*/
void se_close(void);

#endif