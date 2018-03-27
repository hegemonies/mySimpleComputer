// #include "bigchars.h"
// #include "myReadKey.h"
#include "helper.h"

#include <signal.h>

void sighandler(int sig)
{
	printf("Получен сигнал - %d\n", sig);
}

int main()
{
	sc_memoryInit();
	sc_regInit();
	initNumberCell();
	interface(1, 0, 1, 1, 1, 1, 1, 1, 1);

	enum keys key;
	enum way w;
	w = way_DEFAULT;
	selectCellMemory(w);

	while (1) {
		interface(0, 0, 0, 1, 1, 1, 1, 0, 0);
		rk_readKey(&key);
		if (key == 'q') {
			break;
		}

		int value;
		sc_regGet(CI, &value);
		mt_gotoXY(27, 1);
		printf("value = %d\n", value);
		if (value == 0) {
			continue;
		}
		
		if (key == UP) {
			w = way_UP;
			selectCellMemory(w);
		}
		if (key == LEFT) {
			w = way_LEFT;
			selectCellMemory(w);
		}
		if (key == DOWN) {
			w = way_DOWN;
			selectCellMemory(w);
		}
		if (key == RIGHT) {
			w = way_RIGHT;
			selectCellMemory(w);
		}
		

		if (key >= 0 && key < 10) {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += key;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'a') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 10;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'b') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 11;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'c') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 12;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'd') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 13;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'e') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 14;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'f') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 15;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
		}


	}

	mt_gotoXY(26, 1);

	return 0;
}