#ifndef _SETUP_H
#define _SETUP_H

#include "../host.h"

/*

Prepare message setup for HiDRA environment

Input:
	UC buf[bufSZ] = output buffer

Output:
	SUCCESS = setup message ready
	NOT_CONNECTING = entry node not available

*/
errCO se_hidra(UC buf[bufSZ]);

#endif