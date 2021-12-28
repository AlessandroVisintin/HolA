#ifndef _KEYS_H
#define _KEYS_H

#include <const.h>

// Diffie-Hellman
extern UC DH_P[dhMO];
extern UC DH_B[dhBA];

// RSA key pairs - certificates
extern UC PVT_CA[rsaPV];
extern UC PUB_CA[rsaPU];

struct s_rsa{
	UC mac[macSZ];
	UC pvt[rsaPV];
	UC pub[rsaPU];
	UC cer[rsaBK];
};

extern struct s_rsa RSA_KEYS[MAXDEV];

#endif