#ifndef BASIC_H
#define BASIC_H

#include "asm.h"
#include "helper.h"

typedef struct
{
	int orig_num_line;
	int num_line;
	int command;
	// int num_cell;
} unit_command;


#define REM    1
#define INPUT  2
#define OUTPUT 3
#define END    4
#define GOTO   5
#define IF     6
#define LET    7

#define NONE_value 10000


#define additional_operations 10


int basic_string_parser(char *str, int *num_line, int *command, int *num_cell);
int basic_translator(char *path_from, char *path_where);
int get_command_basic(char *str);

typedef struct var
{
	char *name;
	int value;
	int num_cell;
	struct var *next;
} var;

var *head_stack_of_vars;

int add_var(char *name, int value, int num_cell);
var *get_var(char *name);
// int check_var(char *name);

int cell_number_for_variables;

int get_cellNumberForNewVariables();


#endif