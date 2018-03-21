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

	if (x1 < 1 || y1 < 1) {
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

	printf("\E(0");
	mt_ssetfgcolor(fg);
	mt_ssetbgcolor(bg);

	for (int k = 0; k < 2; k++) {
		for (int i = 0; i < 4; i++) {
			mt_gotoXY(y + i + k * 4, x);
			for (int j = 0; j < 8; j++) {
				if ((a[k] >> (i * 8 + j)) & 1) {
					//printf("%c", bc_shadedCell);
					printf("a");
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