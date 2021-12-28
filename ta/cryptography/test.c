#include "aes.h"
#include "dh.h"
#include "rsa.h"
#include "keys.h"

int main(int argc, char const *argv[])
{
	printf("Testing AES-encryption with Diffie-Hellman keys...\n");

	printf("Genrating first device secret and ID...\n");
	UC rand1[dhEX], id1[dhMO];
	gen_random(rand1, dhEX);
	dh_generateId(rand1, id1);

	printf("Genrating second device secret and ID...\n");
	UC rand2[dhEX], id2[dhMO];
	gen_random(rand2, dhEX);
	dh_generateId(rand2, id2);

	UC key[dhMO] ;
	printf("Generating init vector...\n");
	UC iv[aesBK];
	gen_random(iv, aesBK);

	printf("Generating first key...\n");
	dh_generateKey(id2, rand1, key);

	printf("Generating plain message...\n");
	UC plain[maiSZ], crypto[maiSZ];
	memset(plain, 10, maiSZ);
	memprint(plain, maiSZ);
	memset(crypto, 0, maiSZ);

	printf("Encrypting CHECK...\n");
	aes_encryptCheck(plain, key, iv, crypto);
	memprint(crypto, maiSZ);

	printf("Encrypting HEAD...\n");
	aes_encryptHeader(plain, key, iv, crypto);
	memprint(crypto, maiSZ);

	printf("Encrypting BODY...\n");	
	aes_encryptBody(plain, key, iv, crypto);
	memprint(crypto, maiSZ);

	printf("Generating second key...\n");
	dh_generateKey(id1, rand2, key);

	memset(plain, 0, maiSZ);

	printf("Decrypting CHECK...\n");
	aes_decryptCheck(crypto, key, iv, plain);
	memprint(plain, maiSZ);

	printf("Decrypting HEADER...\n");
	aes_decryptHeader(crypto, key, iv, plain);
	memprint(plain, maiSZ);

	printf("Decrypting BODY...\n");
	aes_decryptBody(crypto, key, iv, plain);
	memprint(plain, maiSZ);

	printf("Testing RSA sign/verify function...\n");
	UC pub[rsaPU], cert[rsaBK];
	memset(pub, 10, rsaPU);

	printf("Signing key...\n");
	rsa_signKey(pub, cert);

	printf("Verifying key... ");
	if (rsa_verifyKey(pub, cert))
		printf("verified!\n");
	else
		printf("not verified!\n");

	printf("Testing RSA encr/decr function...\n");
	UC plate[plaSZ], crypted[rsaBK];
	memset(plate, 10, plaSZ);

	printf("Printing plate...\n");
	memprint(plate, plaSZ);

	printf("Encrypting plate\n");
	rsa_encryptPlate(PUB_CA, plate, crypted);

	printf("Printing crypted result...\n");
	memprint(crypted, rsaBK);

	printf("Decrypting\n");
	rsa_decryptPlate(PVT_CA, crypted, plate);

	printf("Printing decrypted result...\n");
	memprint(plate, plaSZ);

	printf("Testing certificate list...\n");

	UC *pubKey, *certificate;
	for (int i = 0; i < MAXDEV; i++)
	{
		pubKey = &(RSA_KEYS[i].pub[0]);
		certificate = &(RSA_KEYS[i].cer[0]);
		printf("Key #%d: ", i);

		if (rsa_verifyKey(pubKey, certificate))
			printf("verified!\n");
		else
			printf("not verified!\n");
	}

	return 0;
}