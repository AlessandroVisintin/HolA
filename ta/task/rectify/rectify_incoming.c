#include "rectify.h"
#include "../../device/device.h"

static void answ_oprev(void)
{
	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, answOPREV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));

	ro_setPlat(sndBU, 0, de_getPlate());
}

static void take_proof(void)
{
	ro_setOpco(sndBU, continuePROOF);

 	for (int i = 0; i < ro_maxPR; i++)
 	{
 		if (!de_insertProof(ro_getProo(rcvBU, i)))
 		{
 			ro_setOpco(sndBU, endRECTIFY);
 			break;
 		}
 	}

 	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));
}


void re_incoming(void)
{
	if (curOP == askOPREV)
		answ_oprev();
	else if (curOP == movePROOF)
		take_proof();
	else
		bu_nulPred(curBU);
}