#include "attest.h"
#include "../../device/device.h"

// General attestation state
static volatile bool at_started; 
static volatile bool at_done[oveNU];
static UC at_random[oveNU][plaSZ];

// Choosen prover info
static UC at_plate[plaSZ];
static UC at_proof[proSZ];

// Lost proof info
static volatile bool at_found;
static volatile bool at_lost[oveNU];
static UC at_cache[proSZ];

static UC process[oveNU][4]; 
static UC sequence[oveNU][1];
static Clock at_timeout[oveNU];

static void ask_rand(void);
static void analyze_chal(void);
static void start_chal(void);
static void close_chal(void);
static void restore_proof(void);
static void close_restore(void);

bool attest_isDone(void)
{ return at_done[curOV]; }

bool attest_checkLost(void)
{	
	for (int i = 0; i < oveNU; i++)
	{
	 	if (at_lost[i])
			return true;
	} 
	return false;
}

static bool at_checkRecovery(void)
{
	for (int i = 0; i < oveNU; i++)
	{
	 	if (!at_lost[i])
			return false;
	} 
	return true;
}

static bool at_checkDone(void)
{
	for (int i = 0; i < oveNU; i++)
	{
	 	if (!at_done[i])
			return false;
	} 
	return true; 
}

void attest_init(void)
{
	at_started = false;
	at_found = false;
	
	for (int i = 0; i < oveNU; i++)
	{
		at_done[i] = false;
		at_lost[i] = false;
		at_outgoing[i] = ask_rand;
	}
}

static void ask_rand(void)
{	
	if (
		(!check_membership()) ||
		(at_started && at_done[curOV]) ||
		(curCO != TA_HIDRA_OUTGOING)
	)
	{
		at_outgoing[curOV] = ask_rand;
		bu_nulPred(curBU);
		return;
	}

	sequence[curOV][0] = 0;
	memset((UC*) (&(at_random[curOV][0])), 0, plaSZ);
	gen_random((UC*) (&(process[curOV][0])), 4);
	gen_random((UC*) (&(at_random[curOV][ipaSZ])), dhMO);

	UC plate[plaSZ];
	if (de_findSuccessor(
			(UC*) (&(at_random[curOV][ipaSZ])), plate)
	)
		ro_setOpco(sndBU, startCHAL);
	else
		ro_setOpco(sndBU, askRAND);

	bu_setAddr(curBU, plate);
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);

	ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
	ro_setSequ(sndBU, 0);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, plate);

	ro_setPlat(sndBU, 0, (UC*) (&(at_random[curOV][0])));

	at_outgoing[curOV] = analyze_chal;
	clock_set(&at_timeout[curOV]);
}

static void analyze_chal(void)
{
 	if (curCO == TA_HIDRA_OUTGOING)
	{
		if (clock_delta(&at_timeout[curOV]) < timeoutRAND)
			bu_nulPred(curBU);
		
		else if (at_started && !at_done[curOV])
		{
			at_outgoing[curOV] = start_chal;
			
			ro_setProc(rcvBU, (UC*) (&(process[curOV][0])));
			ro_setSequ(rcvBU, 0);
			start_chal();
		}
		else
		{
			at_outgoing[curOV] = ask_rand;
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

	if (ro_getOpco(rcvBU) != acceptCHAL)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

	sequence[curOV][0] = ro_getSequ(rcvBU) + 1;
	de_insertFinger(ro_getPlat(rcvBU, 0));

 	if (!at_started)
 	{
 		at_started = true;
 		memcpy(
 			(UC*) (&(at_plate[0])), ro_getPlat(rcvBU, 0), plaSZ
 		);

 		memcpy(
 			(UC*) (&(at_proof[0])), ro_getProo(rcvBU, ro_maxPR-1), proSZ
 		);
	}

	at_outgoing[curOV] = start_chal;
	start_chal();
}

static void start_chal(void)
{
	UC plate[plaSZ];
	if (de_findSuccessor(at_proof, plate))
		ro_setOpco(sndBU, checkPROOF);
	else
		ro_setOpco(sndBU, forwPROOF);

	bu_setAddr(curBU, plate);
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);

	ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, plate);

	ro_setProo(sndBU, 0, at_proof);

	at_outgoing[curOV] = close_chal;
	clock_set(&at_timeout[curOV]);
}

static void close_chal(void)
{
 	if (curCO == TA_HIDRA_OUTGOING)
 	{
 		if (clock_delta(&at_timeout[curOV]) < timeoutCHAL)
 			bu_nulPred(curBU);
 		else
 		{
 			at_lost[curOV] = true;
 			at_outgoing[curOV] = restore_proof;
 			restore_proof();
 		}
 		return;
 	}

 	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

 	if (curOP == lostPROOF)
	{
		de_insertFinger(ro_getSend(rcvBU));

 		at_lost[curOV] = true;
 		sequence[curOV][0] = ro_getSequ(rcvBU);
 		at_outgoing[curOV] = restore_proof;
 		restore_proof();
 		return;
 	}
 	else if(curOP != returnPROOF)
 	{
		bu_nulPred(curBU);
	 	return;
 	}

 	de_insertFinger(ro_getSend(rcvBU));

	if (!at_found)
 	{
 		at_found = true;
 		memcpy((UC*) (&(at_cache[0])), ro_getProo(rcvBU, 0), proSZ);
 	}

 	// Tamper attestation
 	UC rand;
 	gen_random(&rand, 1);
 	rand = (rand * 100) / 256;

 	if (rand < (dropRATE / oveNU))
 	{
 		memset(ro_getProo(rcvBU, 0), 0, proSZ);
 		ro_setFlag(curDA, 1);
 	}
 	else
 		ro_setFlag(curDA, 0);
	
	if (memcmp(
			(UC*) (&(at_proof[0])), ro_getProo(rcvBU, 0), proSZ) != 0)
		ro_setOpco(sndBU, devTAMPERED);
 	else
		ro_setOpco(sndBU, devCHECKED);

	bu_setAddr(curBU, de_getPlate());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getPlate());

	at_outgoing[curOV] = ask_rand;
	at_done[curOV] = true;
	if (at_checkDone())
	{
		for (int i = 0; i < oveNU; i++)
		{
			at_lost[i] = false;
 			at_done[i] = false;
		}

 		at_started = false;
 		at_found = false;
	}
}

static void restore_proof(void)
{
	if (!at_found && !at_checkRecovery())
	{
		at_outgoing[curOV] = restore_proof;
		bu_nulPred(curBU);
		return;
	}

	UC plate[plaSZ];
	if (at_found)
	{
		ro_setProo(sndBU, 0, at_cache);
		if (de_findSuccessor(at_cache, plate))
			ro_setOpco(sndBU, savePROOF);
		else
			ro_setOpco(sndBU, restorePROOF);
	}
	else
	{
		ro_setProo(sndBU, 0, at_proof);
		if (de_findSuccessor(at_proof, plate))
			ro_setOpco(sndBU, saveMISSING);
		else
			ro_setOpco(sndBU, restoreMISSING);
	}

	sequence[curOV][0] += 1;

	bu_setAddr(curBU, plate);
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setProc(sndBU, (UC*) (&(process[curOV][0])));
	ro_setSequ(sndBU, sequence[curOV][0]);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, plate);

	at_outgoing[curOV] = close_restore;
	clock_set(&at_timeout[curOV]);
}

static void close_restore(void)
{
	if (curCO == TA_HIDRA_OUTGOING)
	{
		if (clock_delta(&at_timeout[curOV]) < timeoutRESTORE)
			bu_nulPred(curBU);
		else
		{
			at_outgoing[curOV] = restore_proof;
			restore_proof();
		}
 		return;
 	}

 	if (memcmp(
		(UC*) (&(process[curOV][0])), ro_getProc(rcvBU), 4) != 0)
	{
		bu_nulPred(curBU);
	 	return;
	}

	if (
		ro_getOpco(rcvBU) != restoredPROOF &&
		ro_getOpco(rcvBU) != restoredMISSING
	)
	{
	 	bu_nulPred(curBU);
	 	return;
	}

	de_insertFinger(ro_getSend(rcvBU));

 	at_outgoing[curOV] = ask_rand;
 	at_done[curOV] = true;

 	if (at_checkDone())
 	{
 		for (int i = 0; i < oveNU; i++)
 		{
 			at_lost[i] = false;
 			at_done[i] = false;
 		}

 		at_started = false;
 		at_found = false;
 	}

 	bu_setAddr(curBU, de_getPlate());
	serialize(maiSZ, bu_getLeng(curBU));

	ro_setOver(sndBU, curOV);
	ro_setOpco(sndBU, closeRESTORE);
	ro_setProc(sndBU, ro_getProc(rcvBU));
	ro_setSequ(sndBU, ro_getSequ(rcvBU) + 1);
	ro_setSour(sndBU, de_getPlate());
	ro_setSend(sndBU, de_getPlate());
	ro_setReci(sndBU, de_getPlate());
}