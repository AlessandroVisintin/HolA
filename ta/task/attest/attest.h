#ifndef _ATTEST_H
#define _ATTEST_H

#include "../../ta.h"

// sent as query
#define askRAND 160
#define startCHAL 161
#define forwPROOF 162
#define checkPROOF 163
#define devCHECKED 164
#define devTAMPERED 165
#define restorePROOF 166
#define savePROOF 167
#define restoreMISSING 168
#define saveMISSING 169
#define closeRESTORE 170

#define timeoutRAND maxTI
#define timeoutCHAL maxTI
#define timeoutRESTORE maxTI

// received as response
#define acceptCHAL 171
#define returnPROOF 172
#define lostPROOF 173
#define restoredPROOF 174
#define restoredMISSING 175

/*

Initialize attest environment

*/
void attest_init(void);

/*

Check if current overlay already did attestation

Output:
	true = attestation done
	false = attestation not done

*/
bool attest_isDone(void);

/*

Check if at least one overlay is in lost-state

Output:
	true = at least one overlay is in lost-state
	false = all overlay are fine

*/
bool attest_checkLost(void);

/*

Attest routine function

*/
ROfunc at_outgoing[oveNU];

/*

Attest incoming function

*/
void at_incoming(void);

#endif