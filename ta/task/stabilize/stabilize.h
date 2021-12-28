#ifndef _STABILIZE_H
#define _STABILIZE_H

#include "../../ta.h"
#include "../rectify/rectify.h"

// sent as query
#define askPSUCC 120
#define askNSUCC 121
#define sendREGARD 122
#define sendGREET 123
#define purgePROOF 124

#define timeoutPSUCC totTI
#define timeoutNSUCC totTI

// received as response
#define answPSUCC 130
#define answNSUCC 131

/*

Initialize stabilize environment

*/
void stabilize_init(void);

/*

Stabilize routine function

*/
ROfunc st_outgoing[oveNU];

/*

Stabilize incoming function

*/
void st_incoming(void);

#endif