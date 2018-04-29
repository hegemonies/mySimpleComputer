#ifndef HELPER_H
#define HELPER_H

// #include "myReadKey.h"
#include "cpu.h"

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

enum way {
	way_UP, way_DOWN, way_LEFT, way_RIGHT, way_DEFAULT
};

int cell;
int instCount;
int numStrForLogs;

int intToHex(int number, char *str);
void initNumberCell();
void printCell();
int printBigCharInBox();
void selectCellMemory(enum way w);
void selectCellMemoryByNumber(int num);
void initInstCounter();

int load_prog_from_file(char *path);
int save_prog_in_file(char *path);

int runtime();

void initNumStrForLogs();
void incrementNumStrForLogs();

int runtime_OneStep();

int get_command(char *command);

#endif