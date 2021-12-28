#include "list.h"

bool insertion_check(UC* list, UC* data)
{ return (memcmp(list, data, 1) >= 0); }

bool overwrite_check(UC* list, UC* data)
{ return (memcmp(list, data, 1) == 0); }

bool modify_check(UC* list, UC *data)
{ return true; }

void modify_exe(UC* list)
{ list[0] = 255; }

int main(int argc, char const *argv[])
{	
	printf("Creating new list...\n");
	List *ptr = list_create();

	printf("Trying to destroy NULL pointer... ");
	if (list_destroy(NULL) == SUCCESS)
		printf("not detected!?!\n");
	else
		printf("ok detected!\n");

	int len = 16;
	UC data[len];
	
	printf("Trying to add first element...\n");
	
	memset(data, 9, len);
	list_addByIndex(ptr, data, len, 0);

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to delete first element...\n");
	list_deleteByIndex(ptr, 0);

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to add 7 elements...\n");
	for (int i = 0; i < 7; i++)
	{
		memset(data, i, len);
		list_addByIndex(ptr, data, len, 0);
	}

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to delete first, middle and last element...\n");
	list_deleteByIndex(ptr, 0);
	list_deleteByIndex(ptr, 3);
	list_deleteByIndex(ptr, -1);

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to insert 5 element\n");
	for (int i = 10; i < 15; i++)
	{
		memset(data, i, len);
		list_addByData(ptr, data, len, insertion_check, overwrite_check);
	}

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to insert in the middle...\n");
	memset(data, 9, len);
	list_addByData(ptr, data, len, insertion_check, overwrite_check);
	memset(data, 6, len);
	list_addByData(ptr, data, len, insertion_check, overwrite_check);

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to insert with overwrite...\n");
	memset(data, 8, len);
	data[0] = 9;
	list_addByData(ptr, data, len, insertion_check, overwrite_check);

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to delete based on data\n");
	data[0] = 5;
	data[1] = 9;
	data[2] = 14;
	list_deleteByData(ptr, &data[0], overwrite_check);
	list_deleteByData(ptr, &data[1], overwrite_check);
	list_deleteByData(ptr, &data[2], overwrite_check);

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to get by index...\n");
	memprint(list_getByIndex(ptr, 3), 16);
	printf("Last one...\n");
	memprint(list_getByIndex(ptr, -1), 16);

	printf("Trying to get by data...\n");
	data[0] = 13;
	memprint(list_getByData(ptr, &data[0], overwrite_check), 16);

	printf("Trying to modify data\n");
	list_modifyData(ptr, modify_check, modify_exe);

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to extract by data...\n");
	data[0] = 255;
	UC d[16];

	if (list_extractByData(ptr, &data[0], overwrite_check, d) == NOT_FOUND)
		printf("First Byte 13 not found!?!\n");
	else
	{
		printf("Found!\n");
		memprint(d, 16);
	}

	if (list_extractByData(ptr, &data[1], overwrite_check, d) == NOT_FOUND)
		printf("First Byte 9 not found!?!\n");
	else
	{
		printf("Found!\n");
		memprint(d, 16);
	}

	printf("Printing list...\n");
	list_print(ptr);

	printf("Trying to destroy ACTUAL pointer... ");
	if (list_destroy(ptr) == SUCCESS)
		printf("ok done!\n");
	else
		printf("not done!?!\n");

	return 0;
}