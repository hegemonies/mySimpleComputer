#include "bigchars.h"

int main()
{
	/*
	sc_memoryInit();
	//sm_printMemory();
	
	sc_memorySet(0, 65535);
	
	//int tmp = 0;
	//sc_memoryGet(55, &tmp);
	//printf("tmp = %d\n", tmp);
	//sc_memorySave("test.bin");
	
	//sc_memoryLoad("test.bin");
	// system("clear");
	// sm_printMemory();

	printf("flags = ");
	for(int i = 7; i >= 0; --i) {
		printf("%d", (flags >> i) & 1);
		if (i == 4) {
			printf(" ");
		}
	}
	printf("\n");

	int *val = malloc(sizeof(int));

	sc_commandEncode(26, 3, val);

	printf("value = ");
	for(int i = 14; i >= 0; --i) {
		printf("%d", (*val >> i) & 1);
		if (i % 7 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	int *command = malloc(sizeof(int));
	int *operand = malloc(sizeof(int));
	int value = 0b001100111011001;
	sc_commandDecode(value, command, operand);

	printf("value = ");
	for(int i = 14; i >= 0; --i) {
		printf("%d", (value >> i) & 1);
		if (i % 7 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	printf("command = ");
	for(int i = 7; i >= 0; --i) {
		printf("%d", (*command >> i) & 1);
		if (i % 4 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	printf("operand = ");
	for(int i = 7; i >= 0; --i) {
		printf("%d", (*operand >> i) & 1);
		if (i % 4 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	sm_printMemory();

	// mt_clrscr();

	mt_gotoXY(3, 2);


	int *r = malloc(sizeof(int));
	int *c = malloc(sizeof(int));

	mt_getscreensize(r, c);

	printf("%d :: %d\n", *r, *c);

	enum colors f = red;

	mt_ssetfgcolor(f);

	enum colors a = white;

	mt_ssetbgcolor(a);

	printf("HELLO\n");

	mt_stopcolor();
*/

	//bc_box(5, 10, 40, 20);

	int a[2] = bc_Null;

	enum colors fg = red;
	enum colors bg = white;

	bc_printbigchar(a, 2, 2, fg, bg);

	int b[2] = bc_One;

	bc_printbigchar(b, 11, 2, fg, bg);

	int c[2] = bc_Two;

	bc_printbigchar(c, 20, 2, fg, bg);

	int a1[2] = bc_Three;

	bc_printbigchar(a1, 29, 2, fg, bg);

	int a2[2] = bc_Four;

	bc_printbigchar(a2, 38, 2, fg, bg);

	int a3[2] = bc_Five;

	bc_printbigchar(a3, 47, 2, fg, bg);

	int a4[2] = bc_Six;

	bc_printbigchar(a4, 56, 2, fg, bg);

	int a5[2] = bc_Seven;

	bc_printbigchar(a5, 65, 2, fg, bg);

	int a6[2] = bc_Eight;

	bc_printbigchar(a6, 74, 2, fg, bg);

	int a7[2] = bc_Nine;

	bc_printbigchar(a7, 83, 2, fg, bg);

	int a8[2] = bc_A;

	bc_printbigchar(a8, 92, 2, fg, bg);

	int a9[2] = bc_B;


	bc_printbigchar(a9, 101, 2, fg, bg);

	int aa[2] = bc_C;


	bc_printbigchar(aa, 110, 2, fg, bg);

	int ab[2] = bc_D;

	bc_printbigchar(ab, 2, 11, fg, bg);

	int ac[2] = bc_E;

	bc_printbigchar(ac, 11, 11, fg, bg);

	int ad[2] = bc_F;

	bc_printbigchar(ad, 20, 11, fg, bg);

	int af[2] = bc_Plus;

	bc_printbigchar(af, 29, 11, fg, bg);
	
	int big[2] = { 0 };

	bc_setbigcharpos(big, 2, 1, 1);

	printf("big = ");
	for(int i = 31; i >= 0; --i) {
		printf("%d", (*big >> i) & 1);
		if (i % 4 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	int val;

	bc_getbigcharpos(big, 2, 1, &val);

	printf("value = %d\n", val);

	int fd = open("test.txt", O_WRONLY);

	big[0] = -1;
	big[1] = -1;

	bc_bigcharwrite(fd, big, 1);
	close(fd);
	
	int count;

	int b33[2] = { 0 };

	fd = open("test.txt", O_RDONLY);
	
	bc_bigcharread(fd, b33, 1, &count);
	
	close(fd);

	printf("b = %d\n", b33[0]);
	printf("b = %d\n", b33[1]);
	printf("\n");

	return 0;
}
