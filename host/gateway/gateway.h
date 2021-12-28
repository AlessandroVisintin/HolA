#ifndef _GATEWAY_H
#define _GATEWAY_H

#include "../host.h"

/*

Request outgoing message

Input:
	UC buf[bufSZ] = resulting message

*/
void ga_outgoing(UC buf[bufSZ]);

/*

Process incoming message

Input:
	UC buf[bufSZ] = incoming message and resulting message

*/
void ga_incoming(UC buf[bufSZ]);

#endif