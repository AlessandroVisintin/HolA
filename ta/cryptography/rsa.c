#include "rsa.h"
#include "keys.h"
#include <operation/operation.h>
#include <mbedtls/include/entropy.h>
#include <mbedtls/include/ctr_drbg.h>
#include <mbedtls/include/pk.h>
#include <mbedtls/include/rsa.h>

typedef mbedtls_pk_context KEY_CTX;
typedef mbedtls_ctr_drbg_context RAND_CTX;

static void rsa_setRandom(RAND_CTX *r_ctx)
{
		mbedtls_entropy_context e;
		mbedtls_entropy_init(&e);

		mbedtls_ctr_drbg_init(r_ctx);

		const char* phrase = "HiDRA";
		int err = mbedtls_ctr_drbg_seed(
			r_ctx, mbedtls_entropy_func,
			&e, (const UC*) phrase, strlen(phrase)
		);

		if(err != 0)
			printf("rsa_setRandom: failed - 0x%02X\n", -err);

		mbedtls_entropy_free(&e);
}

static void rsa_parsePvtKey(KEY_CTX* pvt, UC key[rsaPV])
{
		mbedtls_pk_init(pvt);

		int err = mbedtls_pk_parse_key(pvt, key, rsaPV, NULL, -1);

		if(err != 0)
			printf( "rsa_parsePvtKey: failed - 0x%02X\n", -err);
}

static void rsa_parsePubKey(KEY_CTX* pub, UC key[rsaPU])
{
	mbedtls_pk_init(pub);

	int err = mbedtls_pk_parse_public_key(pub, key, rsaPU);

	if (err != 0)
		printf("rsa_parsePubKey failed - 0x%02X\n", -err);
}

static void rsa_sign(KEY_CTX* pvt, UC *msg, int len, UC digest[rsaBK])
{
   	UC hash[shaSZ];
   	op_sha256(msg, len, hash);

   	size_t l = rsaBK;
   	int err = mbedtls_pk_sign(
   		pvt, MBEDTLS_MD_SHA256, hash, 0, digest, &l, NULL, NULL);

   	if (err != 0)
   		printf("rsa_sign: failed - 0x%02X\n", -err);
}

static bool rsa_verify(KEY_CTX* pub, UC *msg, int len, UC digest[rsaBK])
{
	UC hash[shaSZ];
	op_sha256(msg, len, hash);

	int err = mbedtls_pk_verify(
		pub, MBEDTLS_MD_SHA256, hash, shaSZ, digest, rsaBK);

	if(err == 0)
		return true;
	else if (err == -0x4380)
		return false;
	else
	{
		printf("rsa_verify: failed - 0x%02X\n", -err);
		return false;
	}
}

void rsa_signKey(UC pub[rsaPU], UC cert[rsaBK])
{
	KEY_CTX pvt;
	rsa_parsePvtKey(&pvt, PVT_CA);
	rsa_sign(&pvt, &pub[0], rsaPU, cert);
	mbedtls_pk_free(&pvt);
}

bool rsa_verifyKey(UC pub[rsaPU], UC cert[rsaBK])
{
	KEY_CTX key;
	rsa_parsePubKey(&key, PUB_CA);
	bool r = rsa_verify(&key, &pub[0], rsaPU, cert);
	mbedtls_pk_free(&key);

	return r;
}

void rsa_encryptPlate(UC key[rsaPU], UC plate[plaSZ], UC crypto[rsaBK])
{
	RAND_CTX rand;
	KEY_CTX pub;

	rsa_setRandom(&rand);
	rsa_parsePubKey(&pub, key);

	size_t l = 0;
	int err = mbedtls_pk_encrypt(
		&pub, &plate[0], plaSZ, &crypto[0], &l, rsaBK,
		mbedtls_ctr_drbg_random, &rand);

	if (err != 0)
		printf("rsa_encryptPlate: failed - 0x%02X\n", -err);

	mbedtls_pk_free(&pub);
	mbedtls_ctr_drbg_free(&rand);
}

void rsa_decryptPlate(UC key[rsaPV], UC crypto[rsaBK], UC plate[plaSZ])
{
	RAND_CTX rand;
	KEY_CTX pvt;

	rsa_setRandom(&rand);
	rsa_parsePvtKey(&pvt, key);

	size_t l = 0;
	int err = mbedtls_pk_decrypt(
		&pvt, &crypto[0], rsaBK, &plate[0], &l, plaSZ,
		mbedtls_ctr_drbg_random, &rand);

	if (err != 0)
		printf("rsa_decryptPlate: failed - 0x%02X\n", -err);

	mbedtls_pk_free(&pvt);
	mbedtls_ctr_drbg_free(&rand);
}
