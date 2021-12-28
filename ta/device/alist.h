#ifndef _ALIST_H
#define _ALIST_H

#include "../ta.h"

struct s_list;
typedef struct s_list Alist;

/*

Create new attestation list

Input:
	UC plate[plaSZ] = plate of node creating the list

Output:
	pointer to list created

*/
Alist* alist_create(UC plate[plaSZ]);

/*

Destroy attestation list

Input:
	Alist *head = pointer to list created

Output:
	SUCCESS = list destroyed
	BAD_PARAMETERS = bad input parameters

*/
errCO alist_destroy(Alist *head);

/*

Return number of proofs inside the list

Input:
	Alist *head = pointer to list

Output:
	number of proofs	

*/
int alist_count(Alist *head);

/*

Insert new element in attestation list

Input:
	Alist *head = attestation list pointer
	UC pro[proSZ] = proof to insert

Output:
	SUCCESS = element added
	BAD_PARAMETERS = bad input parameters

*/
errCO alist_insert(Alist *head, UC pro[proSZ]);

/*

Find proof inside attestation list

Input:
	Alist *head = attestation list pointer
	UC uid[dhMO] = uid to search
	UC result[dhMO] = resulting proof
	bool flag = set flag if true

Output:
	true = uid found and result saved in result
	false = uid not found

*/
bool alist_findProof(
	Alist* head, UC uid[dhMO], UC result[proSZ]);

/*

Relocate proof from attestation list

Input:
	Alist *head = attestation list pointer
	UC nprev[dhMO] = new predecessor ID
	UC result[proSZ] = proof to relocate, if any

Output:
	true = proof found and saved in result
	false = proof not found

*/
bool alist_relocateProof(
	Alist *head, UC nprev[dhMO], UC result[proSZ]);

/*

Delete all attestation with true flag

Input:
	Alist *head = attestation list pointer
	UC nprev[dhMO] = new predecessor ID

Output:
	SUCCESS = operation completed
	BAD_PARAMETERS = bad input parameters
	EMPTY_LIST = list empty

*/
errCO alist_purge(Alist *head, UC nprev[dhMO]);

/*

Drop all contained proofs (for testing purposes)

Input:
	Alist *head = attestation list pointer

*/
void alist_dropAlist(Alist* head);

/*

Print attestation list

Input:
	Alist *head = attestation list to print

*/
void alist_print(Alist *head);

#endif