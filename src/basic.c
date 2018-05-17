#include "basic.h"

int basic_string_parser_first(char *str, int *i, unit_command *unit_commands, int *add_oper, char *name_var)
{
	char *command_ = malloc(sizeof(char) * 7);

	for (*i = 0; (*i) < 2; (*i)++) {
		if (isdigit(str[*i])) {
			if (*i == 0) {
				unit_commands->orig_num_line += ((int)str[*i] - 48) * 10;
			} else {
				unit_commands->orig_num_line += ((int)str[*i] - 48);
			}
		} else {
			printf("Error incorrect format number");
			return 1;
		}
	}

	for (; !isalpha(str[*i]); (*i)++) { }

	if (*(i) < 3) {
		printf("Too close");
		return 1;
	} else if (*(i) > 10) {
		printf("Too much distance");
		return 1;
	}

	for (int j = 0; isalpha(str[*i]); (*i)++, j++) {
		command_[j] = str[*i];
	}

	printf("%s!\n", command_);

	if ((unit_commands->command = get_command_basic(command_)) == -1) {
		printf("Incorrect command");
		return 1;
	}

	if (unit_commands->command > 4) {
		*add_oper = additional_operations;
	} else if (unit_commands->command != REM) {
		for (; !isalpha(str[*i]); (*i)++) { }
		// printf("i = %d\n", (*i));
		// char *name_var = malloc(sizeof(char) * 10);
		// name_var = malloc(sizeof(char) * 10);
		for (int j = 0; isalpha(str[*i]); (*i)++, j++) {
			name_var[j] = str[*i];
		}
		// printf("i = %d\n", (*i));
		var *time_var;
		if (!(time_var = get_var(name_var))) {
			if (add_var(name_var, get_cellNumberForNewVariables())) {
				printf("Sorry \n");
				return 1;
			}
			time_var = get_var(name_var);
			if (time_var)
				printf("%s :: %d\n", time_var->name, time_var->num_cell);
		}
	}



	// free(command_);

	return 0;
}


int basic_translator(char *path_from, char *path_where)
{
	FILE *in = fopen(path_from, "r");

	if (!in) {
		printf("No such file.\n");
		return 1;
	}

	FILE *out = fopen(path_where, "w");

	char *buf = NULL;
	size_t len = 0;

	int amount_lines = 0;

	while (getline(&buf, &len, in) != -1) {
		amount_lines++;
	}

	unit_command *pull_commands = malloc(sizeof(unit_command) * amount_lines);

	fseek(in, 0, SEEK_SET);

	int now_lines = 0;
	int add_oper = 0;

	// int num_line;
	// int command;
	// int num_cell;

	while (getline(&buf, &len, in) != -1) {
		int i = 0;
		pull_commands[now_lines].num_line = now_lines;
		char *name_var = malloc(sizeof(char) * 10);
		if (basic_string_parser_first(buf, &i, &pull_commands[now_lines], &add_oper, name_var)) {
			fclose(in);
			printf(" in %d line\n", now_lines);
			printf("%s\n", buf);
			for (; i != 0; i--) {
				printf(" ");
			}
			mt_ssetbgcolor(red);
			printf("^");
			mt_stopcolor();
			printf("  Error is here\n");
 
			return 1;
		}

		int tmp_command;
		var *tvar;

		if (add_oper) {

			

		} else {
			tmp_command = pull_commands[now_lines].command;
			// printf("command = %d\n", tmp_command);
			switch (tmp_command) {
				case REM:
					now_lines--;
					break;
				case INPUT:
					tvar = get_var(name_var);
					if (pull_commands[now_lines].num_line < 10) {
						fprintf(out, "0");
					}
					fprintf(out, "%d READ %d\n", pull_commands[now_lines].num_line, tvar->num_cell);
					break;
				case OUTPUT:
					tvar = get_var(name_var);
					if (!tvar) {
						printf("There is no such variable\n");
						return 1;
						break;
					}
					if (pull_commands[now_lines].num_line < 10) {
						fprintf(out, "0");
					}
					fprintf(out, "%d WRITE %d\n", pull_commands[now_lines].num_line, tvar->num_cell);
					break;
				case END:
					if (pull_commands[now_lines].num_line < 10) {
						fprintf(out, "0");
					}
					fprintf(out, "%d HALT 00\n", pull_commands[now_lines].num_line);
			}
		}

		now_lines++;
	}


	fclose(in);
	fclose(out);

	return 0;
}

int get_command_basic(char *str)
{
	if (m_strcmp(str, "REM"))
		return REM;
	if (m_strcmp(str, "INPUT"))
		return INPUT;
	if (m_strcmp(str, "OUTPUT"))
		return OUTPUT;
	if (m_strcmp(str, "GOTO"))
		return GOTO;
	if (m_strcmp(str, "IF"))
		return IF;
	if (m_strcmp(str, "LET"))
		return LET;
	if (m_strcmp(str, "END"))
		return END;

	return -1;
}

int add_var(char *name_, int num_cell_)
{
	if (!name_) {
		printf("Name is NULL\n");
		return 1;
	}

	if (!head_stack_of_vars) {
		head_stack_of_vars = malloc(sizeof(var));
		if (!head_stack_of_vars) {
			printf("Bad alloc head_stack_of_vars\n");
			return 1;
		}
		head_stack_of_vars->name = name_;
		head_stack_of_vars->num_cell = num_cell_;
		head_stack_of_vars->next = NULL;
	} else {
		var *tmp = head_stack_of_vars;
		var *prev;
		while (tmp != NULL) {
			prev = tmp;
			tmp = tmp->next;
		}
		tmp = malloc(sizeof(var));
		tmp->name = name_;
		tmp->num_cell = num_cell_;
		prev->next = tmp;
		printf("check add var\n");
	}

	return 0;
}


var *get_var(char *name)
{
	if (!head_stack_of_vars) {
		return NULL;
	}

	var *tmp = head_stack_of_vars;

	while (tmp != NULL) {
		if (m_strcmp(tmp->name, name)) {
			return tmp;
		}
		tmp = tmp->next;
	}

	return NULL;
}


int get_cellNumberForNewVariables()
{
	if (cell_number_for_variables < 51) {
		printf("Too many variables\n");
		exit(1);
	}

	return --cell_number_for_variables;
}