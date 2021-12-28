#ifndef _CONST_H
#define _CONST_H

#include <string.h>

typedef unsigned char UC;
typedef unsigned int UI;

// diffie-hellman
#define dhBA 1 // base
#define dhEX 32 // exp
#define dhMO 64 // mod

// RSA encryption
#define rsaBK 256 // block
#define rsaPV 1218 // private key der
#define rsaPU 294 // public key der

// AES encryption
#define aesBK 16 // block
#define aesSZ 32 // key
#define aesBI (aesSZ * 8) // key length bit

// SHA256
#define shaSZ 32 // length

// Protocol
#define TA_HIDRA_UUID \
		{ 0xbcd075c3, 0x3ea6, 0x4e63, \
		{ 0x91, 0x77, 0xc5, 0x08, 0x71, 0x2f, 0xcb, 0x14} }
#define TA_HIDRA_OUTGOING 1
#define TA_HIDRA_INCOMING 2

// #define RANGE "192.168.42" // protocol IP range
// #define ENTRY "192.168.42.21" // protocol entry IP
// #define RANGE "172.20.0" // protocol IP range
// #define ENTRY "172.20.0.2" // protocol entry IP
#define RANGE "192.168.1" // protocol IP range
#define ENTRY "192.168.1.5" // protocol entry IP
#define MAXDEV 200 // max number of device supported

#define cryptFLAG 0 // [0 = false, 1 = true]
#define changeRATE 33 // [0-100]
#define dropRATE 0 // [0-100]

#define ipaSZ 4 // IP address length
#define macSZ 6 // MAC address length
#define plaSZ (ipaSZ + dhMO) // Plate of device
#define proSZ (dhMO + dhMO) // Proof for attesting
#define secSZ 8 // Second representation length
#define milSZ 8 // Milliseconds representation length
#define timSZ (secSZ + milSZ) // Time representation length
#define finSZ 4 // finger position length

#define oveNU 1 // Overlays number
#define sucNU 2 // Successors number

#define MINSLEEP (4000 / oveNU) // min sleep time
#define MAXSLEEP (5000 / oveNU) // max sleep time

#define conTI 750 // connection timeout millis
#define excTI 750 // exchange timeout millis
#define totTI (conTI + excTI)
#define maxTI (10 * totTI)

// Error codes
typedef unsigned int errCO;

#define SUCCESS 0
#define BAD_PARAMETERS -1
#define OUT_BOUND -2
#define EMPTY_LIST -3
#define NOT_FOUND -4
#define NOT_CONNECTING -5
#define NOT_RESPONDING -6

#if ((dhMO / shaSZ) != 2)
	#error IDs must be exactly twice of SHA256 length \
		(Device UID and ATTESTATION are comprised of two hashes each)
#endif

#endif
