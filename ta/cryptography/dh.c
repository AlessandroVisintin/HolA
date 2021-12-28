#include "dh.h"
#include "keys.h"
#include <mbedtls/include/bignum.h>

typedef mbedtls_mpi MPI;

static void dh_modExp(
	UC* b, int bl, UC* e, int el, UC* m, int ml, UC* r, int rl)
{
	int err;
	MPI base, exp, mod, res;

	mbedtls_mpi_init(&base);
	mbedtls_mpi_init(&exp);
	mbedtls_mpi_init(&mod);
	mbedtls_mpi_init(&res);

	err = mbedtls_mpi_read_binary(&base, b, bl);
	if (err != 0)
		printf("aes_modExp: base read failed - 0x%02X\n", -err);

	err = mbedtls_mpi_read_binary(&exp, e, el);
	if (err != 0)
		printf("aes_modExp: exp read failed - 0x%02X\n", -err);

	err = mbedtls_mpi_read_binary(&mod, m, ml);
	if (err != 0)
		printf("aes_modExp: mod read failed - 0x%02X\n", -err);

	err = mbedtls_mpi_exp_mod(&res, &base, &exp, &mod, NULL); 
	if (err != 0)
		printf("aes_modExp: op failed - 0x%02X\n", -err);
	
	err = mbedtls_mpi_write_binary(&res, r, rl);
	if (err != 0)
		printf("aes_modExp: write failed - 0x%02X\n", -err);

	mbedtls_mpi_free(&base);
	mbedtls_mpi_free(&exp);
	mbedtls_mpi_free(&mod);
	mbedtls_mpi_free(&res);
}

void dh_generateId(UC rand[dhEX], UC result[dhMO])
{
	dh_modExp(
		&DH_B[0], dhBA, &rand[0], dhEX, 
		&DH_P[0], dhMO, &result[0], dhMO);
}

void dh_generateKey(UC id[dhMO], UC rand[dhEX], UC result[dhMO])
{
	dh_modExp(
		&id[0], dhMO, &rand[0], dhEX, 
		&DH_P[0], dhMO, &result[0], dhMO);
}