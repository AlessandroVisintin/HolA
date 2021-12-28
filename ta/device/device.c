#include "device.h"
#include "flist.h"
#include "alist.h"
#include "../cryptography/rsa.h"
#include <operation/operation.h>

typedef struct s_node {
	UC secret[dhEX];
	UC plate[plaSZ];
	UC entry[plaSZ];
	UC prev[plaSZ];
	UC slist[sucNU][plaSZ];
	Flist *fhead;
	Alist *ahead;
} Node;

typedef struct s_dev {
	UC mac[macSZ];
	UC uid[dhMO];
	UC pvt[rsaPV];
	UC pub[rsaPU];
	UC cer[rsaBK];
	Node ovl[oveNU];
} Device;

// Device info
Device d;

// Mac
UC* de_getMac(void) 
{ return (UC*) (&(d.mac[0])); }

void de_setMac(UC mac[macSZ]) 
{ memcpy(de_getMac(), (UC*) (&(mac[0])), macSZ); }

// Unique ID
UC* de_getUid(void)
{ return (UC*) (&(d.uid[0])); }

void de_setUid(UC uid[dhMO])
{ memcpy(de_getUid(), (UC*) (&(uid[0])), dhMO); }

// Private key
UC* de_getPvt(void)
{ return (UC*) (&(d.pvt[0])); }

void de_setPvt(UC pvt[rsaPV])
{ memcpy(de_getPvt(), (UC*) (&(pvt[0])), rsaPV); }

// Public key
UC* de_getPub(void)
{ return (UC*) (&(d.pub[0])); }

void de_setPub(UC pub[rsaPU])
{ memcpy(de_getPub(), (UC*) (&(pub[0])), rsaPU); }

// Certificate
UC* de_getCer(void)
{ return (UC*) (&(d.cer[0])); }

void de_setCer(UC cer[rsaBK])
{ memcpy(de_getCer(), (UC*) (&(cer[0])), rsaBK); }

// Node secret
UC* de_getSecret(void)
{ return (UC*) (&(d.ovl[curOV].secret[0])); }

void de_setSecret(UC secret[dhEX])
{ memcpy(de_getSecret(), (UC*) (&(secret[0])), dhEX); }

// Node plate
UC* de_getPlate(void) 
{ return (UC*) (&(d.ovl[curOV].plate[0])); }

void de_setPlate(UC plate[plaSZ]) 
{ memcpy(de_getPlate(), (UC*) (&(plate[0])), plaSZ); }

// Node entry
UC* de_getEntry(void)
{ return (UC*) (&(d.ovl[curOV].entry[0])); }

void de_setEntry(UC entry[plaSZ]) 
{ memcpy(de_getEntry(), (UC*) (&(entry[0])), plaSZ); }

// Node prev
UC* de_getPrev(void) 
{ return (UC*) (&(d.ovl[curOV].prev[0])); }

void de_setPrev(UC prev[plaSZ]) 
{ memcpy(de_getPrev(), (UC*) (&(prev[0])), plaSZ); }

// Node ith successor
UC* de_getSucc(int ith) 
{ return (UC*) (&(d.ovl[curOV].slist[ith][0])); }

void de_setSucc(int ith, UC succ[plaSZ]) 
{ memcpy(de_getSucc(ith), (UC*) (&(succ[0])), plaSZ); }

// Finger list
int de_flistCount(void)
{ return flist_count(d.ovl[curOV].fhead); }

void de_initFlist(void)
{
	if (!d.ovl[curOV].fhead) 
		d.ovl[curOV].fhead = flist_create(de_getPlate()); 
}

void de_closeFlist(void)
{
	if (d.ovl[curOV].fhead)
		flist_destroy(d.ovl[curOV].fhead); 
}

UC* de_getFinger(int ith)
{ return flist_getPlateByIndex(d.ovl[curOV].fhead, ith); }

void de_insertFinger(UC finger[plaSZ])
{	
	if (!memisnull( (UC*) (&(finger[0])), plaSZ)) 
		flist_insert(d.ovl[curOV].fhead, finger); 
}

bool de_findSuccessor(UC id[dhMO], UC result[plaSZ])
{
 	if (
 		!memisnull(de_getSucc(0), plaSZ) &&
 		op_between(de_getPlate()+ipaSZ, id, de_getSucc(0)+ipaSZ)
 	)
 	{
 		memcpy((UC*) (&(result[0])), de_getSucc(0), plaSZ);
 		return true;
 	}
 	
  	if (!flist_closestPredecessor(d.ovl[curOV].fhead, id, result))
  	{
  		memcpy((UC*) (&(result[0])), de_getSucc(0), plaSZ);
  	}

  	return false;	
}

// Proof list
int de_alistCount(void)
{ return alist_count(d.ovl[curOV].ahead); }

void de_initAlist(void)
{
	if (!d.ovl[curOV].ahead) 
		d.ovl[curOV].ahead = alist_create(de_getPlate()); 
}

void de_closeAlist(void)
{
	if (d.ovl[curOV].ahead)
		alist_destroy(d.ovl[curOV].ahead); 
}

bool de_insertProof(UC proof[proSZ])
{
 	if (memisnull((UC*) (&(proof[0])), proSZ))
 		return false;

 	alist_insert(d.ovl[curOV].ahead, proof);
 	return true;
}

bool de_findProof(UC uid[dhMO], UC result[proSZ])
{
 	return alist_findProof(
 		d.ovl[curOV].ahead, uid, result); 
}

bool de_relocateProof(UC nprev[plaSZ], UC result[proSZ])
{
	return alist_relocateProof(
		d.ovl[curOV].ahead,	(UC*) (&(nprev[ipaSZ])), result);
}

void de_produceProof(UC proof[proSZ])
{
	if (!proof)
 		return;

 	UC* p = (UC*) (&(proof[0]));
 	
 	memcpy(p, de_getUid(), dhMO);
	
	int half = (curLE / 2);
	op_sha256(curPO, half, p + dhMO);
 	op_sha256(curPO + half, curLE - half, p + dhMO + shaSZ);
}

void de_purgeAlist(UC nprev[plaSZ])
{ alist_purge( d.ovl[curOV].ahead, (UC*) (&(nprev[ipaSZ]))); }

void de_dropAlist(void)
{
	for (int i = 0; i < oveNU; i++)
		alist_dropAlist(d.ovl[i].ahead); 
}

void de_printDevice(void)
{
	printf("Mac: ");
	memprint(de_getMac(), macSZ);
	printf("Uid:\n");
	memprint(de_getUid(), dhMO);
 	printf("Pvt key: (...) ");
	memprint(de_getPvt(), 10);
	printf("Pub key: (...) ");
	memprint(de_getPub(), 10);
	printf("Cert: (...) ");
	memprint(de_getCer(), 10);
}

void de_printNode(void)
{
	printf("Ovl: %d\n", curOV);
 	printf("Plate: ");
 	memprint(de_getPlate(), ipaSZ);
 	memprint(de_getPlate() + ipaSZ, dhMO);
 	printf("Prev: ");
 	memprint(de_getPrev(), ipaSZ);
 	memprint(de_getPrev() + ipaSZ, dhMO);
 	
 	for (UI i = 0; i < sucNU; i++)
 	{
 		printf("Succ #%d: ", i);
 		memprint(de_getSucc(i), ipaSZ);
 		memprint(de_getSucc(i) + ipaSZ, dhMO);
 	}

 	flist_print(d.ovl[curOV].fhead);
 	alist_print(d.ovl[curOV].ahead);
}