#include "device.h"

int main(int argc, char const *argv[])
{
	UC mac[macSZ], uid[dhMO], att[dhMO];
	UC pvt[rsaPV], pub[rsaPU], cer[rsaBK];

	UC secret[dhEX], plate[plaSZ], entry[plaSZ];
	UC prev[plaSZ], slist[sucNU][plaSZ];

	printf("Generating DEVICE info...\n");
	memset(mac, 0, macSZ);
	memset(uid, 1, dhMO);
	memset(att, 2, dhMO);
	memset(pvt, 3, rsaPV);
	memset(pub, 4, rsaPU);
	memset(cer, 5, rsaBK);

	printf("Setting DEVICE info...\n");
	de_setMac(mac);
	de_setUid(uid);
	de_setPvt(pvt);
	de_setPub(pub);
	de_setCer(cer);

	printf("Printing DEVICE info...\n");
	de_printDevice();

	printf("Generating NODE info...\n");
	curOV = 1;
	memset(secret, 0, dhEX);
	memset(plate, 1, plaSZ);
	memset(entry, 2, plaSZ);
	memset(prev, 3, plaSZ);

	for (int i = 0; i < sucNU; i++)
		memset(&slist[i][0], i, plaSZ);

	printf("Setting NODE info...\n");
	de_setSecret(secret);
	de_setPlate(plate);
	de_setEntry(entry);
	de_setPrev(prev);

	for (int i = 0; i < sucNU; i++)
		de_setSucc(i, &slist[i][0]);

	printf("Printing NODE info...\n");
	de_printNode();

	printf("Testing finger list...\n");
	de_initFlist();

	UC finger[plaSZ];
	printf("Inserting first finger...\n");
	memset((UC*) (&(finger[0])), 1, plaSZ);
	de_insertFinger(finger);

	printf("Inserting second finger...\n");
	memset((UC*) (&(finger[plaSZ-1])), 2, 1);
	de_insertFinger(finger);

	printf("Inserting third finger...\n");
	memset((UC*) (&(finger[plaSZ-2])), 3, 2);
	de_insertFinger(finger);

	printf("Inserting fourth finger...\n");
	memset((UC*) (&(finger[plaSZ/2])), 4, plaSZ-plaSZ/2);
	de_insertFinger(finger);

	printf("Printing NODE info...\n");
	de_printNode();

	printf("Trying to find successor... ");
	memset((UC*) (&(finger[0])), 1, plaSZ);
	finger[plaSZ-1] = 2;

	UC result[plaSZ];
	if (de_findSuccessor(finger+ipaSZ, result))
		printf("found!\n");
	else
		printf("not found\n");

	memprint(result, plaSZ);

	printf("Testing proof list...\n");
	de_initAlist();	

	UC proof[proSZ], extracted[dhMO], relocate[proSZ];
	printf("Inserting first proof...\n");
	memset((UC*) (&(proof[0])), 10, proSZ);
	de_insertProof(proof);

	printf("Inserting first proof...\n");
	memset((UC*) (&(proof[0])), 20, proSZ);
	de_insertProof(proof);

	printf("Inserting first proof...\n");
	memset((UC*) (&(proof[0])), 30, proSZ);
	de_insertProof(proof);

	printf("Printing NODE info...\n");
	de_printNode();

	printf("Trying to find proof... ");
	if (de_findProof(proof, extracted))
	{
		printf("found!\n");
		memprint(extracted, dhMO);
	}
	else
		printf("not found!\n");

	printf("Trying to relocate...\n");
	memset(plate, 25, plaSZ);
	while(de_relocateProof(plate, relocate))
	{
		printf("New relocation!\n");
		memprint(relocate, proSZ);
	}

	printf("Printing NODE info...\n");
	de_printNode();

	de_holdAlist();
	printf("After HOLD...\n");
	de_printNode();

	de_relocateProof(plate, relocate);
	de_purgeAlist();
	printf("After relocation and PURGE...\n");
	de_printNode();

	de_closeAlist();
	de_closeFlist();
	return 0;
}