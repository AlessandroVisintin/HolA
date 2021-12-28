#include "task.h"
#include "init/init.h"
#include "certification/certification.h"
#include "join/join.h"
#include "stabilize/stabilize.h"
#include "rectify/rectify.h"
#include "attest/attest.h"
#include "../device/device.h"
#include "../cryptography/aes.h"
#include "../cryptography/dh.h"

static volatile bool init_done = false;
static volatile bool certification_done = false;

static int ovlCO;
static signed char ovlCA[oveNU];
static int tskCO[oveNU];
static signed char tskCA[oveNU][3];

static volatile bool rectify_priority[oveNU];

static void task_init(void)
{
	join_init();
	stabilize_init();
	rectify_init();
	attest_init();

	ovlCO = 0;
	for (int i = 0; i < oveNU; i++)
	{
		rectify_priority[i] = true;
		ovlCA[i] = -1;
		tskCO[i] = 0;
		for (int j = 0; j < 3; j++)
			tskCA[i][j] = -1;
	}
}

static UC next_ovl(void)
{
	if (ovlCO >= oveNU)
	{
		ovlCO = 0;
		for (int i = 0; i < oveNU; i++)
			ovlCA[i] = -1;
	}

	UC rand;
	gen_random(&rand, 1);
	rand = rand % oveNU;

	while (ovlCA[rand] != -1)
		rand = (rand+1) % oveNU;

	ovlCO += 1;
	ovlCA[rand] = 1;

	return rand;
}

static UC next_tsk(void)
{
	if (tskCO[curOV] >= 3)
	{
		tskCO[curOV] = 0;
		for (int i = 0; i < 3; i++)
			tskCA[curOV][i] = -1;
	}

	if (!attest_isDone() && attest_checkLost())
	{
	  	if (tskCA[curOV][2] == -1)
	  	{
	  		tskCO[curOV] += 1;
	  		tskCA[curOV][2] = 1;
	  	}
	  	return 2;
	}

	if (rectify_checkNew() && rectify_priority[curOV])
	{
		rectify_priority[curOV] = false;
		if (tskCA[curOV][1] == -1)
		{
			tskCO[curOV] += 1;
			tskCA[curOV][1] = 1;
		}
		return 1;
	}
	rectify_priority[curOV] = true;

	UC rand;
	gen_random(&rand, 1);
	rand = rand % 3;

	while (tskCA[curOV][rand] != -1)
		rand = (rand+1) % 3;

	tskCO[curOV] += 1;
	tskCA[curOV][rand] = 1;

	return rand;
}

void outgoing(void)
{
	bu_nulBuff(curBU);

	if (!init_done)
		return;

	if (!certification_done)
	{
		ce_create();
	 	return;
	}

	curOV = next_ovl();
	memset((UC*) (&(sndBU[0])), 0, maiSZ);

	if (!join_done())
		(*jo_outgoing[curOV])();

	else
	{
	 	UC next;
	 	for (int i = 0; i < 3; i++)
	 	{
	 		next = next_tsk();

	 		if (next == 0)
	 			(*st_outgoing[curOV])();

	 		else if (next == 1)
	 			(*re_outgoing[curOV])();

	 		else if (next == 2)
	 			(*at_outgoing[curOV])();

	 		if (!memisnull(bu_getAddr(curBU), ipaSZ))
	 			break;
	 	}
	}

	if (!memisnull(bu_getAddr(curBU), ipaSZ))
	{
		ro_setMoni(curDA, 1);
		ro_setCopy(curDA, sndBU);
		clock_store(ro_getEncr(curDA));

		if (cryptFLAG == 0)
			memcpy(curDA, (UC*) (&(sndBU[0])), maiSZ);
		else
		{
			UC key[dhMO];
			dh_generateKey(ro_getReci(sndBU)+ipaSZ, de_getSecret(), key);
			gen_random(ro_getVect(curDA), aesBK);

			aes_encryptCheck(
				ro_getReci(sndBU)+ipaSZ, ro_getVect(curDA), curDA);
			aes_encryptHeader(
				sndBU, ro_getReci(sndBU)+ipaSZ, ro_getVect(curDA), curDA);
			aes_encryptBody(
				sndBU, key, ro_getVect(curDA), curDA);
		}

		clock_store(ro_getEndt(curDA));
	}
}

void incoming(void)
{
	UI len = deserialize(bu_getLeng(curBU), true);

	if (len == parSZ)
	{
	 	if (init_done)
	 	{
	 		printf("incoming: init already done\n");
	 		bu_nulBuff(curBU);
	 	}
		else
		{
			task_init();
			in_device();
			init_done = true;
		}
		return;
	}

	if (len == cerSZ)
	{
		curOV = 0;
		if (!memcmp(de_getEntry(), de_getPlate(), ipaSZ) == 0)
		{
			printf("incoming: device cannot certify\n");
			bu_nulBuff(curBU);
		}
		else
			ce_verify();
		return;
	}

	if (len == admSZ)
	{
		if (certification_done)
			printf("incoming: certification already done\n");
		else
		{
			ce_save();
			certification_done = true;
		}

		bu_nulBuff(curBU);
		return;
	}

	if (len == maiSZ)
	{
		UC starTI[timSZ], decrTI[timSZ];

		clock_store(starTI);

		memset((UC*) (&(rcvBU[0])), 0, maiSZ);
		memset((UC*) (&(sndBU[0])), 0, maiSZ);

		// decrypt
		if (cryptFLAG == 0)
			memcpy((UC*) (&(rcvBU[0])), curDA, maiSZ);
		else
		{
			for (int i = 0; i < oveNU; i++)
			{
				curOV = i;
				if (aes_decryptCheck(curDA, de_getPlate()+ipaSZ, ro_getVect(curDA)))
				{
					aes_decryptHeader(
						curDA, de_getPlate()+ipaSZ, ro_getVect(curDA), rcvBU);

					UC key[dhMO];
					dh_generateKey(ro_getSend(rcvBU)+ipaSZ, de_getSecret(), key);

					aes_decryptBody(curDA, key, ro_getVect(curDA), rcvBU);

					break;
				}

				if (i == (oveNU-1))
				{
					bu_nulPred(curBU);
					return;
				}
			}
		}

		clock_store(decrTI);

		curOV = ro_getOver(rcvBU);
		curOP = ro_getOpco(rcvBU);

		if ((curOP - acceptCHAL) >= 0)
			(*at_outgoing[curOV])();

		else if ((curOP - askRAND) >= 0)
			at_incoming();

		else if ((curOP - answOPREV) >= 0)
			(*re_outgoing[curOV])();

		else if ((curOP - askOPREV) >= 0)
			re_incoming();

		else if ((curOP - answPSUCC) >= 0)
			(*st_outgoing[curOV])();

		else if ((curOP - askPSUCC) >= 0)
			st_incoming();

		else if ((curOP - answJOIN) >= 0)
			(*jo_outgoing[curOV])();

		else if ((curOP - askJOIN) >= 0)
			jo_incoming();

		else
			bu_nulPred(curBU);

		if (!memisnull(bu_getAddr(curBU), ipaSZ))
		{
			UC aflag = ro_getFlag(curDA);
			UC adrop = ro_getDrop(curDA);
			UC pcoun = ro_getPcou(curDA);
			UC fcoun = ro_getFcou(curDA);
			bu_nulData(curBU);
			ro_setFlag(curDA, aflag);
			ro_setDrop(curDA, adrop);
			ro_setPcou(curDA, pcoun);
			ro_setFcou(curDA, fcoun);

			ro_setStar(curDA, starTI);
			ro_setDecr(curDA, decrTI); 

			ro_setMoni(curDA, 1);
			ro_setCopy(curDA, sndBU);
			clock_store(ro_getEncr(curDA));

			// encrypt
			if (cryptFLAG == 0)
				memcpy(curDA, (UC*) (&(sndBU[0])), maiSZ);
			else
			{
				UC key[dhMO];
				dh_generateKey(ro_getReci(sndBU)+ipaSZ, de_getSecret(), key);
				gen_random(ro_getVect(curDA), aesBK);

				aes_encryptCheck(
					ro_getReci(sndBU)+ipaSZ, ro_getVect(curDA), curDA);
				aes_encryptHeader(
					sndBU, ro_getReci(sndBU)+ipaSZ, ro_getVect(curDA), curDA);
				aes_encryptBody(
					sndBU, key, ro_getVect(curDA), curDA);
			}

			clock_store(ro_getEndt(curDA));
		}

		return;
	}

	bu_nulBuff(curBU);
}
