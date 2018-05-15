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
	if (cell != 55) {
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_stopcolor();
		printCell();
	}

	int prepareNumCell[20] = {95, 84, 86, 73, 77, 62, 68, 51, 59, 41, 49, 31, 39, 22, 28, 33, 37, 44, 46, 55};
	// selectCellMemoryByNumber(0);
	for (int i = 0; i < 20; i++) {
		interface(0, 0, 0, 0, 1, 0, 1, 0, 0);
		selectCellMemoryByNumber(prepareNumCell[i]);
		ptr_str[cell] += i * prepareNumCell[i] + 1;
		selectCellMemoryByNumber(prepareNumCell[i]);
		instCount++;
		if (i > 0 && i < 19) {
			usleep(300000);
			i++;
			interface(0, 0, 0, 0, 1, 0, 1, 0, 0);
			selectCellMemoryByNumber(prepareNumCell[i]);
			ptr_str[cell] += i * prepareNumCell[i];
			selectCellMemoryByNumber(prepareNumCell[i]);
			instCount++;
		}
		usleep(300000);
	}

	instCount = 0;

	return 0;
}

void test()
{
	sc_commandEncode(10, 78, &ptr_str[0]); // READ 78
	sc_commandEncode(10, 88, &ptr_str[1]); // READ 88

	sc_commandEncode(20, 79, &ptr_str[2]); // LOAD 79
	sc_commandEncode(30, 89, &ptr_str[3]); // ADD 89
	sc_commandEncode(21, 99, &ptr_str[4]); // STORE 99

	sc_commandEncode(20, 78, &ptr_str[5]); // LOAD 78
	sc_commandEncode(30, 88, &ptr_str[6]); // ADD 88
	sc_commandEncode(21, 98, &ptr_str[7]); // STORE 98

	sc_commandEncode(43, 0, &ptr_str[8]); // HALT
}

int main()
{
	sc_memoryInit();
	sc_regInit();
	initNumberCell();
	initInstCounter();
	initNumStrForLogs();

	test();

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
		if (key == 'p') { //пасхалка
			sc_regSet(CI, 0);
			presentProgram();
			sc_regSet(CI, 1);
			continue;
		}
		if (key == MINUS) {
			if (ptr_str[cell] > 0) {
				ptr_str[cell] *= -1;
				w = way_DEFAULT;
				selectCellMemory(w);
				mt_gotoXY(29, 1);
			}
			continue;
		}
		if (key == PLUS) {
			if (ptr_str[cell] < 0) {
				ptr_str[cell] *= -1;
				w = way_DEFAULT;
				selectCellMemory(w);
				mt_gotoXY(29, 1);
			}
			continue;
 		}
 		if (key == F5) {
 			accum = ptr_str[cell];
 			interface(0, 0, 0, 1, 0, 0, 0, 0, 0);
 			continue;
 		}
 		if (key == 'i') {
 			sc_regInit();
 			sc_memoryInit();
			initInstCounter();
			interface(0, 0, 1, 1, 1, 1, 1, 0, 0);
			cell = 0;
			selectCellMemoryByNumber(cell);
			continue;
 		}
 		if (key == 'l') {
 			mt_gotoXY(26 + numStrForLogs, 1);
 			incrementNumStrForLogs();
 			printf("Enter path to file: ");
 			// numStrForLogs++;
 			char *path = calloc(0, sizeof(char) * 30);
 			scanf("%s", path);
 			load_prog_from_file(path);
 			interface(0, 0, 1, 1, 1, 0, 1, 0, 0);
 			continue;
 		}
 		if (key == 's') {
 			mt_gotoXY(26 + numStrForLogs, 1);
 			incrementNumStrForLogs();
 			printf("Enter path to file: ");
 			// numStrForLogs++;
 			char *path = calloc(0, sizeof(char) * 30);
 			scanf("%s", path);
 			save_prog_in_file(path);
			interface(0, 0, 1, 1, 1, 0, 1, 0, 0);
 			continue;
 		}

 		if (key == 'r') {
			sc_regSet(CI, 0);
 			runtime();
			sc_regSet(CI, 1);
 			continue;
 		}

 		if (key == 't') {
			sc_regSet(CI, 0);
 			runtime_OneStep();
			sc_regSet(CI, 1);
 			continue;
 		}
	}

	mt_gotoXY(26, 1);

	return 0;
}