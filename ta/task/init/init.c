#include "init.h"
#include "../../device/device.h"
#include "../../cryptography/keys.h"
#include "../../cryptography/dh.h"
#include "../../cryptography/rsa.h"
#include <operation/operation.h>


void in_device(void)
{
	UC *ep = pa_getEntr(curDA);
	UC *ip = pa_getIpad(curDA);
	UC *mac = pa_getMaca(curDA);

	UC uid[dhMO], plate[plaSZ];

	de_setMac(mac);

	memcpy((UC*) (&(curMAC[0])), mac, macSZ);

	op_sha256(ip, ipaSZ, (UC*) (&(uid[0])));
	op_sha256(mac, macSZ, (UC*) (&(uid[shaSZ])));
	de_setUid(uid);

	if (memisnull(ep, ipaSZ))
	{
		for (int i = 0; i < oveNU; i++)
			membership[i] = true;
	}
	else
	{
		for (int i = 0; i < oveNU; i++)
			membership[i] = false;
	}

	#if (ENV == 0)
		for (int i = 0; i < MAXDEV; i++)
		{
			if (memcmp(mac, RSA_KEYS[i].mac, macSZ) == 0)
			{
				de_setPvt(RSA_KEYS[i].pvt);
				de_setPub(RSA_KEYS[i].pub);
				de_setCer(RSA_KEYS[i].cer);
				break;
			}

			if (i == (MAXDEV-1))
			{
				printf("in_device: MAC not found\n");
				return;
			}
		}
	#else
		de_setPvt(RSA_KEYS[0].pvt);
		de_setPub(RSA_KEYS[0].pub);
		de_setCer(RSA_KEYS[0].cer);
	#endif

	for (int i = 0; i < oveNU; i++)
	{
		curOV = i;

		memcpy((UC*) (&(plate[0])), ip, ipaSZ);
		gen_random(de_getSecret(), dhEX);
		dh_generateId(de_getSecret(), (UC*) (&(plate[ipaSZ])));
		de_setPlate(plate);

		de_initFlist();
		de_initAlist();
		
		if (memisnull(ep, ipaSZ))
		{
			de_setPrev(plate);
			de_setSucc(0, plate);
			de_setEntry(plate);
		}
		else
		{
			memset(de_getPrev(), 0, plaSZ);
			memset(de_getSucc(0), 0, plaSZ);
			memset((UC*) (&(plate[0])), 0, plaSZ);
			memcpy((UC*) (&(plate[0])), ep, ipaSZ);
			de_setEntry(plate);
		}
	}
}