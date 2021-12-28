#include "certification.h"
#include "../../device/device.h"
#include "../../cryptography/rsa.h"

void ce_create(void)
{
	curOV = 0;
	bu_setAddr(curBU, de_getEntry());
	serialize(cerSZ, bu_getLeng(curBU));

	ce_setAppl(curDA, de_getPlate());
	ce_setCert(curDA, de_getCer());
	ce_setPubl(curDA, de_getPub());
}

void ce_verify(void)
{
	UC pub[rsaPU];
	memcpy((UC*) (&(pub[0])), ce_getPubl(curDA), rsaPU);

 	if (rsa_verifyKey(pub, ce_getCert(curDA)))
 	{
 		bu_nulPred(curBU);
 		bu_setAddr(curBU, ce_getAppl(curDA));
		serialize(admSZ, bu_getLeng(curBU));
 		
 		bu_nulData(curBU);
 		for (int i = 0; i < oveNU; i++)
 		{
 			curOV = i;
 			rsa_encryptPlate(
 				pub, de_getPlate(), ad_getRsab(curDA, i));
 		}
 	}
 	else
 		bu_nulPred(curBU);
}

void ce_save(void)
{
 	for (int i = 0; i < oveNU; i++)
 	{
 		curOV = i;
 		rsa_decryptPlate(
 			de_getPvt(), ad_getRsab(curDA, i), de_getEntry());
 	}

}