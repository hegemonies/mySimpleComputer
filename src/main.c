// #include "bigchars.h"
#include "myReadKey.h"
#include "helper.h"

int main()
{
	// interface(1, 1, 1, 1, 1, 1, 1, 1, 1);
	enum keys key;
	while (1) {
		rk_readKey(&key);
		if (key == UP)
			printf("PRESS UP\n");
		else if (key == DOWN)
			printf("PRESS DOWN\n");
		else if (key == RIGHT)
			printf("PRESS RIGHT\n");
		else if (key == LEFT)
			printf("PRESS LEFT\n");
		else if (key == F5)
			printf("PRESS F5\n");
		else if (key == OTHER)
			printf("PRESS OTHER\n");
		else if (key == F6) {
		    printf("PRESS F6\n");
		    break;
		}
	}

	// printf("key = %d\n", key);

	return 0;
}
