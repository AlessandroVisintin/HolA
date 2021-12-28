#ifndef _DH_H
#define _DH_H

#include "../ta.h"

/*

Generate public ID from secret number

Input:
	UC rand[dhEX] = random number
	UC result[dhMO] = result ID

*/
void dh_generateId(UC rand[dhEX], UC result[dhMO]);

/*

Generate AES-key from public ID and secret number 

Input:
	UC id[dhMO] = recipient public ID
	UC rand[dhEX] = sender secret number
	UC result[dhMO] = resulting AES-key
*/
void dh_generateKey(UC id[dhMO], UC rand[dhEX], UC result[dhMO]);


#endif