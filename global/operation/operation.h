#ifndef _OPERATION_H
#define _OPERATION_H

#include "../global.h"

/*

Create sha256 hash

Input:
	UC *msg = message to hash
	int len = message length
	UC result[shaSZ] = hash result 

*/
void op_sha256(UC *msg, int len, UC result[shaSZ]);

/*

Calculate if b is between a and b

Input:
	UC a[dhMO] = smaller ID
	UC b[dhMO] = checking ID
	UC c[dhMO] = greater ID

Output:
	true = ID B is between A and B
	false = ID B is not between A and B		

*/
bool op_between(UC a[dhMO], UC b[dhMO], UC c[dhMO]);

/*

Calculate IDs modular addition

Input:
	UC f[dhMO] = first addend
	UC s[dhMO] = second addend
	UC r[dhMO] = addition result

*/
void op_add(UC f[dhMO], UC s[dhMO], UC r[dhMO]);

/*

Calculate subtraction between two hashes

Input:
	UC m[dhMO] = minuend
	UC s[dhMO] = subtracting
	UC r[dhMO] = subtraction result		

*/
void op_subtract(UC m[dhMO], UC s[dhMO], UC r[dhMO]);

/*

Calculate the number corresponding to 
	the power of two specified

Input:
	UC p[fpSZ] = power of two
	UC r[dhMO] = resulting number	

*/
void op_exp2(UC p[finSZ], UC r[dhMO]);

/*

Calculate the closest power of two less than n

Input:
	UC n[dhMO] = input ID
	UC r[finSZ] = resulting power of two

*/
void op_log2(UC n[dhMO], UC r[finSZ]);

#endif