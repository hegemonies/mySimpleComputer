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

		if (value == 0) {
			continue;
		}
		
		if (key == UP) {
			w = way_UP;
			selectCellMemory(w);
			printf("ptr_str[%d] = %d\n", cell, ptr_str[cell]);
			continue;
		}
		if (key == LEFT) {
			w = way_LEFT;
			selectCellMemory(w);
			printf("ptr_str[%d] = %d\n", cell, ptr_str[cell]);
			continue;
		}
		if (key == DOWN) {
			w = way_DOWN;
			selectCellMemory(w);
			printf("ptr_str[%d] = %d\n", cell, ptr_str[cell]);
			continue;
		}
		if (key == RIGHT) {
			w = way_RIGHT;
			selectCellMemory(w);
			printf("ptr_str[%d] = %d\n", cell, ptr_str[cell]);
			continue;
		}
		

		if (key >= 0 && key < 10) {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += key;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'a') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 10;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'b') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 11;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'c') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 12;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'd') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 13;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'e') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 14;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'f') {
			if (ptr_str[cell] < 65535) {
				ptr_str[cell] += 15;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == MINUS) {
			if (ptr_str[cell] > 0) {
				ptr_str[cell] -= ptr_str[cell] * 2;
				w = way_DEFAULT;
				selectCellMemory(w);
				mt_gotoXY(29, 1);
				printf("ptr_str[%d] = %d\n", cell, ptr_str[cell]);
			}
			continue;
		}
		if (key == PLUS) {
			if (ptr_str[cell] < 0) {
				ptr_str[cell] += ptr_str[cell] * 2;
				w = way_DEFAULT;
				selectCellMemory(w);
				mt_gotoXY(29, 1);
				printf("ptr_str[%d] = %d\n", cell, ptr_str[cell]);
			}
			continue;
		}
	}

	mt_gotoXY(26, 1);

	return 0;
}