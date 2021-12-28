#ifndef _AES_H
#define _AES_H

#include "../ta.h"
#include <mbedtls/include/aes.h>

/*

Encrypt check phrase

Input:
	UC key[aesSZ] = encryption key
	UC ivr[aesBK] = inizialization vector
	UC crypto[maiSZ] = crypted result 

 The check phrase will be used for decryption check

*/
void aes_encryptCheck(UC key[aesSZ], UC ivr[aesBK], UC crypto[maiSZ]);

/*

Encrypt header

Input:
	UC plain[maiSZ] = plain message
	UC key[aesSZ] = encryption key
	UC ivr[aesBK] = initialization vector
	UC crypto[maiSZ] = crypted result

*/
void aes_encryptHeader(
	UC plain[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC crypto[maiSZ]);

/*

Encrypt body

Input:
	UC plain[maiSZ] = plain message
	UC key[aesSZ] = encryption key
	UC ivr[aesBK] = initialization vector
	UC crypto[maiSZ] = crypted result

*/
void aes_encryptBody(
	UC plain[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC crypto[maiSZ]);

/*

Decrypt check phrase

Input:
	UC crypto[maiSZ] = crypted message
	UC key[aesSZ] = decryption key
	UC ivr[aesBK] = initialization vector

Output:
	true = check decrypted successfully
	false = check not decrypted

*/
bool aes_decryptCheck(UC crypto[maiSZ], UC key[aesSZ], UC ivr[aesBK]);

/*

Decrypt header

Input:
	UC crypto[maiSZ] = crypted message
	UC key[aesSZ] = decryption key
	UC ivr[aesBK] = initialization vector
	UC plain[maiSZ] = plain result

*/
void aes_decryptHeader(
  	UC crypto[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC plain[maiSZ]);

/*

Decrypt body

Input:
	UC crypto[maiSZ] = crypted message
	UC key[aesSZ] = decryption key
	UC ivr[aesBK] = initialization vector
	UC plain[maiSZ] = plain result

*/
void aes_decryptBody(
  	UC crypto[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC plain[maiSZ]);

#endif