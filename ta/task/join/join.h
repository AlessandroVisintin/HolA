#ifndef _JOIN_H
#define _JOIN_H

#include "../../ta.h"

// sent as query
#define askJOIN 100
#define askSLIST 101
#define providePROOF 102
#define storePROOF 103
#define updateCHAIN 104
#define closeJOIN 105

#define timeoutJOIN maxTI
#define timeoutSLIST totTI
#define timeoutPROOF maxTI

// received as response
#define answJOIN 110
#define answSLIST 111
#define endJOIN 112

/*

Initialize join environment

*/
void join_init(void);

/*

Check if join is completed in every overlay

Output:
	true = join completed
	false = join not completed yet

*/
bool join_done(void);

/*

Join routine function

*/
ROfunc jo_outgoing[oveNU];


/*

Join incoming response

*/
void jo_incoming(void);

#endif