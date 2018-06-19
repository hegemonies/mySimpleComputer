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

	// printf("%s!\n", command_);

	if ((unit_commands->command = get_command_basic(command_)) == -1) {
		printf("Incorrect command");
		return 1;
	}

	if (unit_commands->command > 4) {
		*add_oper = additional_operations;
	} else if (unit_commands->command != REM && unit_commands->command != END) {
		for (; !isalpha(str[*i]); (*i)++) { }
		// for (int j = 0; isalpha(str[*i]); (*i)++, j++) {
		// 	name_var[j] = str[*i];
		// }
		*name_var = str[*i];
		(*i)++;

		var *time_var;
		char short_name_var = *name_var;
		if (!(time_var = get_var(short_name_var))) {
			if (add_var(short_name_var, get_cellNumberForNewVariables())) {
				printf("Sorry \n");
				return 1;
			}
			// time_var = get_var(short_name_var);
			// if (time_var)
			// 	printf("new variable  %c :: %d\n", time_var->name, time_var->num_cell);
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
	int real_line = 0;

	while (getline(&buf, &len, in) != -1) {
		int i = 0;
		add_oper = 0;
		pull_commands[real_line].num_line = now_lines;
		char name_var;
		if (basic_string_parser_first(buf, &i, &pull_commands[real_line], &add_oper, &name_var)) {
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
			pull_commands[real_line].str = malloc(sizeof(char) * 1000);
			tmp_command = pull_commands[real_line].command;
			int dig;
			char oper_a;
			char oper_b;
			int operation;
			int num_cell_for_jump;
			int temp;

			switch (tmp_command) {
				case GOTO:
					basic_translator_goto(buf, &dig, &i);
					pull_commands[real_line].tmp_dig = dig;

					if (isCommandInPull(pull_commands, dig)) {
						int num_line_to_ass;
						if ((num_line_to_ass = get_num_line_to_ass_from_pull(pull_commands, dig)) == -1) {
							printf("ERror GOTO.\n");
							return 1;
						}
						if (pull_commands[real_line].num_line < 10) {
							sprintf(pull_commands[real_line].str, "0%d JUMP %d", pull_commands[real_line].num_line, num_line_to_ass);
						} else {
							sprintf(pull_commands[real_line].str, "%d JUMP %d", pull_commands[real_line].num_line, num_line_to_ass);
						}
					} else {
						pull_commands[real_line].command = GOTO_B;
					}
					break;
				case IF:
					temp = pull_commands[real_line].num_line;
					basic_translator_if(buf, &oper_a, &oper_b, &operation, &i, &num_cell_for_jump);
					pull_commands[real_line].tmp_dig = num_cell_for_jump;

					if (isCommandInPull(pull_commands, num_cell_for_jump)) {
						int num_line_to_ass;
						if ((num_line_to_ass = get_num_line_to_ass_from_pull(pull_commands, num_cell_for_jump)) == -1) {
							printf("ERror IF.\n");
							return 1;
						}
						if (add_var(oper_a, get_cellNumberForNewVariables())) {
							printf("Sorry \n");
							return 1;
						}
						var *vra = get_var(oper_a);
						if (!vra) {
							printf("AHTUNG.\n");
							return 1;
						}
						if (pull_commands[real_line].num_line < 10) {
							sprintf(pull_commands[real_line].str, "0%d LOAD %d\n", pull_commands[real_line].num_line, vra->num_cell);
						} else {
							sprintf(pull_commands[real_line].str, "%d LOAD %d\n", pull_commands[real_line].num_line, vra->num_cell);
						}

						pull_commands[real_line].num_line++;

						printf("test  = %s\n", pull_commands[real_line].str);


						if (operation == EQL) {
							if (isalpha(oper_b)) {
								if (add_var(oper_b, get_cellNumberForNewVariables())) {
									printf("Sorry \n");
									return 1;
								}
								var *vrb = get_var(oper_b);

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, vrb->num_cell);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, vrb->num_cell);
								}

								pull_commands[real_line].num_line++;

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								}
								now_lines += 2;
							} else if (isdigit(oper_b)) {
								pull_commands[real_line].num_line--;
								int tmp_num_cell_for_const = get_num_line_for_tmp_var();

								sprintf(pull_commands[real_line].str, "%s%d = %c\n", pull_commands[real_line].str, tmp_num_cell_for_const, oper_b);

								pull_commands[real_line].num_line++;

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, tmp_num_cell_for_const);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, tmp_num_cell_for_const);
								}

								pull_commands[real_line].num_line++;

								sprintf(pull_commands[real_line].str, "%s%d = 0\n", pull_commands[real_line].str, tmp_num_cell_for_const);

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								}

								now_lines += 2;
							}
							
						} else if (operation == LARGER) {
							if (isalpha(oper_b)) {
								if (add_var(oper_b, get_cellNumberForNewVariables())) {
									printf("Sorry \n");
									return 1;
								}
								var *vrb = get_var(oper_b);

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, vrb->num_cell);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, vrb->num_cell);
								}

								pull_commands[real_line].num_line++;

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								}
								now_lines += 2;
							} else if (isdigit(oper_b)) {
								pull_commands[real_line].num_line--;
								int tmp_num_cell_for_const = get_num_line_for_tmp_var();

								sprintf(pull_commands[real_line].str, "%s%d = %c\n", pull_commands[real_line].str, tmp_num_cell_for_const, oper_b);

								pull_commands[real_line].num_line++;

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, tmp_num_cell_for_const);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, tmp_num_cell_for_const);
								}

								pull_commands[real_line].num_line++;

								sprintf(pull_commands[real_line].str, "%s%d = 0\n", pull_commands[real_line].str, tmp_num_cell_for_const);

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d JB %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d JB %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								}

								now_lines += 2;
							}
						} else if (operation == LESS) {

							printf("test  = %s\n", pull_commands[real_line].str);


							if (isalpha(oper_b)) {
								if (add_var(oper_b, get_cellNumberForNewVariables())) {
									printf("Sorry \n");
									return 1;
								}
								printf("test1  = %s\n", pull_commands[real_line].str);
								var *vrb = get_var(oper_b);

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, vrb->num_cell);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, vrb->num_cell);
								}

								pull_commands[real_line].num_line++;

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d JZ %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								}
								now_lines += 2;
							} else if (isdigit(oper_b)) {
								pull_commands[real_line].num_line--;
								int tmp_num_cell_for_const = get_num_line_for_tmp_var();

								sprintf(pull_commands[real_line].str, "%s%d = %c\n", pull_commands[real_line].str, tmp_num_cell_for_const, oper_b);

								pull_commands[real_line].num_line++;

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, tmp_num_cell_for_const);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d SUB %d\n", pull_commands[real_line].str, pull_commands[real_line].num_line, tmp_num_cell_for_const);
								}

								pull_commands[real_line].num_line++;

								sprintf(pull_commands[real_line].str, "%s%d = 0\n", pull_commands[real_line].str, tmp_num_cell_for_const);

								if (pull_commands[real_line].num_line < 10) {
									sprintf(pull_commands[real_line].str, "%s0%d JNEG %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								} else {
									sprintf(pull_commands[real_line].str, "%s%d JNEG %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
								}

								now_lines += 2;
							}
						}
					} else {
						pull_commands[real_line].command = IF_B;
						// pull_commands[real_line].str = buf;
						strcpy(pull_commands[real_line].str, buf);
						now_lines += 2;
					}
					pull_commands[real_line].num_line = temp;
					break;
				case LET:
					temp = pull_commands[real_line].num_line;
					basic_translator_let(buf, &pull_commands[real_line], &i);
					now_lines = pull_commands[real_line].num_line;
					now_lines--;
					pull_commands[real_line].num_line = temp;
					break;
			}
			

		} else {
			tmp_command = pull_commands[real_line].command;
			// printf("command = %d\n", tmp_command);
			switch (tmp_command) {
				case REM:
					now_lines--;
					real_line--;
					break;
				case INPUT:
					tvar = get_var(name_var);
					pull_commands[real_line].str = malloc(sizeof(char) * 20);
					if (pull_commands[real_line].num_line < 10) {
						sprintf(pull_commands[real_line].str, "0%d READ %d", pull_commands[real_line].num_line, tvar->num_cell);
					} else {
						sprintf(pull_commands[real_line].str, "%d READ %d", pull_commands[real_line].num_line, tvar->num_cell);
					}
					break;
				case OUTPUT:
					tvar = get_var(name_var);
					pull_commands[real_line].str = malloc(sizeof(char) * 20);
					if (!tvar) {
						printf("There is no such variable\n");
						return 1;
						break;
					}
					if (pull_commands[real_line].num_line < 10) {
						sprintf(pull_commands[real_line].str, "0%d WRITE %d", pull_commands[real_line].num_line, tvar->num_cell);
					} else {
						sprintf(pull_commands[real_line].str, "%d WRITE %d", pull_commands[real_line].num_line, tvar->num_cell);
					}
					break;
				case END:
					pull_commands[real_line].str = malloc(sizeof(char) * 20);
					if (pull_commands[real_line].num_line < 10) {
						sprintf(pull_commands[real_line].str, "0%d HALT 00", pull_commands[real_line].num_line);
					} else {
						sprintf(pull_commands[real_line].str, "%d HALT 00", pull_commands[real_line].num_line);
					}
			}
			// if (tmp_command != REM)
			// 	printf("%s\n", pull_commands[real_line].str);
		}

		if (pull_commands[now_lines + 1].command != REM) {
			printf("\norig = %d\nnum_line = %d\ncommand = %d\ntmp_dig = %d\n", pull_commands[real_line].orig_num_line, pull_commands[real_line].num_line, pull_commands[real_line].command, pull_commands[real_line].tmp_dig);
			if (pull_commands[real_line].str) {
				printf("str = %s\n\n", pull_commands[real_line].str);
			}
		}

		now_lines++;
		real_line++;
	}

	// printf("now_line = %d\n", now_lines);
	// printf("real_line = %d\n", real_line);

	for (int j = 0; j < real_line; j++) {
		if (pull_commands[j].command == GOTO_B) {
			int n_line_ass = get_num_line_to_ass_from_pull(pull_commands, pull_commands[j].tmp_dig);
			if (n_line_ass == -1) {
				printf("Error in GOTO\nThere is no such mark %d!\n", pull_commands[j].tmp_dig);
				return 1;
			}

			if (pull_commands[j].num_line < 10) {
				sprintf(pull_commands[j].str, "0%d JUMP %d", pull_commands[j].num_line, n_line_ass);
			} else {
				sprintf(pull_commands[j].str, "%d JUMP %d", pull_commands[j].num_line, n_line_ass);
			}
		} else if (pull_commands[j].command == IF_B) {
			int temp = pull_commands[j].num_line;

			int num_cell_for_jump;
			int operation;
			char oper_a = 0;
			char oper_b = 0;
			int k;
			basic_translator_if(pull_commands[j].str, &oper_a, &oper_b, &operation, &k, &num_cell_for_jump);
			int num_line_to_ass;

			if ((num_line_to_ass = get_num_line_to_ass_from_pull(pull_commands, num_cell_for_jump)) == -1) {
				printf("ERror IF_B.\n");
				return 1;
			}
			if (add_var(oper_a, get_cellNumberForNewVariables())) {
				printf("Sorry \n");
				return 1;
			}
			var *vra = get_var(oper_a);
			if (!vra) {
				printf("AHTUNG IF_B.\n");
				return 1;
			}
			if (pull_commands[j].num_line < 10) {
				sprintf(pull_commands[j].str, "0%d LOAD %d\n", pull_commands[j].num_line, vra->num_cell);
			} else {
				sprintf(pull_commands[j].str, "%d LOAD %d\n", pull_commands[j].num_line, vra->num_cell);
			}

			pull_commands[j].num_line++;

			if (operation == EQL) {
				if (isalpha(oper_b)) {
					if (add_var(oper_b, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var *vrb = get_var(oper_b);

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, vrb->num_cell);
					} else {
						sprintf(pull_commands[j].str, "%s%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, vrb->num_cell);
					}

					pull_commands[j].num_line++;

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d JZ %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[j].str, "%s%d JZ %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					}
					now_lines += 2;
				} else if (isdigit(oper_b)) {
					pull_commands[j].num_line--;
					int tmp_num_cell_for_const = get_num_line_for_tmp_var();

					sprintf(pull_commands[j].str, "%s%d = %c\n", pull_commands[j].str, tmp_num_cell_for_const, oper_b);

					pull_commands[j].num_line++;

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, tmp_num_cell_for_const);
					} else {
						sprintf(pull_commands[j].str, "%s%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, tmp_num_cell_for_const);
					}

					pull_commands[j].num_line++;

					sprintf(pull_commands[j].str, "%s%d = 0\n", pull_commands[j].str, tmp_num_cell_for_const);

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d JZ %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[j].str, "%s%d JZ %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					}

					now_lines += 2;
				}
			} else if (operation == LARGER) {
				if (isalpha(oper_b)) {
					if (add_var(oper_b, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var *vrb = get_var(oper_b);

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, vrb->num_cell);
					} else {
						sprintf(pull_commands[j].str, "%s%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, vrb->num_cell);
					}

					pull_commands[j].num_line++;

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d JB %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[j].str, "%s%d JB %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					}
					now_lines += 2;
				} else if (isdigit(oper_b)) {
					pull_commands[j].num_line--;
					int tmp_num_cell_for_const = get_num_line_for_tmp_var();

					sprintf(pull_commands[j].str, "%s%d = %c\n", pull_commands[j].str, tmp_num_cell_for_const, oper_b);

					pull_commands[j].num_line++;

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, tmp_num_cell_for_const);
					} else {
						sprintf(pull_commands[j].str, "%s%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, tmp_num_cell_for_const);
					}

					pull_commands[j].num_line++;

					sprintf(pull_commands[j].str, "%s%d = 0\n", pull_commands[j].str, tmp_num_cell_for_const);

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d JB %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[j].str, "%s%d JB %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					}

					now_lines += 4;
				}
			} else if (operation == LESS) {
				if (isalpha(oper_b)) {
					if (add_var(oper_b, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var *vrb = get_var(oper_b);

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, vrb->num_cell);
					} else {
						sprintf(pull_commands[j].str, "%s%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, vrb->num_cell);
					}

					pull_commands[j].num_line++;

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d JZ %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[j].str, "%s%d JZ %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					}
					now_lines += 4;
				} else if (isdigit(oper_b)) {
					pull_commands[j].num_line--;
					int tmp_num_cell_for_const = get_num_line_for_tmp_var();

					sprintf(pull_commands[j].str, "%s%d = %c\n", pull_commands[j].str, tmp_num_cell_for_const, oper_b);

					pull_commands[j].num_line++;

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, tmp_num_cell_for_const);
					} else {
						sprintf(pull_commands[j].str, "%s%d SUB %d\n", pull_commands[j].str, pull_commands[j].num_line, tmp_num_cell_for_const);
					}

					pull_commands[j].num_line++;

					sprintf(pull_commands[j].str, "%s%d = 0\n", pull_commands[j].str, tmp_num_cell_for_const);

					if (pull_commands[j].num_line < 10) {
						sprintf(pull_commands[j].str, "%s0%d JNEG %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[j].str, "%s%d JNEG %d", pull_commands[j].str, pull_commands[j].num_line, num_line_to_ass);
					}

					now_lines += 4;
				}
			}
			pull_commands[j].num_line = temp;
			now_lines = temp;
		}

	}


	for (int j = 0; j < real_line; j++) {
		fprintf(out, "%s", pull_commands[j].str);
		if (j != real_line - 1) {
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

int add_var(char name_, int num_cell_)
{
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


var *get_var(char name)
{
	if (!head_stack_of_vars) {
		return NULL;
	}

	var *tmp = head_stack_of_vars;

	while (tmp != NULL) {
		// if (m_strcmp(tmp->name, name)) {
		if (tmp->name == name) {
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

int basic_translator_if(char *buf, char *oper_a, char *oper_b, int *operation, int *i, int *num_cell_for_jump)
{
	*i = 0;
	int j;
	for (; !isalpha(buf[*i]); (*i)++) { }
	for (j = 0; isalpha(buf[*i]); (*i)++, j++) {
	}

	for (; !isalpha(buf[*i]); (*i)++) { }
	*oper_a = buf[*i];
	(*i)++;

	for (; buf[*i] != '<' && buf[*i] != '>' && buf[*i] != '='; (*i)++) { }
	if ('<' == buf[*i]) {
		*operation = LESS;
	} else if ('>' == buf[*i]) {
		*operation = LARGER;
	} else if ('=' == buf[*i]) {
		*operation = EQL;
	} else {
		*operation = 0;
		printf("Error operation.");
		return 1;
	}

	for (; !isalpha(buf[*i]); (*i)++) {
		if (isdigit(buf[*i])) {
			break;
		}
	}

	*oper_b = buf[*i];
	(*i)++;
	
	char *cmnd = malloc(sizeof(char) * 5);

	for (; !isalpha(buf[*i]); (*i)++) {}

	for (j = 0; isalpha(buf[*i]); (*i)++, j++) {
		cmnd[j] = buf[*i];
	}

	if (get_command_basic(cmnd) != GOTO) {
		printf("Error. Need GOTO\n");
		return 1;
	}

	for (; !isdigit(buf[*i]); (*i)++) {
		if (buf[*i] == '\0' || buf[*i] == '\n') {
			printf("Error\n");
			return 1;
		}
	}


	if (basic_translator_goto(buf, num_cell_for_jump, i)) {
		printf("Error\n");
		return 1;
	}

	return 0;
}


int get_num_line_for_tmp_var()
{
	var *tmp = head_stack_of_vars;
	while (tmp->next != NULL) {
		tmp = tmp->next;
	}
	return tmp->num_cell - 1;
}

void init_stack(Stack *head)
{
	head->top = 0;
	head->bot = 0;
}

void push_stack(Stack *head, char s)
{
	if (head->top < NMAX) {
		head->top++;
		head->str[head->top] = s;
	} else {
		printf("Stack is full\n");
		return;
	}
}

char pop_stack(Stack *head)
{
	char tmp = 0;
	if (head->top > 0) {
		tmp = head->str[head->top];
		head->str[head->top] = 0;
		head->top--;	
	} else if (head->top == 0) {
		tmp = head->str[head->top];
		head->str[head->top] = 0;
	}

	return tmp;
}

char pop_bot_stack(Stack *head)
{
	if (head->bot > head->top) {
		return 0;
	}
	return head->str[head->bot++];
}

char get_head_elem_stack(Stack *head)
{
	return head->str[head->top];
}

int isOperation(char symbol)
{

	if (symbol == '+' || symbol == '-' || symbol == '*' || symbol == '/' || symbol == '('  || symbol == ')') {
		return 1;
	}

	return 0;
}

int basic_translator_let(char *buf, unit_command *command, int *i_)
{
	int i = *i_;

	for (; !isalpha(buf[i]); i++) { }

	char var_where_store;
	var_where_store = buf[i];
	i++;
	var *var_store;
	if (!(var_store = get_var(var_where_store))) {
		if (add_var(var_where_store, get_cellNumberForNewVariables())) {
			printf("Sorry \n");
			return 1;
		}
		var_store = get_var(var_where_store);
	}

	char *inf = malloc(sizeof(char) * (strlen(buf) - i + 4));

	if (!inf) {
		printf("Bad alloc\n");
		return 1;
	}

	for (int j = 0; buf[i] != '\0' && buf[i] != '\n'; i++) {
		if (isalpha(buf[i]) || isdigit(buf[i]) || isOperation(buf[i])) {
			inf[j] = buf[i];
			j++;
		}
	}
	inf[strlen(inf)] = '$';

	Stack *post = malloc(sizeof(Stack));
	init_stack(post);
	Stack *in = malloc(sizeof(Stack));
	init_stack(in);

	int status = 2;
	int j = 0;

	while (status == 2) {
		if (isalpha(inf[j]) || isdigit(inf[j])) {
			push_stack(post, inf[j]);
			j++;
		}
		char first = get_head_elem_stack(in);
		if (inf[j] == '+' || inf[j] == '-') {
			if (first == 0 || first == '(') {
				push_stack(in, inf[j]);
				j++;
			} else if (first == '+' || first == '-' || first == '*' || first == '/') {
				push_stack(post, pop_stack(in));
			}

		} else if (inf[j] == '*' || inf[j] == '/') {
			if (first == 0 || first == '(' || first == '+' || first == '-') {
				push_stack(in, inf[j]);
				j++;
			} else if (first == '*' || first == '/') {
				push_stack(post, pop_stack(in));
			}
		} else if (inf[j] == '(') {
			push_stack(in, inf[j]);
			j++;
		} else if (inf[j] == ')') {
			if (first == 0) {
				status = 0;
			} else if (first == '+' || first == '-' || first == '*' || first == '/') {
				push_stack(post, pop_stack(in));
			} else if (first == '(') {
				pop_stack(in);
				j++;
			}
		} else if (inf[j] == '$') {
			if (first == 0) {
				status = 1;
			} else if (first == '+' || first == '-' || first == '*' || first == '/') {
				push_stack(post, pop_stack(in));
			} else if (first == '(') {
				status = 0;
			}
		}
	}

	*i_ = i;


	var *tmp_var;
	char name_tmp_var;
	if (post->top > 3) {
		name_tmp_var = 126;
		if (!(tmp_var = get_var(name_tmp_var))) {
			if (add_var(name_tmp_var, get_cellNumberForNewVariables())) {
				printf("Sorry \n");
				return 1;
			}
			tmp_var = get_var(name_tmp_var);
		}
	}

	if (post->top == 1) {
		char first = pop_bot_stack(post);
		first = pop_bot_stack(post);
		if (isalpha(first)) {
			var *var_A;
			if (!(var_A = get_var(first))) {
				if (add_var(first, get_cellNumberForNewVariables())) {
					printf("Sorry \n");
					return 1;
				}
				var_A = get_var(first);
			}
			if (command->num_line < 10) {
				sprintf(command->str, "%s0%d LOAD %d\n", command->str, command->num_line, var_A->num_cell);
			} else {
				sprintf(command->str, "%s%d LOAD %d\n", command->str, command->num_line, var_A->num_cell);
			}
		} else if (isdigit(first)) {
			if (command->num_line < 10) {
				sprintf(command->str, "%s0%d SET %c\n", command->str, command->num_line, first);
			} else {
				sprintf(command->str, "%s%d SET %c\n", command->str, command->num_line, first);
			}
		}
		command->num_line++;
		if (command->num_line < 10) {
			sprintf(command->str, "%s0%d STORE %d", command->str, command->num_line, var_store->num_cell);
		} else {
			sprintf(command->str, "%s%d STORE %d", command->str, command->num_line, var_store->num_cell);
		}
		command->num_line++;
	}

	while (post->bot <= post->top) {
		char first = pop_bot_stack(post);
		if (isalpha(first) || isdigit(first)) {
			push_stack(in, first);
		} else if (isOperation(first)) {
			char oper_a = pop_stack(in);
			char oper_b = pop_stack(in);

			if (isalpha(oper_b) || oper_b == name_tmp_var) {
				var *var_a;
				if (!(var_a = get_var(oper_b))) {
					if (add_var(oper_b, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var_a = get_var(oper_b);
				}
				if (command->num_line < 10) {
					sprintf(command->str, "%s0%d LOAD %d\n", command->str, command->num_line, var_a->num_cell);
				} else {
					sprintf(command->str, "%s%d LOAD %d\n", command->str, command->num_line, var_a->num_cell);
				}
			} else if (isdigit(oper_b)) { // TODO
				if (command->num_line < 10) {
					sprintf(command->str, "%s0%d SET %c\n", command->str, command->num_line, oper_b);
				} else {
					sprintf(command->str, "%s%d SET %c\n", command->str, command->num_line, oper_b);
				}
			}
			command->num_line++;

			
			if (isalpha(oper_a) || oper_a == name_tmp_var) {
				var *var_b;
				if (!(var_b = get_var(oper_a))) {
					if (add_var(oper_a, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var_b = get_var(oper_a);
				}
				if (command->num_line < 10) {
					switch (first) {
						case '+':
							sprintf(command->str, "%s0%d ADD %d\n", command->str, command->num_line, var_b->num_cell);
							break;
						case '-':
							sprintf(command->str, "%s0%d SUB %d\n", command->str, command->num_line, var_b->num_cell);
							break;
						case '*':
							sprintf(command->str, "%s0%d MUL %d\n", command->str, command->num_line, var_b->num_cell);
							break;
						case '/':
							sprintf(command->str, "%s0%d DIVIDE %d\n", command->str, command->num_line, var_b->num_cell);
							break;
					}
				} else {
					switch (first) {
						case '+':
							sprintf(command->str, "%s%d ADD %d\n", command->str, command->num_line, var_b->num_cell);
							break;
						case '-':
							sprintf(command->str, "%s%d SUB %d\n", command->str, command->num_line, var_b->num_cell);
							break;
						case '*':
							sprintf(command->str, "%s%d MUL %d\n", command->str, command->num_line, var_b->num_cell);
							break;
						case '/':
							sprintf(command->str, "%s%d DIVIDE %d\n", command->str, command->num_line, var_b->num_cell);
							break;
					}
				}
			} else if (isdigit(oper_a)) { // TODO
				int temp_num_cell = get_num_line_for_tmp_var();
				if (temp_num_cell < 10) {
					sprintf(command->str, "%s0%d = %c\n", command->str, temp_num_cell, oper_a);
				} else {
					sprintf(command->str, "%s%d = %c\n", command->str, temp_num_cell, oper_a);
				}
				if (command->num_line < 10) {
					switch (first) {
						case '+':
							sprintf(command->str, "%s0%d ADD %d\n", command->str, command->num_line, temp_num_cell);
							break;
						case '-':
							sprintf(command->str, "%s0%d SUB %d\n", command->str, command->num_line, temp_num_cell);
							break;
						case '*':
							sprintf(command->str, "%s0%d MUL %d\n", command->str, command->num_line, temp_num_cell);
							break;
						case '/':
							sprintf(command->str, "%s0%d DIVIDE %d\n", command->str, command->num_line, temp_num_cell);
							break;
					}
				} else {
					switch (first) {
						case '+':
							sprintf(command->str, "%s%d ADD %d\n", command->str, command->num_line, temp_num_cell);
							break;
						case '-':
							sprintf(command->str, "%s%d SUB %d\n", command->str, command->num_line, temp_num_cell);
							break;
						case '*':
							sprintf(command->str, "%s%d MUL %d\n", command->str, command->num_line, temp_num_cell);
							break;
						case '/':
							sprintf(command->str, "%s%d DIVIDE %d\n", command->str, command->num_line, temp_num_cell);
							break;
					}
				}
				if (temp_num_cell < 10) {
					sprintf(command->str, "%s0%d = 0\n", command->str, temp_num_cell);
				} else {
					sprintf(command->str, "%s%d = 0\n", command->str, temp_num_cell);
				}
			}
			command->num_line++;

			if (post->str[post->bot]) {
				if (command->num_line < 10) {
					sprintf(command->str, "%s0%d STORE %d\n", command->str, command->num_line, tmp_var->num_cell);
				} else {
					sprintf(command->str, "%s%d STORE %d\n", command->str, command->num_line, tmp_var->num_cell);
				}
				command->num_line++;
				push_stack(in, name_tmp_var);
			} else {
				if (command->num_line < 10) {
					sprintf(command->str, "%s0%d STORE %d", command->str, command->num_line, var_store->num_cell);
				} else {
					sprintf(command->str, "%s%d STORE %d", command->str, command->num_line, var_store->num_cell);
				}
				command->num_line++;
			}
		}
	}

	// printf("num = %d\n", command->num_line);
	// printf("str = \n%s\n", command->str);

	return 0;
}