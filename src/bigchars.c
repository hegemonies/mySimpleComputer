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

	mt_getscreensize(&size_y, &size_x);

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