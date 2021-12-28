#ifndef _FLIST_H
#define _FLIST_H

#include "../ta.h"

struct s_list;
typedef struct s_list Flist;

/*

Create new finger list

Input:
	UC plate[plaSZ] = plate of node creating the list

Output:
	pointer to list created

*/
Flist* flist_create(UC plate[plaSZ]);

/*

Destroy finger list

Input:
	Flist *head = pointer to list

Output:
	SUCCESS = list destroyed
	BAD_PARAMETERS = bad input parameters
	
*/
errCO flist_destroy(Flist *head);

/*

Return number of fingers inside the list

Input:
	Flist *head = pointer to list

Output:
	number of fingers	

*/
int flist_count(Flist *head);

/*

Insert new element in finger list

Input:
	Flist *head = finger list pointer
	UC plate[plaSZ] = plate to insert

Output:
	SUCCESS = element added
	BAD_PARAMETERS = bad input parameters

*/
errCO flist_insert(Flist *head, UC plate[plaSZ]);

/*

Get finger based on position

Input:
	Flist *head = finger list pointer
	UC pos[finSZ] = position to search for

Output:
	pointer to finger found, NULL otherwise

*/
UC* flist_getPlateByPos(Flist *head, UC pos[finSZ]);

/*

Get finger based on index

Input:
	Flist *head = finger list pointer 
	int i = index to get

Output:
	pointer to finger found, NULL otherwise

*/
UC* flist_getPlateByIndex(Flist *head, int index);

/*

Find closest preceding plate inside finger list

Input:
	Flist *head = finger list pointer 
	UC id[dhMO] = checking ID
	UC result[plaSZ] = resulting plate

Output:
	true = closest preceding found
	false = closest preceding not found

*/
bool flist_closestPredecessor(
	Flist *head, UC id[dhMO], UC result[plaSZ]);

/*

Print finger list

Input:
	Flist *head = finger list to print

*/
void flist_print(Flist *head);

#endif