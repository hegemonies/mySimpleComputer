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

	// printf("%d :: %d\n", 15 / 10, 15 % 10);

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
			ptr_str[cell] += key;
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'a') {
			ptr_str[cell] += 10;
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'b') {
			ptr_str[cell] += 11;
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'c') {
			ptr_str[cell] += 12;
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'd') {
			ptr_str[cell] += 13;
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'e') {
			ptr_str[cell] += 14;
			w = way_DEFAULT;
			selectCellMemory(w);
		}
		if (key == 'f') {
			ptr_str[cell] += 15;
			w = way_DEFAULT;
			selectCellMemory(w);
		}
	}

	mt_gotoXY(26, 1);


	// while (1) {
	// 	rk_readKey(&key);
	// 	if (key == UP)
	// 		printf("PRESS UP\n");
	// 	else if (key == DOWN)
	// 		printf("PRESS DOWN\n");
	// 	else if (key == RIGHT)
	// 		printf("PRESS RIGHT\n");
	// 	else if (key == LEFT)
	// 		printf("PRESS LEFT\n");
	// 	else if (key == F5)
	// 		printf("PRESS F5\n");
	// 	else if (key == OTHER)
	// 		printf("PRESS OTHER\n");
	// 	else if (key == F6)
	// 	    printf("PRESS F6\n");
	// 	else if (key == 'l')
	// 		printf("PRESS l\n");
	// 	else if (key == 's')
	// 		printf("PRESS s\n");
	// 	else if (key == 'r')
	// 		printf("PRESS r\n");
	// 	else if (key == 't')
	// 		printf("PRESS t\n");
	// 	else if (key == 'i')
	// 		printf("PRESS i\n");
	// 	else if (key == MINUS)
	// 		printf("PRESS MINUS\n");
	// 	else if (key == PLUS)
	// 		printf("PRESS PLUS\n");
	// 	else if (key == 'a') 
	// 		printf("PRESS a\n");
	// 	else if (key == 'q') {
	// 		printf("PRESS q\n");
	// 		break;
	// 	} else
	// 		printf("%d\n", key);
	// }

	// printf("key = %d\n", key);

	return 0;
}