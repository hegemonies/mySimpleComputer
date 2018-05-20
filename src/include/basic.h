#ifndef BASIC_H
#define BASIC_H

#include "asm.h"
#include "helper.h"
#include <math.h>

typedef struct
{
	int orig_num_line;
	int num_line;
	int command;
	int tmp_dig;
	char *str;
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
#define GOTO_B 6
#define IF     7
#define IF_B   8
#define LET    9


#define additional_operations 10


int basic_string_parser_first(char *str, int *i, unit_command *unit_commands, int *add_oper, char *name_var);
int basic_translator(char *path_from, char *path_where);
int get_command_basic(char *str);

	
int amount_lines;
var *head_stack_of_vars;

int add_var(char *name, int num_cell);
var *get_var(char *name);

int cell_number_for_variables;

int get_cellNumberForNewVariables();

int basic_translator_goto(char *str, int *dig, int *i);

int get_num_line_to_ass_from_pull(unit_command *pull_commands, int num);
int isCommandInPull(unit_command *pull_commands, int num);


#define EQL 20
#define LARGER 21
#define LESS 22

int basic_translator_if(char *buf, char *oper_a, char *oper_b, int *operation, int *i, 	int *num_cell_for_jump);

int get_num_line_for_tmp_var();

int isOperation(char symbol);

int basic_translator_let(char *buf, unit_command *command, int *i_);

#define NMAX 100

typedef struct Stack
{
	char str[NMAX];
	int top;
} Stack;

void init_stack(Stack *head);
char pop_stack(Stack *head);
void push_stack(Stack *head, char str);
char get_head_elem_stack(Stack *head);

#endif