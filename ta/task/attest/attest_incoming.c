#include "attest.h"
#include "../../device/device.h"

static void answ_rand(void)
{
 	UC plate[plaSZ];
 	if (de_findSuccessor(
 		ro_getPlat(rcvBU, 0)+ipaSZ, plate)
 	)
 		ro_setOpco(sndBU, startCHAL);
 	else
 	{
 		de_insertFinger(plate);
 		ro_setOpco(sndBU, askRAND);
 	}

 	bu_setAddr(curBU, plate);
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);

	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, plate);

	ro_setPlat(sndBU, 0, ro_getPlat(rcvBU, 0));
}

static void accept_chal(void)
{
	UC proof[proSZ];

	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, acceptCHAL);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));

	ro_setPlat(sndBU, 0, de_getPlate());
	de_produceProof(proof);
	ro_setProo(sndBU, ro_maxPR-1, proof);
}

static void forw_proof(void)
{
	UC plate[plaSZ];
 	if (de_findSuccessor(ro_getProo(rcvBU, 0), plate))
 		ro_setOpco(sndBU, checkPROOF);
 	else
 	{
 		de_insertFinger(plate);
 		ro_setOpco(sndBU, forwPROOF);
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

static void check_proof(void)
{
	UC proof[proSZ];
 	if (de_findProof(ro_getProo(rcvBU, 0), proof))
 	{
 		ro_setOpco(sndBU, returnPROOF);
 		ro_setProo(sndBU, 0, proof);
 	}
 	else
 		ro_setOpco(sndBU, lostPROOF);

 	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));
}

static void restore_data(void)
{
	UC plate[plaSZ];
 	if (de_findSuccessor(ro_getProo(rcvBU, 0), plate))
 	{
 		if (curOP == restorePROOF)
 			ro_setOpco(sndBU, savePROOF);
 		else
 			ro_setOpco(sndBU, saveMISSING);
 	}
 	else
 	{
 		de_insertFinger(plate);
 		if (curOP == restorePROOF)
 			ro_setOpco(sndBU, restorePROOF);
 		else
 			ro_setOpco(sndBU, restoreMISSING);
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

static void save_data(void)
{ 
	de_insertProof(ro_getProo(rcvBU, 0));

	if (curOP == savePROOF)
 		ro_setOpco(sndBU, restoredPROOF);
 	else
 		ro_setOpco(sndBU, restoredMISSING);

	bu_setAddr(curBU, ro_getSour(rcvBU));
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, ro_getSour(rcvBU));
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, ro_getSour(rcvBU));
}

void at_incoming(void)
{
	if (curOP == askRAND)
		answ_rand();
	
	else if (curOP == startCHAL)
	  	accept_chal();
	
	else if (curOP == forwPROOF)
		forw_proof();
	
	else if (curOP == checkPROOF)
	  	check_proof();
	
	else if (curOP == restorePROOF)
	 	restore_data();
	
	else if (curOP == savePROOF)
	  	save_data();

	else if (curOP == restoreMISSING)
	 	restore_data();
	
	else if (curOP == saveMISSING)
	  	save_data();
	
	else
		bu_nulPred(curBU);
}