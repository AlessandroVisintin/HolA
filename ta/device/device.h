#ifndef _DEVICE_H
#define _DEVICE_H

#include "../ta.h"

// Mac
UC* de_getMac(void);
void de_setMac(UC mac[macSZ]);

// Unique ID
UC* de_getUid(void);
void de_setUid(UC uid[dhMO]);

// Private key
UC* de_getPvt(void);
void de_setPvt(UC pvt[rsaPV]);

// Public key
UC* de_getPub(void);
void de_setPub(UC pub[rsaPU]);

// Certificate
UC* de_getCer(void);
void de_setCer(UC cer[rsaBK]);

// Node plate
UC* de_getPlate(void);
void de_setPlate(UC plate[plaSZ]);

// Node secret
UC* de_getSecret(void);
void de_setSecret(UC secret[dhEX]);

// Node entry
UC* de_getEntry(void);
void de_setEntry(UC entry[plaSZ]);

// Node prev
UC* de_getPrev(void);
void de_setPrev(UC prev[plaSZ]);

// Node ith successor
UC* de_getSucc(int ith);
void de_setSucc(int ith, UC succ[plaSZ]);

// Finger list
int de_flistCount(void);
void de_initFlist(void);
void de_closeFlist(void);
UC* de_getFinger(int ith);
void de_insertFinger(UC finger[plaSZ]);
bool de_findSuccessor(UC id[dhMO], UC result[plaSZ]);

// Proof list
int de_alistCount(void);
void de_initAlist(void);
void de_closeAlist(void);
bool de_insertProof(UC proof[proSZ]);
bool de_findProof(UC uid[dhMO], UC result[proSZ]);
bool de_relocateProof(UC nprev[plaSZ], UC result[proSZ]);
void de_produceProof(UC proof[proSZ]);
void de_purgeAlist(UC nprev[plaSZ]);
void de_dropAlist(void);

// Print methods
void de_printDevice(void);
void de_printNode(void);

#endif