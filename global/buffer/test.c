#include "../global.h"

int main(int argc, char const *argv[])
{
	UC buf[bufSZ];
	UC *dat = bu_getData(buf);

	bu_nulPred(buf);
	bu_nulData(buf);

	printf("Buffer total length: %d\n", bufSZ);
	printf("Parameters total length: %d\n", parSZ);
	printf("Certificate total length: %d\n", cerSZ);
	printf("Admittance total length: %d\n", admSZ);
	printf("Routine total length: %d\n", rouSZ);

	printf("Testing pre-data...\n");
	UC addr[ipaSZ], leng[4];

	printf("Generating address...\n");
	memset(addr, 0, ipaSZ);

	printf("Generating data length...\n");
	memset(leng, 1, 4);

	printf("Setting pre-data...\n");
	bu_setAddr(buf, addr);
	bu_setLeng(buf, leng);

	printf("Printing pre-data...\n");
	memprint(buf, preSZ);

	printf("Testing parameters...\n");
	UC entr[ipaSZ], ipad[ipaSZ], maca[macSZ];

	printf("Generating ENTRY address...\n");
	memset(entr, 0, ipaSZ);

	printf("Generating IP address...\n");
	memset(ipad, 1, ipaSZ);

	printf("Generating MAC address...\n");
	memset(maca, 2, macSZ);

	printf("Setting pre-data...\n");
	pa_setEntr(dat, entr);
	pa_setIpad(dat, ipad);
	pa_setMaca(dat, maca);

	printf("Printing parameters\n");
	memprint(dat, parSZ);

	bu_nulData(buf);
	
	printf("Testing certificate...\n");
	UC appl[ipaSZ], cert[rsaBK], publ[rsaPU];

	printf("Generating APPLICANT address...\n");
	memset(appl, 0, ipaSZ);

	printf("Generating CERTIFICATE...\n");
	memset(cert, 1, rsaBK);

	printf("Generating PUBLIC KEY...\n");
	memset(publ, 2, rsaPU);

	printf("Setting pre-data...\n");
	ce_setAppl(dat, appl);
	ce_setCert(dat, cert);
	ce_setPubl(dat, publ);

	printf("Printing certificate...\n");
	memprint(dat, cerSZ);

	bu_nulData(buf);
	
	printf("Testing admittance...\n");
	UC rsab[admSZ];

	printf("Generating RSA blocks...\n");
	for (int i = 0; i < oveNU; i++)
		memset(rsab+(i*rsaBK), i, rsaBK);

	printf("Setting admittance...\n");
	for (int i = 0; i < oveNU; i++)
		ad_setRsab(dat, i, rsab+(i*rsaBK));

	printf("Printing admittance...\n");
	memprint(dat, admSZ);

	bu_nulData(buf);
	
	printf("Testing routine...\n");

	printf("Generating HEADER...\n");
	UC over[1], opco[1], proc[4], sequ[1];
	UC sour[plaSZ], send[plaSZ], reci[plaSZ];

	memset(over, 0, 1);
	memset(opco, 1, 1);
	memset(proc, 2, 4);
	memset(sequ, 3, 1);
	memset(sour, 4, plaSZ);
	memset(send, 5, plaSZ);
	memset(reci, 6, plaSZ);

	printf("Generating BODY...\n");
	UC body[ro_bodSZ], chec[aesBK], vect[aesBK];
	
	for (int i = 0; i < ro_maxPL; i++)
		memset(body+(i*plaSZ), i, plaSZ);

	memset(chec, ro_maxPL, aesBK);
	memset(vect, ro_maxPL+1, aesBK);

	printf("Generating MONITOR...\n");
	UC flag[1], star[timSZ], decr[timSZ];
	UC encr[timSZ], endt[timSZ];

	memset(flag, 0, 1);
	memset(star, 1, timSZ);
	memset(decr, 2, timSZ);
	memset(encr, 3, timSZ);
	memset(endt, 4, timSZ);

	printf("Setting header...\n");
	ro_setOver(dat, over);
	ro_setOpco(dat, opco);
	ro_setProc(dat, proc);
	ro_setSequ(dat, sequ);
	ro_setSour(dat, sour);
	ro_setSend(dat, send);
	ro_setReci(dat, reci);

	printf("Setting body...\n");
	for (int i = 0; i < ro_maxPL; i++)
		ro_setPlat(dat, i, body+(i*plaSZ));

	ro_setChec(dat, chec);
	ro_setVect(dat, vect);

	printf("Setting monitor...\n");
	ro_setFlag(dat, flag);
	ro_setStar(dat, star);
	ro_setDecr(dat, decr);
	ro_setEncr(dat, encr);
	ro_setEndt(dat, endt);
	ro_setCopy(dat, dat);
	
	printf("Buffer pointer: %p\n", (void*) buf);
	printf("Data pointer: %p\n", (void*) dat);

	printf("Printing routine...\n"); 
	memprint(dat, rouSZ);

	return 0;
}