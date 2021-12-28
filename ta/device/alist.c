#include "alist.h"
#include <list/list.h>
#include <operation/operation.h>

static UC PLATES[oveNU][plaSZ];
static int relocate_index[oveNU];
static int relocate_count[oveNU];

static bool equal_uid(UC *list, UC *check)
{ return (memcmp(list, check, dhMO) == 0); }

static bool update_proof(UC *list, UC *check)
{	
 	(void) list;
 	(void) check;
 	return true; 
}

static bool relocate(UC *list, UC *check)
{
	if (relocate_count[curOV] < relocate_index[curOV])
	{
		relocate_count[curOV] += 1;
		return false;
	}

	relocate_count[curOV] += 1;
	return op_between(
			list, check, (UC*) (&(PLATES[curOV][ipaSZ])));
}

static bool purge(UC *list, UC *check)
{
	return op_between(
			list, check, (UC*) (&(PLATES[curOV][ipaSZ])));
}

Alist* alist_create(UC plate[plaSZ])
{	
	for (int i = 0; i < oveNU; i++)
	{
		relocate_index[i] = 0;
		relocate_count[i] = 0;
	}
	
	memcpy((UC*) (&(PLATES[curOV][0])), (UC*) (&(plate[0])), plaSZ); 
	return list_create(); 
}

int alist_count(Alist *head)
{ return list_getCount(head); }

errCO alist_insert(Alist *head, UC pro[proSZ])
{	
	if (!head || !pro)
		return BAD_PARAMETERS;

	return list_addByData(
		head, (UC*) (&(pro[0])), proSZ, equal_uid, update_proof);
}

bool alist_findProof(
	Alist *head, UC uid[dhMO], UC result[proSZ])
{
	if (!head || !uid || !result)
		return BAD_PARAMETERS;

	UC *p = list_getByData(head, (UC*) (&(uid[0])), equal_uid);
	if (p)
	{
		memcpy((UC*) (&(result[0])), p, proSZ);
		return true;
	}
	else
		return false;	
}

bool alist_relocateProof(
	Alist *head, UC nprev[dhMO], UC result[proSZ])
{
	if (!head || !nprev || !result)
		return BAD_PARAMETERS;

	UC *p = list_getByData(head, (UC*) (&(nprev[0])), relocate);

	if (p)
	{
		relocate_index[curOV] = relocate_count[curOV];
		relocate_count[curOV] = 0;
		memcpy((UC*) (&(result[0])), p, proSZ);
		return true;
	}
	else
	{
		relocate_index[curOV] = 0;
		relocate_count[curOV] = 0;
		return false;
	}
}

errCO alist_purge(Alist *head, UC nprev[dhMO])
{
	return 
		list_deleteByData(head, (UC*) (&(nprev[0])), purge); 
}

void alist_dropAlist(Alist *head)
{
	while (list_deleteByIndex(head, 0) == SUCCESS)
		continue; 
}

errCO alist_destroy(Alist *head)
{ return list_destroy(head); }

void alist_print(Alist *head)
{
	printf("Attestation list:\n");
 	list_print(head);
}