// #include "bigchars.h"
// #include "myReadKey.h"
#include "helper.h"

int main()
{
	sc_memoryInit();
	initNumberCell();
	interface(1, 0, 1, 1, 1, 1, 1, 1, 1);

	enum keys key;
	enum way w;
	w = way_DEFAULT;
	selectCellMemory(w);

	while (1) {
		rk_readKey(&key);
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
		if (key == 'q') {
			break;
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