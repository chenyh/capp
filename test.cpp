#include <stdio.h>
#include "testsize.h"

int main()
{
	int value = 0xf3ff0000;
	unsigned int uvalue = value;
	printf("%d, %u", value, uvalue);

	testsize::test();
	return 1;
}
