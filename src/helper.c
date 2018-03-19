#include "helper.h"

int getBannerFromFile(char *namefile, int *count_lines)
{
	FILE *in = fopen(namefile, "r");

	if (!in) {
		return 1;
	}

	char *buf = NULL;
	size_t len = 0;

	*count_lines = 0;

	while (getline(&buf, &len, in) != -1) {
		(*count_lines)++;
	}

	fseek(in, 0, SEEK_SET);
	banner = calloc(*count_lines, sizeof(char*));

	for (int i = 0; getline(&buf, &len, in) != -1; i++) {
		banner[i] = calloc(strlen(buf) + 2, sizeof(char));
		strcpy(banner[i], buf);
	}

	fclose(in);

	return 0;
}

void printBanner(int count_lines)
{
	enum colors fg = white;
	enum colors bg = black;
	printf("\E(0");	
	mt_ssetfgcolor(fg);
	mt_ssetbgcolor(bg);

	for (int i = 0; i < count_lines; i++) {
		printf("%s", banner[i]);
		fflush(stdout);
	}

	printf("\E(B");
	mt_stopcolor();
}

int changeSizeTerm()
{
	int size_console_x;
	int size_console_y;

	if (mt_getscreensize(&size_console_y, &size_console_x) != 0) {
		printf("Error\n");
		return 1;
	}

	if (size_console_x < 85 || size_console_y < 46) {
		printf("\033[8;45;84t");
	}

	mt_clrscr(); 
	mt_gotoXY(1, 1);

	return 0;
}

void load()
{
	mt_gotoXY(25, 1);
	printf("loading");

	for (int i = 0; i < 3; i++) {
		sleep(1);
		printf(".");
		fflush(stdout);
	}

	printf("All good. Start\n");

	sleep(2);
	mt_clrscr();
}

int printMemory()
{
	if (bc_box(1, 1, 63, 13) != 0) {
		return 1;
	}

	mt_gotoXY(1, 28);
	printf("Memory");
	sm_printMemory(2, 2);


	return 0;
}

int printAccumalte()
{
	if (bc_box(63, 1, 84, 4) != 0) {
		return 1;
	}

	mt_gotoXY(1, 68);
	printf("Accumulator");
	mt_gotoXY(2, 70);
	printf("+0000");

	return 0;
}

int printInstCounter()
{
	if (bc_box(63, 4, 84, 7) != 0) {
		return 1;
	}

	mt_gotoXY(4, 64);
	printf("InstructionCounter");
	mt_gotoXY(5, 70);
	printf("+0000");

	return 0;
}

int printOperation()
{
	if (bc_box(63, 7, 84, 10) != 0) {
		return 1;
	}

	mt_gotoXY(7, 69);
	printf("Operation");
	mt_gotoXY(8, 69);
	printf("+00 : 00");

	return 0;
}

int printFlags()
{
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

	return 0;
}

int printBoxBC()
{
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

	return 0;
}

int printHelpBox()
{
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

int interface(int size, int ban, int mem, int acc, int insCoun, int oper, int fl, int bc, int h)
{
	rk_mytermsave();
	if (size) {
		changeSizeTerm();
	}

	if (ban) {
		int count_lines = 0;
		if (getBannerFromFile("banner.txt", &count_lines)) {
			return 1;
		}

		printBanner(count_lines);

		load();
	}

	if (mem) {
		if (printMemory()) {
			return 1;
		}
	}
	
	if (acc) {
		if (printAccumalte()) {
			return 1;
		}
	}
	
	if (insCoun) {
		if (printInstCounter()) {
			return 1;
		}
	}
	
	if (oper) {
		if (printOperation()) {
			return 1;
		}
	}
	
	if (fl) {
		if (printFlags()) {
			return 1;
		}
	}
	
	if (bc) {
		if (printBoxBC()) {
			return 1;
		}
	}
	
	if (h) {
		if (printHelpBox()) {
			return 1;
		}
	}

	mt_gotoXY(26, 1);
	fflush(stdout);

	return 0;
}

int intToHex(int number, char *str)
{
	printf("number = %d\n", number);
	if (!str || number > 127 || number < 0) {
		return 1;
	}

	int tmp;
	int i;

	for (i = 0; number > 16; i++) {
		tmp = number / 16;
		number %= 16;
		if (tmp == 10) {
			str[i] = 'A';
		} else if (tmp == 11) {
			str[i] = 'B';
		} else if (tmp == 12) {
			str[i] = 'C';
		} else if (tmp == 13) {
			str[i] = 'D';
		} else if (tmp == 14) {
			str[i] = 'E';
		} else if (tmp == 15) {
			str[i] = 'F';
		} else {
			str[i] = tmp + 48;
		}
	}

	tmp = number;

	if (tmp == 10) {
			str[i] = 'A';
		} else if (tmp == 11) {
			str[i] = 'B';
		} else if (tmp == 12) {
			str[i] = 'C';
		} else if (tmp == 13) {
			str[i] = 'D';
		} else if (tmp == 14) {
			str[i] = 'E';
		} else if (tmp == 15) {
			str[i] = 'F';
		} else {
			str[i] = tmp + 48;
		}

	return 0;
}

void initNumberCell()
{
	cell = 0;
}

void printCell()
{
	if (ptr_str[cell] >= 0) {
		printf("+%04x", ptr_str[cell]);
		fflush(stdout);
	} else {
		printf("-%04x", ptr_str[cell]);
		fflush(stdout);
	}
}

// void printBigCharInBox()
// {
// 	int bigChars[10] = { 0 };

// 	if (ptr_str[cell] >= 0) {
// 		bigChars = bc_Plus;
// 	} else {
// 		bigChars = bc_Minus;
// 	}

// 	for (int i = 1; i < 5; i++) {
// 		int tmp = (ptr_str[cell] % (10 * i)) / (10 * (i - 1));
// 		if (tmp == 0) {
// 			bigChars + (i * 2) = bc_Null;
// 		} else if (tmp == 1) {
// 			bigChars + (i * 2) = bc_One;
// 		} else if (tmp == 2) {
// 			bigChars + (i * 2) = bc_Two;
// 		} else if (tmp == 3) {
// 			bigChars + (i * 2) = bc_Three;
// 		} else if (tmp == 4) {
// 			bigChars + (i * 2) = bc_Four;
// 		} else if (tmp == 5) {
// 			bigChars + (i * 2) = bc_Five;
// 		} else if (tmp == 6) {
// 			bigChars + (i * 2) = bc_Six;
// 		} else if (tmp == 7) {
// 			bigChars + (i * 2) = bc_Seven;
// 		} else if (tmp == 8) {
// 			bigChars + (i * 2) = bc_Eight;
// 		} else if (tmp == 9) {
// 			bigChars + (i * 2) = bc_Nine;
// 		}
// 	}


// }

void selectCellMemory(enum way w)
{
	enum colors color = red;
	// mt_ssetbgcolor(color);
	if (w == way_RIGHT) {
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_stopcolor();

		printCell();

		if (cell < 99) {
			cell++;
		}

		color = red;
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_ssetbgcolor(color);
		
		printCell();
	}

	if (w == way_LEFT) {
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_stopcolor();
		
		printCell();

		if (cell > 0) {
			cell--;
		}

		color = red;
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_ssetbgcolor(color);
		
		printCell();
	}

	if (w == way_UP) {
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_stopcolor();
		
		printCell();

		if (cell > 9) {
			cell -= 10;
		}

		color = red;
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_ssetbgcolor(color);
		
		printCell();
	}

	if (w == way_DOWN) {
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_stopcolor();
		
		printCell();

		if (cell < 90) {
			cell += 10;
		}

		color = red;
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_ssetbgcolor(color);
		
		printCell();
	}

	if (w == way_DEFAULT) {
		color = red;
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_ssetbgcolor(color);
		printCell();
	}

	mt_stopcolor();
}