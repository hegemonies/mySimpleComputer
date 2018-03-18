#ifndef HELPER_H
#define HELPER_H

#include "myReadKey.h"

char **banner;

int getBannerFromFile(char *namefile, int *count_lines);
void printBanner(int count_lines);
int changeSizeTerm();
void load();
int printMemory();
int printAccumalte();
int printInstCounter();
int printOperation();
int printFlags();
int printBoxBC();
int printHelpBox();
int interface(int size, int ban, int mem, int acc, int insCoun, int oper, int fl, int bc, int h);

#endif