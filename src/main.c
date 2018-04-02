// #include "bigchars.h"
// #include "myReadKey.h"
#include "helper.h"

#include <signal.h>

void sighandler(int sig)
{
	printf("Получен сигнал - %d\n", sig);
}

int presentProgram()
{
	mt_gotoXY(2,2);
	mt_stopcolor();
	printCell();

	int prepareNumCell[20] = {95, 84, 86, 73, 77, 62, 68, 51, 59, 41, 49, 31, 39, 22, 28, 33, 37, 44, 46, 55};
	// selectCellMemoryByNumber(0);
	for (int i = 0; i < 20; i++) {
		interface(0, 0, 0, 0, 1, 0, 0, 0, 0);
		selectCellMemoryByNumber(prepareNumCell[i]);
		ptr_str[cell]++;
		selectCellMemoryByNumber(prepareNumCell[i]);
		instCount++;
		if (i > 0 && i < 19) {
			i++;
			interface(0, 0, 0, 0, 1, 0, 0, 0, 0);
			selectCellMemoryByNumber(prepareNumCell[i]);
			ptr_str[cell]++;
			selectCellMemoryByNumber(prepareNumCell[i]);
			instCount++;
		}
		sleep(1);
	}

	return 0;
}

int main()
{
	sc_memoryInit();
	sc_regInit();
	initNumberCell();
	initInstCounter();

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

		int CI_value;
		sc_regGet(CI, &CI_value);
		if (CI_value == 0) {
			continue;
		}
		
		if (key == UP) {
			w = way_UP;
			selectCellMemory(w);
			continue;
		}
		if (key == LEFT) {
			w = way_LEFT;
			selectCellMemory(w);
			continue;
		}
		if (key == DOWN) {
			w = way_DOWN;
			selectCellMemory(w);
			continue;
		}
		if (key == RIGHT) {
			w = way_RIGHT;
			selectCellMemory(w);
			continue;
		}
		

		if (key >= 0 && key < 10) {
			if (ptr_str[cell] > -65534 && ptr_str[cell] < 65535) {
				ptr_str[cell] += key;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'a') {
			if (ptr_str[cell] > -65534 && ptr_str[cell] < 65535) {
				ptr_str[cell] += 10;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'b') {
			if (ptr_str[cell] > -65534 && ptr_str[cell] < 65535) {
				ptr_str[cell] += 11;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'c') {
			if (ptr_str[cell] > -65534 && ptr_str[cell] < 65535) {
				ptr_str[cell] += 12;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'd') {
			if (ptr_str[cell] > -65534 && ptr_str[cell] < 65535) {
				ptr_str[cell] += 13;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'e') {
			if (ptr_str[cell] > -65534 && ptr_str[cell] < 65535) {
				ptr_str[cell] += 14;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'f') {
			if (ptr_str[cell] > -65534 && ptr_str[cell] < 65535) {
				ptr_str[cell] += 15;
			}
			w = way_DEFAULT;
			selectCellMemory(w);
			continue;
		}
		if (key == 'p') {
			sc_regSet(CI, 0);
			presentProgram();
			sc_regSet(CI, 1);
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