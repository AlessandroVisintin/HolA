#include "stabilize.h"
#include "../../device/device.h"
#include <operation/operation.h>

static UC process[oveNU][4];
static volatile bool st_started[oveNU];
static Clock st_timeout[oveNU];

static void ask_psucc(void);
static void ask_nsucc(void);
static void check_nsucc(void);
static void send_note(void);

void stabilize_init(void)
{
	for (int i = 0; i < oveNU; i++)
	{
		st_started[i] = false;
		st_outgoing[i] = ask_psucc;
	}	
}

static void ask_psucc(void)
{
	if (
		st_started[curOV] || 
		curCO != TA_HIDRA_OUTGOING ||
		memisnull(de_getSucc(0), plaSZ)
	)
	{
		st_started[curOV] = false;
		st_outgoing[curOV] = ask_psucc;
		bu_nulPred(curBU);
		return;
	}

	st_started[curOV] = true;
	gen_random((UC*) (&(process[curOV][0])), 4);

	bu_setAddr(curBU, de_getSucc(0));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, askPSUCC);
	ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
	ro_setSequ(sndBU, 0);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getSucc(0));

	// Statistics for monitor
	ro_setPcou(curDA, de_alistCount());
	ro_setFcou(curDA, de_flistCount());

	// Drop proofs
	if (
		check_membership() &&
		(memcmp(de_getPlate(), de_getSucc(0), plaSZ) != 0)
	)
	{
		UC rand;
	 	gen_random(&rand, 1);
	 	rand = (rand * 100) / 256;

	 	if (rand < (dropRATE / oveNU))
	 	{
	 		de_dropAlist();
	 		ro_setDrop(curDA, 1);
	 	}
	 	else
	 		ro_setDrop(curDA, 0);
	}
	
	st_outgoing[curOV] = ask_nsucc;
	clock_set(&st_timeout[curOV]);
}

static void ask_nsucc(void)
{
	if (curCO == TA_HIDRA_OUTGOING)
	{
		if (clock_delta(&st_timeout[curOV]) < timeoutPSUCC)
	 		bu_nulPred(curBU);
		else
		{
			for (int i = 0; i < sucNU-1; i++)
				de_setSucc(i, de_getSucc(i+1));

			memset(de_getSucc(sucNU-1), 0, plaSZ);

			st_started[curOV] = false;
			st_outgoing[curOV] = ask_psucc;
			ask_psucc();
		}
	 	return;
	}

	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

	if (ro_getOpco(rcvBU) != answPSUCC)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

	for (int i = 0; i < sucNU-1; i++)
		de_setSucc(i+1, ro_getPlat(rcvBU, i+1));

	for (int i = 0; i < (ro_maxPL-1-sucNU); i++)
		de_insertFinger(ro_getPlat(rcvBU, 1+sucNU+i));

	if (
		!memisnull(ro_getPlat(rcvBU, 0), plaSZ) &&
		op_between(
			de_getPlate() + ipaSZ, 
			ro_getPlat(rcvBU, 0) + ipaSZ,
			de_getSucc(0) + ipaSZ)
	)
	{	
		bu_setAddr(curBU, ro_getPlat(rcvBU, 0));
		serialize(maiSZ, bu_getLeng(curBU));

		ro_setOver(sndBU, curOV);
		ro_setOpco(sndBU, askNSUCC);
		ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
		ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
		ro_setSour(sndBU, de_getPlate());
		ro_setSend(sndBU, de_getPlate());
		ro_setReci(sndBU, ro_getPlat(rcvBU, 0));

		ro_setPlat(sndBU, 0, de_getPlate());

		st_outgoing[curOV] = check_nsucc;
		clock_set(&st_timeout[curOV]);
		return;
	}

	st_outgoing[curOV] = send_note;
	clock_set(&st_timeout[curOV]);
	send_note();
}

static void check_nsucc(void)
{
	if (curCO == TA_HIDRA_OUTGOING)
	{
		if (clock_delta(&st_timeout[curOV]) < timeoutNSUCC)
	 		bu_nulPred(curBU);
		else
		{
			st_started[curOV] = false;
			st_outgoing[curOV] = ask_psucc;
			ask_psucc();
		}
	 	return;
	}

	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

	if (ro_getOpco(rcvBU) != answNSUCC)
	{
		bu_nulPred(curBU);
	 	return;
	}

	de_setSucc(0, ro_getPlat(rcvBU, 0));

	for (int i = 0; i < sucNU-1; i++)
		de_setSucc(i+1, ro_getPlat(rcvBU, i+1));

	for (int i = 0; i < (ro_maxPL-1-sucNU); i++)
		de_insertFinger(ro_getPlat(rcvBU, 1+sucNU+i));

	st_outgoing[curOV] = send_note;
	clock_set(&st_timeout[curOV]);
	send_note();
}

static void send_note(void)
{
	if (membership[curOV])
		ro_setOpco(sndBU, sendGREET);
	else
		ro_setOpco(sndBU, sendREGARD);

	bu_setAddr(curBU, de_getSucc(0));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getSucc(0));
	
	ro_setPlat(sndBU, 0, de_getPlate());

	st_started[curOV] = false;
	st_outgoing[curOV] = ask_psucc;
}