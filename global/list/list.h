#ifndef _LIST_H
#define _LIST_H

#include "../global.h"

struct s_list;
typedef struct s_list List;

// comparing function
typedef bool (*fCMP)(UC*, UC*);

// modifying function
typedef void (*fMOD)(UC*);

/*

Create new list

Output:
	pointer to list created

*/
List* list_create(void);

/*

Delete list

Input:
	List *h = list to delete

Output:
	SUCCESS = list destroyed
	BAD_PARAMETERS = bad input parameters

*/
errCO list_destroy(List *h);

/*

Return number of elements inside the list

Input:
	List *h = list pointer

Output:
	number of elements

*/
int list_getCount(List *h);

/*

Add data at a given index

Input:
	List *h = list in which to add 
	UC *d = pointer to data
	int l = data length 
	int i = index in which to add

Output:
	success = element added
	wrongPARAMETERS = bad input parameters
	outBOUND = given index is outside boundaries

*/
errCO list_addByIndex(List *h, UC *d, int l, int i);

/*

Add data based on comparing functions

Input:
	List *h = list in which to insert 
	UC *d = pointer to data
	int l = data length
	fCMP f = insertion point check 
	fCMP c = overwrite check

Output:
	SUCCESS = element added
	BAD_PARAMETERS = bad input parameters

*/
errCO list_addByData(List *h, UC *d, int l, fCMP f, fCMP c);

/*

Get data based on index

Input:
	List *h = list in which to search 
	int i = index to find

Output:
	pointer to data found, NULL otherwise

*/
UC* list_getByIndex(List *h, int i);

/*

Get data based on comparing function

Input:
	List *h = list in which to search 
	UC *d = pointer to data to find
	fCMP f = function for comparing data

Output:
	pointer to data found, NULL otherwise

*/
UC* list_getByData(List *h, UC *d, fCMP f);

/*

Modify data based on function

Input:
	List *h = list pointer
	fCMP f = comparing function
	fMOD m = modifying function

Output:
	SUCCESS = data successfully modified
	BAD_PARAMETERS = bad input parameters

*/
errCO list_modifyData(List* h, fCMP f, fMOD m);

/*

Extract data based on comparing function

Input:
	List *h = list in which to search 
	UC *d = pointer to data to compare
	fCMP f = comparing function
	UC *r = data extracted

Output:
	SUCCESS = data found and extracted
	NOT_FOUND = data not found
	BAD_PARAMETERS = bad input parameters

*/
errCO list_extractByData(List *h, UC *d, fCMP f, UC *r);

/*

Delete data based on position

Input:
	List *h = list in which to delete
	int i = index to delete
	
Output:
	SUCCESS = element deleted
	BAD_PARAMETERS = bad input parameters
	OUT_BOUND = given index is outside boundary
	EMPTY_LIST = list empty

*/
errCO list_deleteByIndex(List *h, int i);

/*

Delete data based on data

Input:
	List *h = list in which to delete 
	UC* d = data to compare
	fCMP f = comparing function
	
Output:
	SUCCESS = all positive elements deleted
	BAD_PARAMETERS = bad input parameters
	EMPTY_LIST = list is empty

*/
errCO list_deleteByData(List *h, UC *d, fCMP f);

/*

Print pointed list

Input:
	List *h = list to print

*/
void list_print(List *h);

#endif