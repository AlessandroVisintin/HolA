#include "join.h"
#include "../../device/device.h"
#include <operation/operation.h>

static void answ_join(void)
{
	UC plate[plaSZ];
	if (memcmp(ro_getPlat(rcvBU, 0), de_getPlate(), plaSZ) == 0)
	{
		bu_setAddr(curBU, ro_getSour(rcvBU));
	 	ro_setOpco(sndBU, answJOIN);
	 	ro_setReci(sndBU, ro_getSour(rcvBU));
	 	ro_setPlat(sndBU, 0, de_getPlate());
	}
	else if (de_findSuccessor(ro_getPlat(rcvBU, 0)+ipaSZ, plate))
	{
		bu_setAddr(curBU, ro_getSour(rcvBU));
	 	ro_setOpco(sndBU, answJOIN);
	 	ro_setReci(sndBU, ro_getSour(rcvBU));
	 	ro_setPlat(sndBU, 0, plate);
	}
	else
	{
		de_insertFinger(plate);
		
		bu_setAddr(curBU, plate);
	 	ro_setOpco(sndBU, askJOIN);
	 	ro_setReci(sndBU, plate);
	 	ro_setPlat(sndBU, 0, ro_getPlat(rcvBU, 0));
	}

	serialize(maiSZ, bu_getLeng(curBU));
	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
}

static void answ_slist(void)
{
	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));
	
	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, answSLIST);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));
	
	for (int i = 0; i < sucNU; i++)
		ro_setPlat(sndBU, i, de_getSucc(i));

	UC *ptr;
	for (int i = 0; i < (ro_maxPL - sucNU); i++)
	{
	 	if ((ptr = de_getFinger(i)))
	 		ro_setPlat(sndBU, sucNU+i, ptr);
	 	else
	 		break;
	}
}

static void forw_proof(void)
{	
	UC plate[plaSZ];
	if (de_findSuccessor(ro_getProo(rcvBU, 0), plate))
		ro_setOpco(sndBU, storePROOF);
	else
	{
		de_insertFinger(plate);
		ro_setOpco(sndBU, providePROOF);
	}

	bu_setAddr(curBU, plate);
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, plate);
	ro_setProo(sndBU, 0, ro_getProo(rcvBU, 0));
}

static void save_proof(void)
{
	de_insertProof(ro_getProo(rcvBU, 0));

	if (
		(memcmp(ro_getSend(rcvBU), de_getPrev(), plaSZ) == 0) ||
		op_between(
			de_getPrev() + ipaSZ, 
			ro_getProo(rcvBU, 0),
			de_getPlate() + ipaSZ)
	)
	{
		bu_setAddr(curBU, ro_getSour(rcvBU));
		ro_setOpco(sndBU, endJOIN);
		ro_setReci(sndBU, ro_getSour(rcvBU));
	}
	else
	{
		UC result[proSZ];
		de_findProof(ro_getProo(rcvBU, 0), result);

		bu_setAddr(curBU, de_getPrev());
		ro_setOpco(sndBU, updateCHAIN);
		ro_setReci(sndBU, de_getPrev());
		ro_setProo(sndBU, 0, result);
	}

	serialize(maiSZ, bu_getLeng(curBU));
	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
		
}

static void upgrade_chain(void)
{
	de_insertProof(ro_getProo(rcvBU, 0));

	if (
		memisnull(de_getPrev(), plaSZ) ||
		op_between(
			de_getPrev() + ipaSZ, 
			ro_getProo(rcvBU, 0),
			de_getPlate() + ipaSZ)
	)
	{
		bu_setAddr(curBU, ro_getSour(rcvBU));
		ro_setOpco(sndBU, endJOIN);
		ro_setReci(sndBU, ro_getSour(rcvBU));
	}
	else
	{
		UC result[proSZ];
		de_findProof(ro_getProo(rcvBU, 0), result);

		bu_setAddr(curBU, de_getPrev());
		ro_setOpco(sndBU, updateCHAIN);
		ro_setReci(sndBU, de_getPrev());
		ro_setProo(sndBU, 0, result);
	}

	serialize(maiSZ, bu_getLeng(curBU));
	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());	
}

void jo_incoming(void)
{
	if (curOP == askJOIN)
		answ_join();
	else if (curOP == askSLIST)
		answ_slist();
	else if (curOP == providePROOF)
		forw_proof();
	else if (curOP == storePROOF)
		save_proof();
	else if (curOP == updateCHAIN)
		upgrade_chain();
	else
		bu_nulPred(curBU);
}