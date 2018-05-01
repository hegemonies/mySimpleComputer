#include "helper.h"
#include <stdint.h>

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

	sleep(1);
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

	if (accum < 65536) {
		int tmp = accum;
		if (tmp >= 0) {
			printf("+%04X", tmp);
		} else {
			printf("-%04X", tmp * -1);
		}
	}

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
	printf("+%04d", instCount);

	return 0;
}

int printOperation()
{
	if (bc_box(63, 7, 84, 10) != 0) {
		return 1;
	}

	mt_gotoXY(7, 69);
	printf("Operation");

	int command = ptr_str[cell] >> 7; 
	int operand = ptr_str[cell] & 0b1111111;

	mt_gotoXY(8, 69);
	printf("+ %02d : %02d\n", command, operand);

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
	sc_regGet(DE, &_DE);
	int _EG;
	sc_regGet(EG, &_EG);
	int _CI;
	sc_regGet(CI, &_CI);
	int _IC;
	sc_regGet(IC, &_IC);
	printf("D-%d E-%d G-%d I-%d C-%d", _OD, _DE, _EG, _CI, _IC);

	return 0;
}

int printBoxBC()
{
	if (bc_box(1, 13, 63, 23) != 0) {
		return 1;
	}

	int chr[2];
	chr[0] = bc_Plus(0);
	chr[1] = bc_Plus(1);
	bc_printbigchar(chr, 4, 14, 4, 7);

	chr[0] = bc_D(0);
	chr[1] = bc_D(1);
	bc_printbigchar(chr, 19, 14, 4, 7);

	chr[0] = bc_E(0);
	chr[1] = bc_E(1);
	bc_printbigchar(chr, 30, 14, 4, 7);

	chr[0] = bc_A(0);
	chr[1] = bc_A(1);
	bc_printbigchar(chr, 41, 14, 4, 7);

	chr[0] = bc_D(0);
	chr[1] = bc_D(1);
	bc_printbigchar(chr, 52, 14, 4, 7);


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
	mt_gotoXY(15, 64);
	printf("s - save");
	mt_gotoXY(16, 64);
	printf("r - run");
	mt_gotoXY(17, 64);
	printf("t - step");
	mt_gotoXY(18, 64);
	printf("i - reset");
	mt_gotoXY(19, 64);
	printf("F5 - accumulator");
	mt_gotoXY(20, 64);
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
	if (!str || number >= 65535 || number < 0) {
		return 1;
	}

	for (int i = 0; i < 5; i++) {
		str[i] = 0;
	}

	int remainder;
	int whole = number;
	int i;

	for (i = 0; whole >= 16; i++) {
		remainder = whole % 16;
		whole = whole / 16;
		if (remainder == 10) {
			str[i] = 'A';
		} else if (remainder == 11) {
			str[i] = 'B';
		} else if (remainder == 12) {
			str[i] = 'C';
		} else if (remainder == 13) {
			str[i] = 'D';
		} else if (remainder == 14) {
			str[i] = 'E';
		} else if (remainder == 15) {
			str[i] = 'F';
		} else {
			str[i] = remainder + 48;
		}
	}


	if (whole != 0) {
		if (whole == 10) {
			str[i] = 'A';
		} else if (whole == 11) {
			str[i] = 'B';
		} else if (whole == 12) {
			str[i] = 'C';
		} else if (whole == 13) {
			str[i] = 'D';
		} else if (whole == 14) {
			str[i] = 'E';
		} else if (whole == 15) {
			str[i] = 'F';
		} else {
			str[i] = whole + 48;
		}
	}
		

	return 0;
}

void initNumberCell()
{
	cell = 0;
}

void printCell()
{
	if (ptr_str[cell] < 65536) {
		int tmp = ptr_str[cell];
		if (tmp >= 0) {
			printf("+%04X", tmp);
		} else {
			printf("-%04X", tmp * -1);
		}
	}
	fflush(stdout);
}

int printBigCharInBox()
{
	int bigChars[5][2];

	int tmp_number = ptr_str[cell];

	if (tmp_number >= 0) {
		bigChars[0][0] = bc_Plus(0);
		bigChars[0][1] = bc_Plus(1);
	} else {
		bigChars[0][0] = bc_Minus(0);
		bigChars[0][1] = bc_Minus(1);
		tmp_number = -tmp_number;
	}

	char buf[5];

	if (intToHex(tmp_number, buf)) {
		return 1;
	}

	int j = 4;

	for (int i = 0; i < 4; i++) {
		if (buf[i] == '0') {
			fflush(stdout);
			bigChars[j][0] = bc_Null(0);
			bigChars[j][1] = bc_Null(1);
		} else if (buf[i] == '1') {
			bigChars[j][0] = bc_One(0);
			bigChars[j][1] = bc_One(1);
		} else if (buf[i] == '2') {
			bigChars[j][0] = bc_Two(0);
			bigChars[j][1] = bc_Two(1);
		} else if (buf[i] == '3') {
			bigChars[j][0] = bc_Three(0);
			bigChars[j][1] = bc_Three(1);
		} else if (buf[i] == '4') {
			bigChars[j][0] = bc_Four(0);
			bigChars[j][1] = bc_Four(1);
		} else if (buf[i] == '5') {
			bigChars[j][0] = bc_Five(0);
			bigChars[j][1] = bc_Five(1);
		} else if (buf[i] == '6') {
			bigChars[j][0] = bc_Six(0);
			bigChars[j][1] = bc_Six(1);
		} else if (buf[i] == '7') {
			bigChars[j][0] = bc_Seven(0);
			bigChars[j][1] = bc_Seven(1);
		} else if (buf[i] == '8') {
			bigChars[j][0] = bc_Eight(0);
			bigChars[j][1] = bc_Eight(1);
		} else if (buf[i] == '9') {
			bigChars[j][0] = bc_Nine(0);
			bigChars[j][1] = bc_Nine(1);
		} else if (buf[i] == 'A') {
			bigChars[j][0] = bc_A(0);
			bigChars[j][1] = bc_A(1);
		} else if (buf[i] == 'B') {
			bigChars[j][0] = bc_B(0);
			bigChars[j][1] = bc_B(1);
		} else if (buf[i] == 'C') {
			bigChars[j][0] = bc_C(0);
			bigChars[j][1] = bc_C(1);
		} else if (buf[i] == 'D') {
			bigChars[j][0] = bc_D(0);
			bigChars[j][1] = bc_D(1);
		} else if (buf[i] == 'E') {
			bigChars[j][0] = bc_E(0);
			bigChars[j][1] = bc_E(1);
		} else if (buf[i] == 'F') {
			bigChars[j][0] = bc_F(0);
			bigChars[j][1] = bc_F(1);
		} else {
			bigChars[j][0] = bc_Null(0);
			bigChars[j][1] = bc_Null(1);
		}
		j--;
	}

	int x;
	enum colors fg = red;
	enum colors bg = white;

	for (int i = 0; i < 5; i++) {
		if (i == 0) {
			x = 4;
		} else {
			x = 8;
		}
		bc_printbigchar(bigChars[i], x + i * 11, 14, fg, bg);
	}

	mt_gotoXY(26, 1);
	fflush(stdout);

	return 0;
}

void selectCellMemory(enum way w)
{
	enum colors color = red;

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
		printBigCharInBox();
		printOperation(); // TODO
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
		printBigCharInBox();
		printOperation();
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
		printBigCharInBox();
		printOperation();
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
		printBigCharInBox();
		printOperation();
	}

	if (w == way_DEFAULT) {
		printBigCharInBox();
		color = red;
		mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);
		mt_ssetbgcolor(color);
		printCell();
		printOperation();
	}

	mt_stopcolor();
}

void selectCellMemoryByNumber(int num)
{
	if (num < 0 || num > 99) {
		return;
	}

	cell = num;

	mt_gotoXY((cell / 10) + 2, (cell % 10) * 6 + 2);

	enum colors color = red;

	mt_ssetbgcolor(color);

	printCell();
	printBigCharInBox();
}

void initInstCounter()
{
	instCount = 0;
}

int load_prog_from_file(char *path)
{
	FILE *in = fopen(path, "r");

	if (!in) {
		return 1;
	}

	for (int i = 0; i < 100; i++) {
		fscanf(in, "%d", &ptr_str[i]);
	}

	fclose(in);

	return 0;
}

int save_prog_in_file(char *path)
{
	FILE *out = fopen(path, "w");

	for (int i = 0; i < 100; i++) {
		fprintf(out, "%d ", ptr_str[i]);
	}

	fflush(out);

	fclose(out);

	return 0;
}

int runtime()
{
	while (CU() != 2) { 
		interface(0, 0, 1, 1, 1, 0, 1, 0, 0);
 		sleep(1);
 	}

 	instCount = 0;
 	mt_gotoXY(26 + numStrForLogs, 1);
 	printf("End program\n");
 	incrementNumStrForLogs();

	return 0;
}

void initNumStrForLogs()
{
	numStrForLogs = 0;
}

void incrementNumStrForLogs()
{
	numStrForLogs++;
	if (numStrForLogs > 10)
		numStrForLogs = 0;
}

int runtime_OneStep()
{
	CU();
	interface(0, 0, 1, 1, 1, 0, 1, 0, 0);

 	return 0;
}

int m_strcmp(char *s1, char *s2)
{
	int check = 0;
	for (int i = 0; isalpha(s1[i]) && isalpha(s2[i]); i++) {
		if (s1[i] == s2[i]) {
			check++;
		} else {
			check = 0;
			break;
		}
	}

	return check;
}

int get_command(char *command)
{
	if (m_strcmp(command, "READ")) 
		return READ;
	if (m_strcmp(command, "WRITE"))
		return WRITE;
	if (m_strcmp(command, "LOAD"))
		return LOAD;
	if (m_strcmp(command, "STORE"))
		return STORE;
	if (m_strcmp(command, "ADD"))
		return ADD;
	if (m_strcmp(command, "SUB"))
		return SUB;
	if (m_strcmp(command, "DIVIDE"))
		return DIVIDE;
	if (m_strcmp(command, "MUL"))
		return MUL;
	if (m_strcmp(command, "JUMP"))
		return JUMP;
	if (m_strcmp(command, "JNEG"))
		return JNEG;
	if (m_strcmp(command, "JZ"))
		return JZ;
	if (m_strcmp(command, "HALT"))
		return HALT;

	return 1;
}