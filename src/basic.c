#include "basic.h"

int basic_string_parser_first(char *str, int *i, unit_command *unit_commands, int *add_oper, char *name_var)
{
	char *command_ = malloc(sizeof(char) * 10);

	// printf("str = %s\n", str);

	// for (*i = 0; isdigit(str[*i]); (*i)++) {
	// 	if (isdigit(str[*i])) {
	// 		if (*i == 0) {
	// 			unit_commands->orig_num_line = ((int)str[*i] - 48) * 10;
	// 		} else {
	// 			unit_commands->orig_num_line += ((int)str[*i] - 48);
	// 		}
	// 	} else {
	// 		printf("Error incorrect format number");
	// 		return 1;
	// 	}
	// }
	// printf("\tunit_commands->orig_num_line = %d\n", unit_commands->orig_num_line);

	int tmp_num_cell[6];
	for (int k = 0; k < 6; k++)
		tmp_num_cell[k] = enemy_;

	int j;
	for (*i = 0, j = 0; str[*i] != '\0' && str[*i] != ' ' && str[*i] != '\n'; (*i)++, j++) {
		if (isdigit(str[*i])) {
			tmp_num_cell[j] = (int)str[*i] - 48;
		} else {
			printf("Error incorrect format number");
			return 1;	
		}
	}

	// printf("\ti = %d\n", *i);

	int count;
	for (count = 0; tmp_num_cell[count] != enemy_; count++) { }
	int tnc[count];
	for (int k = 0, n = count - 1; k < count; k++, n--) {
		tnc[n] = tmp_num_cell[k];
	}
	for (int k = 0; k < count; k++) {
		if (k == 0) {
			unit_commands->orig_num_line = tnc[k];
		} else {
			unit_commands->orig_num_line += tnc[k] * pow(10, k);
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
		if (!isupper(str[*i])) {
			printf("Error. Command not must be in lowercase.");
			return 1;
		}
		command_[j] = str[*i];
	}

	printf("%s!\n", command_);

	if ((unit_commands->command = get_command_basic(command_)) == -1) {
		printf("Incorrect command");
		return 1;
	}

	if (unit_commands->command > 4) {
		*add_oper = additional_operations;
	} else if (unit_commands->command != REM && unit_commands->command != END) {
		for (; !isalpha(str[*i]); (*i)++) { }
		for (int j = 0; isalpha(str[*i]); (*i)++, j++) {
			name_var[j] = str[*i];
		}

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



	free(command_);

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

	amount_lines = 0;

	while (getline(&buf, &len, in) != -1) {
		amount_lines++;
	}

	unit_command *pull_commands = malloc(sizeof(unit_command) * amount_lines);

	fseek(in, 0, SEEK_SET);

	int now_lines = 0;
	int add_oper;

	while (getline(&buf, &len, in) != -1) {
		int i = 0;
		add_oper = 0;
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
			// printf("command = %d\n", tmp_command);
			pull_commands[now_lines].str = malloc(sizeof(char) * 20);
			tmp_command = pull_commands[now_lines].command;
			int dig;
			char *oper_a;
			char *oper_b;
			int operation;
			switch (tmp_command) {
				case GOTO:
					basic_translator_goto(buf, &dig, &i);
					pull_commands[now_lines].tmp_dig = dig;

					if (isCommandInPull(pull_commands, dig)) {
						// printf("checkt123\n");
						// fprintf(out, "%d JUMP %d", now_lines, dig);
						int num_line_to_ass;
						if ((num_line_to_ass = get_num_line_to_ass_from_pull(pull_commands, dig)) == -1) {
							printf("ERror.\n");
							return 1;
						}
						if (pull_commands[now_lines].num_line < 10) {
							// fprintf(out, "0");
							sprintf(pull_commands[now_lines].str, "0%d JUMP %d", pull_commands[now_lines].num_line, num_line_to_ass);
						} else {
							sprintf(pull_commands[now_lines].str, "%d JUMP %d", pull_commands[now_lines].num_line, num_line_to_ass);
						}
					} else {
						// printf("che\n");
						pull_commands[now_lines].command = GOTO_B;
					}
					break;
				case IF:
					oper_a = malloc(sizeof(char) * 5);
					oper_b = malloc(sizeof(char) * 5);
					basic_translator_if(buf, oper_a, oper_b, &operation, &i);
					break;
			}
			

		} else {
			tmp_command = pull_commands[now_lines].command;
			// printf("command = %d\n", tmp_command);
			switch (tmp_command) {
				case REM:
					now_lines--;
					break;
				case INPUT:
					tvar = get_var(name_var);
					pull_commands[now_lines].str = malloc(sizeof(char) * 20);
					if (pull_commands[now_lines].num_line < 10) {
						sprintf(pull_commands[now_lines].str, "0%d READ %d", pull_commands[now_lines].num_line, tvar->num_cell);
					} else {
						sprintf(pull_commands[now_lines].str, "%d READ %d", pull_commands[now_lines].num_line, tvar->num_cell);
					}
					break;
				case OUTPUT:
					tvar = get_var(name_var);
					pull_commands[now_lines].str = malloc(sizeof(char) * 20);
					if (!tvar) {
						printf("There is no such variable\n");
						return 1;
						break;
					}
					if (pull_commands[now_lines].num_line < 10) {
						sprintf(pull_commands[now_lines].str, "0%d WRITE %d", pull_commands[now_lines].num_line, tvar->num_cell);
					} else {
						sprintf(pull_commands[now_lines].str, "%d WRITE %d", pull_commands[now_lines].num_line, tvar->num_cell);
					}
					break;
				case END:
					pull_commands[now_lines].str = malloc(sizeof(char) * 20);
					if (pull_commands[now_lines].num_line < 10) {
						sprintf(pull_commands[now_lines].str, "0%d HALT 00", pull_commands[now_lines].num_line);
					} else {
						sprintf(pull_commands[now_lines].str, "%d HALT 00", pull_commands[now_lines].num_line);
					}
			}
			// if (tmp_command != REM)
			// 	printf("%s\n", pull_commands[now_lines].str);
		}

		// if (pull_commands[now_lines + 1].command != REM) {
		// 	printf("\norig = %d\nnum_line = %d\ncommand = %d\ntmp_dig = %d\n", pull_commands[now_lines].orig_num_line, pull_commands[now_lines].num_line, pull_commands[now_lines].command, pull_commands[now_lines].tmp_dig);
		// 	if (pull_commands[now_lines].str) {
		// 		printf("str = %s\n\n", pull_commands[now_lines].str);
		// 	}
		// }

		now_lines++;
	}

	for (int i = 0; i < now_lines; i++) {
		if (pull_commands[i].command == GOTO_B) {
			int n_line_ass = get_num_line_to_ass_from_pull(pull_commands, pull_commands[i].tmp_dig);
			if (n_line_ass == -1) {
				printf("Error in GOTO\nThere is no such mark %d!\n", pull_commands[i].tmp_dig);
				return 1;
			}

			if (pull_commands[i].num_line < 10) {
				sprintf(pull_commands[i].str, "0%d JUMP %d", pull_commands[i].num_line, n_line_ass);
			} else {
				sprintf(pull_commands[i].str, "%d JUMP %d", pull_commands[i].num_line, n_line_ass);
			}
		}
	}

	for (int i = 0; i < now_lines; i++) {
		fprintf(out, "%s", pull_commands[i].str);
		if (i != now_lines - 1) {
			fprintf(out, "\n");
		}
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
	if (m_strcmp(str, "GOTO_B"))
		return GOTO_B;
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

int basic_translator_goto(char *str, int *dig, int *i)
{
	for (; !isdigit(str[*i]); (*i)++) { }

	int tmp_num_cell[6];
	for (int k = 0; k < 6; k++)
		tmp_num_cell[k] = enemy_;

	int j;
	for (j = 0; str[*i] != '\0' && str[*i] != ' ' && str[*i] != '\n'; (*i)++, j++) {
		if (isdigit(str[*i])) {
			tmp_num_cell[j] = (int)str[*i] - 48;
		} else {
			if (j == 2 && *dig == 0) {
				break;
			} else {
				printf("Error incorrect format number");
				return 1;	
			}
		}
	}

	int count;
	for (count = 0; tmp_num_cell[count] != enemy_; count++) { }
	int tnc[count];
	for (int k = 0, n = count - 1; k < count; k++, n--) {
		tnc[n] = tmp_num_cell[k];
	}
	for (int k = 0; k < count; k++) {
		if (k == 0) {
			*dig = tnc[k];
		} else {
			*dig += tnc[k] * pow(10, k);
		}
	}

	return 0;
}

int get_num_line_to_ass_from_pull(unit_command *pull_commands, int num)
{
	for (int i = 0; i < amount_lines; i++) {
		if (pull_commands[i].orig_num_line == num) {
			return pull_commands[i].num_line;
		}
	}

	return -1;
}

int isCommandInPull(unit_command *pull_commands, int num)
{
	for (int i = 0; i < amount_lines; i++) {
		if (pull_commands[i].orig_num_line == num) {
			return 1;
		}
	}

	return 0;
}

int basic_translator_if(char *buf, char *oper_a, char *oper_b, int *operation, int *i)
{
	for (; !isalpha(buf[*i]); (*i)++) { }
	for (; isalpha(buf[*i]); (*i)++) {
		oper_a[*i] = buf[*i];
	}

	for (; buf[*i] != '<' || buf[*i] != '>' || buf[*i] != '='; (*i)++) { }
	if ('<' == buf[*i])
		*operation = LESS;
	if ('>' == buf[*i])
		*operation = LARGER;
	if ('=' == buf[*i])
		*operation = EQL;

	for (; !isalpha(buf[*i]); (*i)++) { }
	for (; isalpha(buf[*i]); (*i)++) {
		oper_b[*i] = buf[*i];
	}

	return 0;
}