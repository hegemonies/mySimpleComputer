#ifndef BASIC_H
#define BASIC_H

#include "asm.h"
#include "helper.h"

typedef struct
{
	int orig_num_line;
	int num_line;
	int command;
} unit_command;

typedef struct var
{
	int num_cell;
	char *name;
	struct var *next;
} var;


#define REM    1
#define INPUT  2
#define OUTPUT 3
#define END    4
#define GOTO   5
#define IF     6
#define LET    7


#define additional_operations 10


int basic_string_parser_first(char *str, int *i, unit_command *unit_commands, int *add_oper, char *name_var);
int basic_translator(char *path_from, char *path_where);
int get_command_basic(char *str);

	

var *head_stack_of_vars;

int add_var(char *name, int num_cell);
var *get_var(char *name);

int cell_number_for_variables;

int get_cellNumberForNewVariables();

int basic_translator_goto(char *str, int *dig, int *i);


#endif