#ifndef ASM_H
#define ASM_H

#include "helper.h"

#define enemy_ 10

void help();
int asm_string_parser(char *str, int *num_str, int *command, int *num_cell, int *i);
int asm_translate(char *path_from, char *path_where);

#endif