#ifndef _RECTIFY_H
#define _RECTIFY_H

#include "../../ta.h"

// sent as query
#define askOPREV 140
#define movePROOF 141
#define closeRECTIFY 142

#define timeoutOPREV totTI
#define timeoutRELOC totTI

// received as response
#define answOPREV 150
#define continuePROOF 151
#define endRECTIFY 152

extern bool re_new[oveNU];
extern UC re_info[oveNU][plaSZ];

/*

Initialize join environment

*/
void rectify_init(void);

/*

Check if new rectify is ready

Output:
	true = new rectify ready
	false = no new rectify

*/
bool rectify_checkNew(void);

/*

Rectify routine function

*/
ROfunc re_outgoing[oveNU];

/*

Recitfy incoming function

*/
void re_incoming(void);

#endif