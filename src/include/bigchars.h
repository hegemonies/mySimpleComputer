#ifndef BIGCHARS_H
#define BIGCHARS_H

#include "terminal.h"

//a f g i j k l m n o p q r s t u v w x y z { | } ~
//▒ ° ± ␋ ┘ ┐ ┌ └ ┼ ⎺ ⎻ ─ ⎼ ⎽ ├ ┤ ┴ ┬ │ ≤ ≥ π ≠ £ ·

#define bc_cornerUpLeft 'l'
#define bc_cornerDownLeft 'm'
#define bc_cornerUpRight 'k'
#define bc_cornerDownRight 'j'
#define bc_horizontaleLine 'q'
#define bc_verticalLine 'x'
#define bc_shadedCell 'a'

int bc_printA(char *str);
int bc_box(int x1, int y1, int x2, int y2);
// int bc_printbigchar(int a[2], int x, int y, enum colors, enum colors);
// int bc_setbigcharpos(int *big, int x, int y, int value);
// int bc_getbigcharpos(int *big, int x, int y, int *value);
// int bc_bigcharwrite(int fd, int *big, int count);
// int bc_bigcharread(int fd, int *big, int need_count, int *count);

#endif