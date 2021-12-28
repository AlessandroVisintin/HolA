#ifndef _RSA_H
#define _RSA_H

#include "../ta.h"

/*

Sign public key with CA private key

Input:
	UC pub[rsaPU] = key to sign
	UC cert[rsaBK] = resulting certificate

*/
void rsa_signKey(UC pub[rsaPU], UC cert[rsaBK]);

/*

Verify signature with CA public key

Input:
	UC pub[rsaPU] = key to be verified
	UC cert[rsaBK] = CA certificate

Output:
	true = key certified
	false = key not certified

*/
bool rsa_verifyKey(UC pub[rsaPU], UC cert[rsaBK]);

/*

Encrypt plate with public key 

Input:
	UC key[rsaPU] = RSA public key
	UC plate[plaSZ] = HiDRA plate
	UC crypto[rsaBK] = crypted result

*/
void rsa_encryptPlate(
	UC key[rsaPU], UC plate[plaSZ], UC crypto[rsaBK]);

/*

Decrypt HiDRA plate with private key 

Input:
	UC key[rsaPV] = RSA private key
	UC crypto[rsaBK] = crypted HiDRA plate
	UC plate[plaSZ] = plain HiDRA plate

*/
void rsa_decryptPlate(
	UC key[rsaPV], UC crypto[rsaBK], UC plate[plaSZ]);

#endif