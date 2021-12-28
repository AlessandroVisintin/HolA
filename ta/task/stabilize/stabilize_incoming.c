#include "stabilize.h"
#include "../../device/device.h"

static void answ_psucc(void)
{
	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, answPSUCC);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));
	
	ro_setPlat(sndBU, 0, de_getPrev());	

	for (int i = 0; i < sucNU; i++)
		ro_setPlat(sndBU, i+1, de_getSucc(i));

	UC *ptr;
	for (int i = 0; i < (ro_maxPL - 1 - sucNU); i++)
	{
	 	if ((ptr = de_getFinger(i)))
	 		ro_setPlat(sndBU, sucNU + 1 + i, ptr);
	 	else
	 		break;
	}
}

static void answ_nsucc(void)
{
	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, answNSUCC);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));
	
	ro_setPlat(sndBU, 0, de_getPlate());	

	for (int j = 0; j < sucNU; j++)
		ro_setPlat(sndBU, j+1, de_getSucc(j));

	UC *ptr;
	for (int j = 0; j < (ro_maxPL - 1 - sucNU); j++)
	{
	 	if ((ptr = de_getFinger(j)))
	 		ro_setPlat(sndBU, sucNU + 1 + j, ptr);
	 	else
			break;
	}
}

static void recv_note(void)
{
	if (memcmp(ro_getPlat(rcvBU, 0), de_getPrev(), plaSZ) != 0)
	{
		memcpy(
			(UC*) (&(re_info[curOV][0])), 
			ro_getPlat(rcvBU, 0), 
			plaSZ
		);
		re_new[curOV] = true;
	
		if (curOP == sendGREET && !membership[curOV])
		{
			membership[curOV] = true;

			bu_setAddr(curBU, de_getSucc(0));
			serialize(maiSZ, bu_getLeng(curBU));

			ro_setOver(sndBU, curOV);
			ro_setOpco(sndBU, purgePROOF);
			ro_setProc(sndBU, ro_getProc(rcvBU));
			ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
			ro_setSour(sndBU, ro_getSour(rcvBU));
			ro_setSend(sndBU, de_getPlate());
			ro_setReci(sndBU, de_getSucc(0));
			
			ro_setPlat(sndBU, 0, ro_getPlat(rcvBU, 0));
			
			return;
		}
	}
	
	bu_nulPred(curBU);
}

static void purge_proof(void)
{ de_purgeAlist(de_getPrev()); }

void st_incoming(void)
{
	if (curOP == askPSUCC)
		answ_psucc();

	else if (curOP == askNSUCC)
	 	answ_nsucc();

	else if (curOP == sendREGARD)
		recv_note();

	else if (curOP == sendGREET)
		recv_note();

	else if (curOP == purgePROOF)
		purge_proof();
	
	else
		bu_nulPred(curBU);
}