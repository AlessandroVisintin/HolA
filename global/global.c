#include "global.h"

static bool ishbe(void)
{
	UI a = 1;
	UC *b = (UC*) &a;

	if (b[0] == 0)
		return true;
	return false;
}

static UI swap32(UI i)
{
	if (!ishbe())
		i = ((i>>24)&0xff) | // 3 to 0
			((i<<8)&0xff0000) | // 1 to 2
			((i>>8)&0xff00) | // 2 to 1
			((i<<24)&0xff000000); // 0 to 3

	return i;
}

void serialize(UI integer, UC *bytes)
{	
	if (!ishbe())
		integer = swap32(integer);

	UC *ptr = (UC*) &integer;
	memcpy(bytes, ptr, 4);
}

UI deserialize(UC *bytes, bool host)
{
	UI integer;
	memcpy(&integer, bytes, 4);

	if (host && !ishbe())
  		return swap32(integer);

  	return integer;
}

bool memisnull(UC *ptr, int len)
{
	if (!ptr)
		return false;

	for (int i = 0; i < len; i++)
	{
		if (ptr[i] != 0)
			return false;
	}
	return true;
}

void memprint(UC *ptr, int len)
{	
	if (!ptr || len < 1)
		return;

	int i = 0;
	int j = 0;
	while (len > 0)
	{
		printf("%u\t", ptr[j]);
		len -= 1;
		i += 1;
		j += 1;

		if (i == 16)
		{
			i = 0;
			printf("\n");
		}
	}
	
	if (i > 0)
		printf("\n");
}