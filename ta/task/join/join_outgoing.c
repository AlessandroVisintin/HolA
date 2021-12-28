#include "join.h"
#include "../../device/device.h"

static UC process[oveNU][4];
static volatile bool jo_started[oveNU];
static volatile bool jo_flags[oveNU];
static Clock jo_timeout[oveNU];

static void ask_join(void);
static void ask_slist(void);
static void send_proof(void);
static void end_join(void);

void join_init(void)
{
	for (int i = 0; i < oveNU; i++)
	{
		jo_flags[i] = false;
		jo_started[i] = false;
		jo_outgoing[i] = ask_join;
	}
}

bool join_done(void)
{	
	for (int i = 0; i < oveNU; i++)
	{
		if (!jo_flags[i])
			return false;
	}
	return true;
}

static void ask_join(void)
{
	if (jo_flags[curOV])
	{
		jo_started[curOV] = false;
		jo_outgoing[curOV] = ask_join;
		bu_nulPred(curBU);
		return;
	}

	if (jo_started[curOV] || curCO != TA_HIDRA_OUTGOING)
	{
		bu_nulPred(curBU);
		return;
	}

	jo_started[curOV] = true;
	gen_random((UC*) (&(process[curOV][0])), 4);

	bu_setAddr(curBU, de_getEntry());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, askJOIN);
	ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
	ro_setSequ(sndBU, 0);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getEntry());
	ro_setPlat(sndBU, 0, de_getPlate());

	jo_outgoing[curOV] = ask_slist;
	clock_set(&jo_timeout[curOV]);

}

static void ask_slist(void)
{
	if (jo_flags[curOV])
	{
		jo_started[curOV] = false;
		jo_outgoing[curOV] = ask_join;
		bu_nulPred(curBU);
		return;
	}

	if (curCO == TA_HIDRA_OUTGOING)
	{
	 	if (clock_delta(&jo_timeout[curOV]) < timeoutJOIN)
	 		bu_nulPred(curBU);
		else
		{
			jo_started[curOV] = false;
			jo_outgoing[curOV] = ask_join;
	 		ask_join();
		}
	 	return;
	}

	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

	if (ro_getOpco(rcvBU) != answJOIN)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

	de_setSucc(0, ro_getPlat(rcvBU, 0));

	bu_setAddr(curBU, de_getSucc(0));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, askSLIST);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getSucc(0));

	jo_outgoing[curOV] = send_proof;
	clock_set(&jo_timeout[curOV]);
}

static void send_proof(void)
{
	if (jo_flags[curOV])
	{
		jo_started[curOV] = false;
		jo_outgoing[curOV] = ask_join;
		bu_nulPred(curBU);
		return;
	}

	if (curCO == TA_HIDRA_OUTGOING)
	{
	 	if (clock_delta(&jo_timeout[curOV]) < timeoutSLIST)
	 		bu_nulPred(curBU);
		else
		{
			jo_started[curOV] = false;
			jo_outgoing[curOV] = ask_join;
	 		ask_join();
		}
	 	return;
	}

	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

	if (ro_getOpco(rcvBU) != answSLIST)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

	for (int i = 0; i < sucNU-1; i++)
	 	de_setSucc(i+1, ro_getPlat(rcvBU, i));

	for (int i = 0; i < (ro_maxPL - sucNU); i++)
		de_insertFinger(ro_getPlat(rcvBU, sucNU+i));

	UC plate[plaSZ], proof[proSZ];
	if (de_findSuccessor(de_getUid(), plate))
		ro_setOpco(sndBU, storePROOF);
	else
		ro_setOpco(sndBU, providePROOF);

	bu_setAddr(curBU, plate);
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, plate);
	de_produceProof(proof);
	ro_setProo(sndBU, 0, proof);

	jo_outgoing[curOV] = end_join;
	clock_set(&jo_timeout[curOV]);
}

static void end_join(void)
{
	if (jo_flags[curOV])
	{
		jo_started[curOV] = false;
		jo_outgoing[curOV] = ask_join;
		bu_nulPred(curBU);
		return;
	}

	if (curCO == TA_HIDRA_OUTGOING)
	{
	 	if (clock_delta(&jo_timeout[curOV]) < timeoutPROOF)
	 		bu_nulPred(curBU);
		else
		{
			jo_started[curOV] = false;
			jo_outgoing[curOV] = ask_join;
	 		ask_join();
		}
	 	return;
	}

	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

	if (ro_getOpco(rcvBU) != endJOIN)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

	de_insertFinger(ro_getSend(rcvBU));
	
	jo_started[curOV] = false;
	jo_flags[curOV] = true;
	jo_outgoing[curOV] = ask_join;

	bu_setAddr(curBU, de_getPlate());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, closeJOIN);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getPlate());
}