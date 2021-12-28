#include "operation.h"
#include "../mbedtls/include/bignum.h"
#include "../mbedtls/include/sha256.h"

typedef mbedtls_mpi MPI;

void op_sha256(UC *msg, int len, UC result[shaSZ])
{
	mbedtls_sha256_context c;
	mbedtls_sha256_init(&c);
	
	mbedtls_sha256_starts(&c, 0);
	mbedtls_sha256_update(&c, msg, len);
	mbedtls_sha256_finish(&c, result);
	
	mbedtls_sha256_free(&c);
}

bool op_between(UC a[dhMO], UC b[dhMO], UC c[dhMO])
{
	if (memcmp(a, c, dhMO) < 0)
		return (
			(memcmp(a, b, dhMO) < 0) && 
			(memcmp(b, c, dhMO) < 0)
		);
	else
		return (
			(memcmp(a, b, dhMO) < 0) || 
			(memcmp(b, c, dhMO) < 0)
		);
}

void op_add(UC f[dhMO], UC s[dhMO], UC r[dhMO])
{
	int err;
	MPI first, second, mod, result;

	mbedtls_mpi_init(&first);
	mbedtls_mpi_init(&second);
	mbedtls_mpi_init(&mod);
	mbedtls_mpi_init(&result);

	err = mbedtls_mpi_read_binary(&first, f, dhMO);
	if (err != 0)
		printf("op_add failed read first - 0x%02X\n", -err);

	err = mbedtls_mpi_read_binary(&second, s, dhMO);
	if (err != 0)
		printf("op_add failed read second - 0x%02X\n", -err);

	err = mbedtls_mpi_grow(&mod, dhMO+1);
	if (err != 0)
		printf("op_add failed grow mod - 0x%02X\n", -err);	

	err = mbedtls_mpi_set_bit(&mod, (8*dhMO), 1);
	if (err != 0)
		printf("op_add failed set mod - 0x%02X\n", -err);

	err = mbedtls_mpi_add_mpi(&result, &first, &second); 
	if (err != 0)
		printf("op_add failed add - 0x%02X\n", -err);
	
	err = mbedtls_mpi_mod_mpi(&result, &result, &mod); 
	if (err != 0)
		printf("op_add failed mod - 0x%02X\n", -err);

	err = mbedtls_mpi_write_binary(&result, r, dhMO);
	if (err != 0)
		printf("op_add failed write - 0x%02X\n", -err);

	mbedtls_mpi_free(&first);
	mbedtls_mpi_free(&second);
	mbedtls_mpi_free(&mod);
	mbedtls_mpi_free(&result);
}

void op_subtract(UC m[dhMO], UC s[dhMO], UC r[dhMO])
{
 	int err;
 	MPI minuend, subtracting, mod, result;

 	mbedtls_mpi_init(&minuend);
 	mbedtls_mpi_init(&subtracting);
 	mbedtls_mpi_init(&mod);
 	mbedtls_mpi_init(&result);

 	err = mbedtls_mpi_read_binary(&minuend, m, dhMO);
 	if (err != 0)
 		printf("op_subtract failed read minuend - 0x%02X\n", -err);

 	err = mbedtls_mpi_read_binary(&subtracting, s, dhMO);
 	if (err != 0)
 		printf("op_subtract failed read subtracting - 0x%02X\n", -err);

 	err = mbedtls_mpi_grow(&mod, dhMO+1);
 	if (err != 0)
 		printf("op_subtract failed grow mod - 0x%02X\n", -err);

	err = mbedtls_mpi_set_bit(&mod, (8*dhMO), 1);
	if (err != 0)
 		printf("op_subtract failed set mod - 0x%02X\n", -err);

 	if (mbedtls_mpi_cmp_mpi(&minuend, &subtracting) < 0)
 	{
 		err = mbedtls_mpi_add_mpi(&minuend, &minuend, &mod);
 		if (err != 0)
 			printf("op_subtract failed add - 0x%02X\n", -err);
 	}

 	err = mbedtls_mpi_sub_mpi(&result, &minuend, &subtracting); 
 	if (err != 0)
 		printf("op_subtract failed sub - 0x%02X\n", -err);

 	err = mbedtls_mpi_write_binary(&result, r, dhMO);
 	if (err != 0)
 		printf("op_subtract failed write - 0x%02X\n", -err);

 	mbedtls_mpi_free(&minuend);
 	mbedtls_mpi_free(&subtracting);
 	mbedtls_mpi_free(&mod);
 	mbedtls_mpi_free(&result);
}

void op_exp2(UC p[finSZ], UC r[dhMO])
{
	int err;
 	MPI pow, two, mod, result;

 	mbedtls_mpi_init(&two);
 	mbedtls_mpi_init(&pow);
 	mbedtls_mpi_init(&mod);
 	mbedtls_mpi_init(&result);

 	err = mbedtls_mpi_read_string(&two, 10, "2");
 	if (err != 0)
 		printf("op_exp2 failed string - 0x%02X\n", -err);

 	err = mbedtls_mpi_read_binary(&pow, p, finSZ);
 	if (err != 0)
 		printf("op_exp2 failed read - 0x%02X\n", -err);

 	if (mbedtls_mpi_cmp_int(&pow, (dhMO*8)) >= 0)
 	{
 		printf("op_exp2: power too big\n");
 		return;
 	}

 	err = mbedtls_mpi_grow(&mod, dhMO+1);
 	if (err != 0)
 		printf("op_exp2 failed mod grow - 0x%02X\n", -err);

	err = mbedtls_mpi_set_bit(&mod, 0, 1);
	if (err != 0)
 		printf("op_exp2 failed set mod first - 0x%02X\n", -err);

	err = mbedtls_mpi_set_bit(&mod, (8*dhMO), 1);
	if (err != 0)
 		printf("op_exp2 failed set mod last - 0x%02X\n", -err);

 	err = mbedtls_mpi_grow(&result, dhMO);
 	if (err != 0)
 		printf("op_exp2 failed result grow - 0x%02X\n", -err);

 	err = mbedtls_mpi_exp_mod(&result, &two, &pow, &mod, NULL);
 	if (err != 0)
 		printf("op_exp2 failed exp - 0x%02X\n", -err);

 	err = mbedtls_mpi_write_binary(&result, r, dhMO);
 	if (err != 0)
 		printf("op_exp2 failed write - 0x%02X\n", -err);

 	mbedtls_mpi_free(&pow);
 	mbedtls_mpi_free(&two);
 	mbedtls_mpi_free(&mod);
 	mbedtls_mpi_free(&result);
}

void op_log2(UC n[dhMO], UC r[finSZ])
{
 	int err;
 	MPI number, result;

 	mbedtls_mpi_init(&number);
 	mbedtls_mpi_init(&result);
 	
 	err = mbedtls_mpi_read_binary(&number, n, dhMO);
 	if (err != 0)
 		printf("op_log2 failed number read - 0x%02X\n", -err);

 	size_t i = mbedtls_mpi_bitlen(&number);
 	if (i > 0)
 		i -= 1;

 	err = mbedtls_mpi_lset(&result, i);
 	if (err != 0)
 		printf("op_log2 failed result set - 0x%02X\n", -err);
	
 	err = mbedtls_mpi_write_binary(&result, r, finSZ);
 	if (err != 0)
 		printf("op_log2 failed write - 0x%02X\n", -err);

 	mbedtls_mpi_free(&number);
 	mbedtls_mpi_free(&result);
}