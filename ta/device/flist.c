#include "flist.h"
#include <list/list.h>
#include <operation/operation.h>

UC PLATES[oveNU][plaSZ];

static bool smaller_pos(UC *list, UC *check)
{ return (memcmp(list, check, finSZ) <= 0); }

static bool equal_pos(UC *list, UC *check)
{ return (memcmp(list, check, finSZ) == 0); }

static bool between(UC *list, UC *check)
{ 
	return op_between(
		(UC*) (&(PLATES[curOV][ipaSZ])),
		(UC*) (&(list[finSZ+ipaSZ])),
		check
	); 
}

Flist* flist_create(UC plate[plaSZ])
{
	memcpy((UC*) (&(PLATES[curOV][0])), (UC*) (&(plate[0])), plaSZ);
	return list_create(); 
}

int flist_count(Flist *head)
{ return list_getCount(head); }

errCO flist_insert(Flist *head, UC plate[plaSZ])
{
	if (!head || !plate)
		return BAD_PARAMETERS;

	UC data[finSZ + plaSZ];
	
	op_subtract(
		(UC*) (&(plate[ipaSZ])), 
		(UC*) (&(PLATES[curOV][ipaSZ])),
		(UC*) (&(data[finSZ+ipaSZ]))
	);
	
	op_log2(
		(UC*) (&(data[finSZ+ipaSZ])), 
		(UC*) (&(data[0]))
	);
	
	memcpy(
		(UC*) (&(data[finSZ])), (UC*) (&(plate[0])), plaSZ);

	return list_addByData(
				head, (UC*) (&(data[0])), finSZ+plaSZ, 
				smaller_pos, equal_pos
			);
}

UC* flist_getPlateByPos(Flist *head, UC pos[finSZ])
{
	UC *p = list_getByData(head, (UC*) (&(pos[0])), equal_pos);

	if (p)
		return (UC*) (&(p[finSZ]));
	else
		return NULL;
}

UC* flist_getPlateByIndex(Flist *head, int index)
{
	UC *p = list_getByIndex(head, index); 

	if (p)
		return (UC*) (&(p[finSZ]));
	else
		return NULL; 
}

bool flist_closestPredecessor(
	Flist *head, UC id[dhMO], UC result[plaSZ])
{
	if (!head || !id || !result)
		return false;

	UC data[finSZ+plaSZ];
	errCO r = list_extractByData(
 				head, (UC*) (&(id[0])), between, (UC*) (&(data[0]))
 			);

 	if (r == SUCCESS)
 	{
 		memcpy((UC*) (&(result[0])), (UC*) (&(data[finSZ])), plaSZ);
 		return true;
 	}
 	
 	return false;
}

errCO flist_destroy(Flist *head)
{ return list_destroy(head); }

void flist_print(Flist *head)
{
	printf("Finger list:\n");
 	list_print(head);
}