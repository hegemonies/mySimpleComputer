// #include "bigchars.h"
#include "myReadKey.h"

int main()
{
	// mt_printPG();

	// mt_gotoXY(26, 1);

	enum keys key;
	rk_readKey(&key);	
	printf("key = %d\n", key);

	return 0;
}
