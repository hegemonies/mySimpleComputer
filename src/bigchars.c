#include "bigchars.h"

int bc_printA(char *str)
{
	if (!str) {
		return 1;
	}

	printf("\E(0%s\E(B", str); 

	return 0; 
}

int bc_box(int x1, int y1, int x2, int y2)
{
	if (x1 > x2 || y1 > y2) {
		return 1;
	}

	int size_x = 0;
	int size_y = 0;

	if (mt_getscreensize(&size_y, &size_x) == 1) {
		return 1;
	}

	if (!size_x || !size_y) {
		return 1;
	}

	if (x1 < 1 || x2 > size_x || y1 < 1 || y2 > size_y) {
		return 1;
	}

	mt_gotoXY(y1, x1);

	int x = x2 - x1 + 1;
	int y = y2 - y1;

	printf("\E(0");

	for (int i = 0; i < y; i++) {
		if (i == 0) {
			printf("%c", bc_cornerUpLeft);

			for (int j = 1; j < x - 2; j++) {
				printf("%c", bc_horizontaleLine);
			}

			printf("%c", bc_cornerUpRight);

			printf("\n");
			continue;
		}

		mt_gotoXY(y1 + i, x1);

		printf("%c", bc_verticalLine);

		for (int j = 1; j < x - 2; j++) {
			printf("%c", ' ');
		}

		printf("%c", bc_verticalLine);
		printf("\n");

		if (i == y - 1) { 
			mt_gotoXY(y1 + i, x1);

			printf("%c", bc_cornerDownLeft);

			for (int j = 1; j < x - 2; j++) {
				printf("%c", bc_horizontaleLine);
			}

			printf("%c", bc_cornerDownRight);

			printf("\n");
		}
	}


	printf("\E(B");

	return 0;
}

int bc_printbigchar(int *a, int x, int y, enum colors fg, enum colors bg)
{
	if (!a || 
		x < 1 || y < 1 || 
		fg > 7 || fg < 0 || 
		bg > 7 || bg < 0) {
		return 1;
	}

	int size_x = 0;
	int size_y = 0;
	if (mt_getscreensize(&size_y, &size_x) == 1) {
		return 1;
	}

	if (x > size_x || y > size_y) {
		return 1;
	}

	printf("\E(0");
	mt_ssetfgcolor(fg);
	mt_ssetbgcolor(bg);

	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < 4; i++) {
			mt_gotoXY(y + i + k * 4, x);
			for (int j = 0; j < 8; j++) {
				if ((a[k] >> (i * 8 + j)) & 1) {
					//printf("%c", bc_shadedCell);
					printf("f");
				} else {
					printf("%c", ' ');
				}
			}
		}
	}
	
	printf("\E(B");

	mt_stopcolor();

	return 0;
}

int bc_setbigcharpos(int *big, int x, int y, int value)
{
	if (!big || x < 0 || x > 7 || y < 0 || y > 7 || value < 0 || value > 1) {
		return 1;
	}

	y--;
	x--;

	if (y < 4) {
		if (value) {
			big[0] |= 1 << (y * 8 + x);
		} else {
			big[0] = ~(1 << (y * 8 + x));
		}
	} else {
		if (value) {
			big[1] |= 1 << (y * 8 + x);
		} else {
			big[1] = ~(1 << (y * 8 + x));
		}
	}
	

	return 0;
}

int bc_getbigcharpos(int *big, int x, int y, int *value)
{
	if (!big || !value || x < 0 || x > 7 || y < 0 || y > 7) {
		return 1;
	}

	x--;
	y--;

	if (y < 4) {
		*value = (big[0] >> (y * 8 + x)) & 1;
	} else {
		*value = (big[1] >> (y * 8 + x)) & 1;
	}

	return 0;
}

int bc_bigcharwrite(int fd, int *big, int count)
{
	if (!big || count < 0 || fd < 0) {
		return 1;
	}

	for (; count > 0; count--) {
		if (write(fd, &big[0], sizeof(int)) < 0) {
			return 1;
		}

		if (write(fd, &big[1], sizeof(int)) < 0) {
			return 1;
		}
	}


	return 0;
}

int bc_bigcharread(int fd, int *big, int need_count, int *count)
{
	if (!big || !count || fd < 0) {
		return 1;
	}

	for (; need_count > 0; need_count--) {
		if (read(fd, &big[0], sizeof(int)) < 0) {
			*count = 0;
			return 1;
		}

		if (read(fd, &big[1], sizeof(int)) < 0) {
			*count = 0;
			return 1;
		}
	}


	return 0;
}

int mt_printPG()
{
	int size_console_x;
	int size_console_y;

	if (mt_getscreensize(&size_console_y, &size_console_x) != 0) {
		printf("ERror\n");
		return 1;
	}

	if (size_console_x < 84 || size_console_y < 24) {
		printf("The screen size is too small. (need 84x24)\n");
		return 1;
	}

	if (bc_box(1, 1, 63, 13) != 0) {
		return 1;
	}
	mt_gotoXY(1, 28);
	printf("Memory");
	sm_printMemory(2, 2);

	if (bc_box(63, 1, 84, 4) != 0) {
		return 1;
	}
	mt_gotoXY(1, 68);
	printf("Accumulator");
	mt_gotoXY(2, 70);
	printf("+0000");

	if (bc_box(63, 4, 84, 7) != 0) {
		return 1;
	}
	mt_gotoXY(4, 64);
	printf("InstructionCounter");
	mt_gotoXY(5, 70);
	printf("+0000");

	if (bc_box(63, 7, 84, 10) != 0) {
		return 1;
	}
	mt_gotoXY(7, 69);
	printf("Operation");
	mt_gotoXY(8, 69);
	printf("+00 : 00");

	if (bc_box(63, 10, 84, 13) != 0) {
		return 1;
	}
	mt_gotoXY(10, 71);
	printf("Flags");
	mt_gotoXY(11, 64);
	int _OD;
	sc_regGet(OD, &_OD);
	int _DE;
	sc_regGet(OD, &_DE);
	int _EG;
	sc_regGet(OD, &_EG);
	int _CI;
	sc_regGet(OD, &_CI);
	int _IC;
	sc_regGet(OD, &_IC);
	printf("D-%d E-%d G-%d I-%d C-%d", _OD, _DE >> 1, _EG >> 2, _CI >> 3, _IC >> 4);

	if (bc_box(1, 13, 63, 23) != 0) {
		return 1;
	}
	int chr[2] = bc_Plus;
	bc_printbigchar(chr, 4, 14, 4, 7);
	int chr1[2] = bc_D;
	bc_printbigchar(chr1, 19, 14, 4, 7);
	int chr2[2] = bc_E;
	bc_printbigchar(chr2, 30, 14, 4, 7);
	int chr3[2] = bc_A;
	bc_printbigchar(chr3, 41, 14, 4, 7);
	int chr4[2] = bc_D;
	bc_printbigchar(chr4, 52, 14, 4, 7);

	if (bc_box(63, 13, 84, 23) != 0) {
		return 1;
	}
	mt_gotoXY(13, 64);
	printf("Keys:");
	mt_gotoXY(14, 64);
	printf("l - load");
	mt_gotoXY(14, 64);
	printf("s - save");
	mt_gotoXY(15, 64);
	printf("r - run");
	mt_gotoXY(16, 64);
	printf("t - step");
	mt_gotoXY(17, 64);
	printf("i - reset");
	mt_gotoXY(18, 64);
	printf("F5 - accumulator");
	mt_gotoXY(19, 64);
	printf("F6 - instrCounter");


	return 0;
}