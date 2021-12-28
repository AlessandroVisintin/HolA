#include "operation.h"

static void check_between(UC id1[dhMO], UC id2[dhMO], UC id3[dhMO])
{
	if (op_between(id1, id2, id3))
		printf("YES!!\n");
	else
		printf("NO!!\n");
}

int main(int argc, char const *argv[])
{
	printf("Generating test IDs...\n");
	UC ida[dhMO], idb[dhMO], idc[dhMO], result[dhMO];

	memset(ida, 0, dhMO);
	memset(idc, 255, dhMO);

	for (int i = 0; i < dhMO; i++)
		idb[i] = i;

	printf("Printing ID1...\n");
	memprint(ida, dhMO);

	printf("Printing ID2...\n");
	memprint(idb, dhMO);

	printf("Printing ID3...\n");
	memprint(idc, dhMO);

	printf("\n");
	printf("Is ID2 between ID1 and ID3? ");
	check_between(ida, idb, idc);

	printf("\n");
	printf("Is ID3 between ID1 and ID2? ");
	check_between(ida, idc, idb);

	printf("\n");
	printf("Is ID1 between ID2 and ID3? ");
	check_between(idb, ida, idc);

	printf("\n");
	printf("Is ID3 between ID2 and ID1? ");
	check_between(idb, idc, ida);

	printf("\n");
	printf("Is ID1 between ID3 and ID2? ");
	check_between(idc, ida, idb);

	printf("\n");
	printf("Is ID2 between ID3 and ID1? ");
	check_between(idc, idb, ida);

	printf("Trying to add ID1 and ID2...\n");
	op_add(ida, idb, result);
	memprint(result, dhMO);

	printf("Trying to add ID1 and ID3...\n");
	op_add(ida, idc, result);
	memprint(result, dhMO);

	printf("Trying to add ID2 and ID3...\n");
	op_add(idb, idc, result);
	memprint(result, dhMO);

	printf("Trying to subtract ID1 from ID2...\n");
	op_subtract(idb, ida, result);
	memprint(result, dhMO);

	printf("Trying to subtract ID2 from ID3...\n");
	op_subtract(idc, idb, result);
	memprint(result, dhMO);

	printf("Trying to subtract ID1 from ID3...\n");
	op_subtract(idc, ida, result);
	memprint(result, dhMO);

	printf("Trying to subtract ID2 from ID1...\n");
	op_subtract(ida, idb, result);
	memprint(result, dhMO);

	printf("Trying to subtract ID3 from ID2...\n");
	op_subtract(idb, idc, result);
	memprint(result, dhMO);

	printf("Trying to subtract ID3 from ID1...\n");
	op_subtract(ida, idc, result);
	memprint(result, dhMO);

	printf("Trying to calculate 2^432...\n");
	UC pow[finSZ] = {0, 0, 1, 176};
	op_exp2(pow, result);
	memprint(result, dhMO);

	printf("Invert last calculation...\n");
	op_log2(result, pow);
	memprint(pow, finSZ);

	return 0;
}