#ifndef _TA_H
#define _TA_H

#include <global.h>

#if (TEST == 0)
#if (ENV == 0)
	#include <trace.h>
	#include <tee_internal_api.h>
	#include <tee_internal_api_extensions.h>

	typedef TEE_Time Clock;
	typedef uint32_t millis;
	extern uint32_t curCO;
#else
	#include <time.h> 
	#include <sys/time.h>
	typedef struct timeval Clock;
	typedef suseconds_t millis;
	typedef errCO TEE_Result;
	extern int curCO;
#endif
#else
	#include <time.h> 
	#include <sys/time.h>
	extern UI curCO;
#endif

/*

Routine function pointer

*/
typedef void (*ROfunc)(void);

/*

Environmental

*/
extern bool membership[oveNU]; // part of the ring
extern UC curOV; // overlay
extern UC curOP; // opcode
extern UC curME; // message
extern UC *curBU; // buffer
extern UC *curDA; // data pointer
extern UC rcvBU[maiSZ]; // decrypted buffer
extern UC sndBU[maiSZ]; // decrypted result
extern UC *curPO; // attest binary
extern int curLE; // attest length

// just for randomness problems
extern UC curMAC[macSZ];

/*

Check if device is member of ring in all
	overlays

*/
bool check_membership(void);

/*

Generate random bytes

Input:
	UC* result = random bytes result
	int len = number of bytes to generate

*/
void gen_random(UC *result, int len);

#if (TEST == 0)
/*

Set clock to current time

Input:
	Clock *t = clock to be set

*/
void clock_set(Clock* t);

/*

Return time passed from the input in milliseconds

Input:
	Clock *t = clock set at the beginning

Output:
	time passed in milliseconds

*/
millis clock_delta(Clock* t);

/*

Store current time

Input:
	UC time[timSZ] = time storage

*/
void clock_store(UC time[timSZ]);
#endif

#endif