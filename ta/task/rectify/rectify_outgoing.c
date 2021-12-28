#include "rectify.h"
#include "../../device/device.h"
#include <operation/operation.h>

// forward declaration
static void ask_oprev(void);
static void check_oprev(void);
static void move_proof(void);
static void forw_proof(void);

static volatile bool re_started[oveNU];
static UC process[oveNU][4];
static Clock re_timeout[oveNU];
static UC re_currentNew[oveNU][plaSZ];
UC re_info[oveNU][plaSZ];
bool re_new[oveNU];

void rectify_init(void)
{
	for (int i = 0; i < oveNU; i++)
	{
		re_new[i] = false;
		re_started[i] = false;
		re_outgoing[i] = ask_oprev;
	}
}

bool rectify_checkNew(void)
{ return re_new[curOV]; }

static void ask_oprev(void)
{
	if (
		(!check_membership()) ||
		re_started[curOV] ||
		!re_new[curOV] ||
		curCO != TA_HIDRA_OUTGOING)
	{
		re_started[curOV] = false;
		re_new[curOV] = false;
		re_outgoing[curOV] = ask_oprev;
		bu_nulPred(curBU);
		return;
	}

	re_started[curOV] = true;
	gen_random((UC*) (&(process[curOV][0])), 4);

	if(memisnull(de_getPrev(), plaSZ))
	{
		de_setPrev((UC*) (&(re_info[curOV][0])));
		re_new[curOV] = false;
		re_outgoing[curOV] = move_proof;

		ro_setProc(rcvBU, (UC*) (&(process[curOV][0])));
		ro_setSequ(rcvBU, 0);
		move_proof();
		return;
	}

	memcpy(
		(UC*) (&(re_currentNew[curOV][0])),
		(UC*) (&(re_info[curOV][0])),
		plaSZ
	);

	bu_setAddr(curBU, de_getPrev());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, askOPREV);
	ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
	ro_setSequ(sndBU, 0);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getPrev());

	re_outgoing[curOV] = check_oprev;
	clock_set(&re_timeout[curOV]);
}

static void check_oprev(void)
{
	UC *prev = (UC*) (&(re_currentNew[curOV][0]));

	if (curCO == TA_HIDRA_OUTGOING)
	{
		if (clock_delta(&re_timeout[curOV]) < timeoutOPREV)
			bu_nulPred(curBU);
		else
		{
			de_setPrev(prev);
			re_new[curOV] = false;
			re_outgoing[curOV] = move_proof;

			ro_setProc(rcvBU, (UC*) (&(process[curOV][0])));
			ro_setSequ(rcvBU, 1);
			move_proof();
		}
		return;
	}

	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

	if (ro_getOpco(rcvBU) != answOPREV)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

 	if (
 		op_between(
 			de_getPrev()+ipaSZ,
 			prev+ipaSZ,
 			de_getPlate()+ipaSZ)
 	)
 	{
		de_setPrev(prev);
		re_new[curOV] = false;
		re_outgoing[curOV] = move_proof;
		move_proof();
		return;
 	}

	re_new[curOV] = false;
	re_started[curOV] = false;
	re_outgoing[curOV] = ask_oprev;

	bu_setAddr(curBU, de_getPlate());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, closeRECTIFY);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getPlate());
}

static void move_proof(void)
{
	bu_setAddr(curBU, de_getPrev());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, movePROOF);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getPrev());

	UC proof[proSZ];
	for (int i = 0; i < ro_maxPR; i++)
	{
		if (de_relocateProof(de_getPrev(), proof))
			ro_setProo(sndBU, i, proof);
		else
			break;
	}

	re_outgoing[curOV] = forw_proof;
	clock_set(&re_timeout[curOV]);
}

static void forw_proof(void)
{
	if (curCO == TA_HIDRA_OUTGOING)
	{
		if (clock_delta(&re_timeout[curOV]) < timeoutRELOC)
			bu_nulPred(curBU);
		else
		{
			re_started[curOV] = false;
			re_outgoing[curOV] = ask_oprev;
			bu_nulPred(curBU);
		}
		return;
	}

	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU); 
	 	return;
	}

	if (ro_getOpco(rcvBU) == continuePROOF)
	{
		re_outgoing[curOV] = move_proof;
		move_proof();
		return;
	}

	else if (ro_getOpco(rcvBU) != endRECTIFY)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

	re_started[curOV] = false;
	re_outgoing[curOV] = ask_oprev;

	bu_setAddr(curBU, de_getPlate());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, closeRECTIFY);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getPlate());
}
