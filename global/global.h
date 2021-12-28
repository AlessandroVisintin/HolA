#ifndef _GLOBAL_H
#define _GLOBAL_H

#include "const.h"
#include "buffer/buffer.h"
#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

/*

Convert unsigned char buffer to unsigned int
	in host-endianness

Input:
	UC *bytes = unsigned char
	bool host = true if convert to host-endianness,
		false for keeping big-endianness

Output:
	conversion result

*/
UI deserialize(UC *bytes, bool host);

/*

Convert unsigned int to unsigned char buffer
	with big endian order

Input:
	UI integer = unsigned int
	UC* bytes = resulting buffer

*/
void serialize(UI integer, UC *bytes);

/*

Check if pointed memory is set to zero 

Input:
	UC *ptr = memory pointer
	int len = length to check

Output:
	true = memory is set to zero
	false = memory is not zero

*/
bool memisnull(UC *ptr, int len);

/*

Print pointed memory 

Input:
	UC *ptr = memory pointer
	int len = length to print

*/
void memprint(UC *ptr, int len);

#if (ENV==1 || ENV==2)
/*

Gateway function for non-OPTEE environment

Input:
	UC *buf = pointer to buffer to use
	int len = length of the buffer
	UC *bin = pointer to function to attest
	int blen = length of attesting binary
	UI code = operation code (TA_HIDRA_OUTGOING / TA_HIDRA_INCOMING)

Output:
	SUCCESS = buffer processed and ready
*/
errCO TEEC_InvokeCommand(UC *buf, int len, UC *bin, int blen, UI code);
#endif


#endif