#include "aes.h"

static const char *check = "HiDRA protocol";

static void aes_encrypt(
	UC *plain, int len, UC *crypto, UC key[aesSZ], UC ivr[aesBK])
{
	mbedtls_aes_context in;
	
	UC ivc[aesBK];
	memcpy(ivc, ivr, aesBK);

	int err = mbedtls_aes_setkey_enc(&in, key, aesBI);
	if(err != 0)
	{
		printf( "aes_encrypt: setkey_enc failed - 0x%02X\n", -err);
 		return;
  	}

  	err = mbedtls_aes_crypt_cbc(&in, MBEDTLS_AES_ENCRYPT, len, 
  									ivc, plain, crypto);

  	mbedtls_aes_free(&in);

  	if(err != 0)
	{
		printf( "aes_encrypt: crypt failed - 0x%02X\n", -err);
  		return;
 	}


}

static void aes_decrypt(
	UC* crypto, int len, UC* plain, UC key[aesSZ], UC ivr[aesBK])
{
	mbedtls_aes_context out;

	UC ivc[aesBK];
	memcpy(ivc, ivr, aesBK);

	int err = mbedtls_aes_setkey_dec(&out, key, aesBI);
	if(err != 0)
	{
		printf( "aes_decrypt: setkey_dec failed - 0x%02X\n", -err);
		return;
	}
	
	err = mbedtls_aes_crypt_cbc(&out, MBEDTLS_AES_DECRYPT, len, 
									ivc, crypto, plain);

	mbedtls_aes_free(&out);

	if(err != 0)
	{
		printf( "aes_decrypt: failed - 0x%02X\n", -err);
 		return;
 	}
}

void aes_encryptCheck(
	UC key[aesSZ], UC ivr[aesBK], UC crypto[maiSZ])
{
 	aes_encrypt((UC*) check, aesBK, ro_getChec(crypto), key, ivr);	
}

void aes_encryptHeader(
	UC plain[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC crypto[maiSZ])
{
	aes_encrypt(
		(UC*) (&(plain[0])), ro_heaSZ, (UC*) (&(crypto[0])), key, ivr);
}

void aes_encryptBody(
	UC plain[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC crypto[maiSZ])
{
	aes_encrypt(
		ro_getPlat(plain,0), ro_bodSZ, ro_getPlat(crypto,0), key, ivr);
}

bool aes_decryptCheck(
  	UC crypto[maiSZ], UC key[aesSZ], UC ivr[aesBK])
{
	UC ck[aesBK];

	aes_decrypt(
		ro_getChec(crypto), aesBK, (UC*) (&(ck[0])), key, ivr);

	if (strncmp(check, (char*) (&(ck[0])), aesBK) == 0)
		return true;
	return false;
}

void aes_decryptHeader(
  	UC crypto[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC plain[maiSZ])
{
  	aes_decrypt(
  		(UC*) (&(crypto[0])), ro_heaSZ, (UC*) (&(plain[0])), key, ivr);
}

void aes_decryptBody(
  	UC crypto[maiSZ], UC key[aesSZ], UC ivr[aesBK], UC plain[maiSZ])
{
 	aes_decrypt(
 		ro_getPlat(crypto,0), ro_bodSZ, ro_getPlat(plain,0), key, ivr);
}