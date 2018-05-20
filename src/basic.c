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
			time_var = get_var(short_name_var);
			if (time_var)
				printf("new variable  %c :: %d\n", time_var->name, time_var->num_cell);
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
			pull_commands[real_line].str = malloc(sizeof(char) * 120);
			tmp_command = pull_commands[real_line].command;
			int dig;
			char oper_a;
			char oper_b;
			int operation;
			int num_cell_for_jump;

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
					// oper_a = malloc(sizeof(char) * 5);
					// oper_b = malloc(sizeof(char) * 5);					
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
							} else if (isdigit(&oper_b)) {
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
							} else if (isdigit(&oper_b)) {
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
							} else if (isdigit(&oper_b)) {
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
					}

					break;
				case LET:
					basic_translator_let(buf, &pull_commands[real_line], &i);
					now_lines = pull_commands[real_line].num_line;
					break;
			}
			

		} else {
			tmp_command = pull_commands[real_line].command;
			// printf("command = %d\n", tmp_command);
			switch (tmp_command) {
				case REM:
					now_lines--;
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

		// if (pull_commands[now_lines + 1].command != REM) {
		// 	printf("\norig = %d\nnum_line = %d\ncommand = %d\ntmp_dig = %d\n", pull_commands[real_line].orig_num_line, pull_commands[real_line].num_line, pull_commands[real_line].command, pull_commands[real_line].tmp_dig);
		// 	if (pull_commands[real_line].str) {
		// 		printf("str = %s\n\n", pull_commands[real_line].str);
		// 	}
		// }

		now_lines++;
		real_line++;
	}

	for (int i = 0; i < real_line; i++) {
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
		} else if (pull_commands[i].command == IF_B) {
			int num_cell_for_jump;
			int operation;
			// char *oper_a = malloc(sizeof(char) * 5);
			// char *oper_b = malloc(sizeof(char) * 5);
			char oper_a;
			char oper_b;
			// printf("pull_commands[I].str = %s\n", pull_commands[i].str);
			int j;
			basic_translator_if(pull_commands[i].str, &oper_a, &oper_b, &operation, &j, &num_cell_for_jump);
			int num_line_to_ass;

			// printf("oper_a = %s\noper_b = %s\noperation = %d\nnum_cell_for_jump = %d\n", oper_a, oper_b, operation, num_cell_for_jump);

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
			// printf("pull_commands[i].num_line = %d\n", pull_commands[i].num_line);
			if (pull_commands[i].num_line < 10) {
				sprintf(pull_commands[i].str, "0%d LOAD %d\n", pull_commands[i].num_line, vra->num_cell);
			} else {
				sprintf(pull_commands[i].str, "%d LOAD %d\n", pull_commands[i].num_line, vra->num_cell);
			}

			pull_commands[i].num_line++;

			if (operation == EQL) {
				if (isalpha(oper_b)) {
					if (add_var(oper_b, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var *vrb = get_var(oper_b);

					if (pull_commands[i].num_line < 10) {
						sprintf(pull_commands[i].str, "%s0%d SUB %d\n", pull_commands[i].str, pull_commands[i].num_line, vrb->num_cell);
					} else {
						sprintf(pull_commands[i].str, "%s%d SUB %d\n", pull_commands[i].str, pull_commands[i].num_line, vrb->num_cell);
					}

					pull_commands[i].num_line++;

					if (pull_commands[i].num_line < 10) {
						sprintf(pull_commands[i].str, "%s0%d JZ %d", pull_commands[i].str, pull_commands[i].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[i].str, "%s%d JZ %d", pull_commands[i].str, pull_commands[i].num_line, num_line_to_ass);
					}
					now_lines += 2;
				} else if (isdigit(oper_b)) {
					pull_commands[i].num_line--;
					int tmp_num_cell_for_const = get_num_line_for_tmp_var();

					sprintf(pull_commands[i].str, "%s%d = %c\n", pull_commands[i].str, tmp_num_cell_for_const, oper_b);

					pull_commands[i].num_line++;

					if (pull_commands[i].num_line < 10) {
						sprintf(pull_commands[i].str, "%s0%d SUB %d\n", pull_commands[i].str, pull_commands[i].num_line, tmp_num_cell_for_const);
					} else {
						sprintf(pull_commands[i].str, "%s%d SUB %d\n", pull_commands[i].str, pull_commands[i].num_line, tmp_num_cell_for_const);
					}

					pull_commands[i].num_line++;

					sprintf(pull_commands[i].str, "%s%d = 0\n", pull_commands[i].str, tmp_num_cell_for_const);
					// pull_commands[real_line].num_line++;

					if (pull_commands[i].num_line < 10) {
						sprintf(pull_commands[i].str, "%s0%d JZ %d", pull_commands[i].str, pull_commands[i].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[i].str, "%s%d JZ %d", pull_commands[i].str, pull_commands[i].num_line, num_line_to_ass);
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
					// pull_commands[real_line].num_line++;

					if (pull_commands[real_line].num_line < 10) {
						sprintf(pull_commands[real_line].str, "%s0%d JB %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[real_line].str, "%s%d JB %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
					}

					now_lines += 2;
				}
			} else if (operation == LESS) {
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
					// pull_commands[real_line].num_line++;

					if (pull_commands[real_line].num_line < 10) {
						sprintf(pull_commands[real_line].str, "%s0%d JNEG %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
					} else {
						sprintf(pull_commands[real_line].str, "%s%d JNEG %d", pull_commands[real_line].str, pull_commands[real_line].num_line, num_line_to_ass);
					}

					now_lines += 2;
				}
			}
		}

	}

	for (int i = 0; i < real_line; i++) {
		fprintf(out, "%s", pull_commands[i].str);
		if (i != real_line - 1) {
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
	// printf("buf = %s\n", buf);
	*i = 0;
	int j;
	for (; !isalpha(buf[*i]); (*i)++) { }
	for (j = 0; isalpha(buf[*i]); (*i)++, j++) {
		// oper_a[j] = buf[*i];
	}

	// for (j = 0; j < 5; j++) {
	// 	oper_a[j] = 0;
	// }

	for (; !isalpha(buf[*i]); (*i)++) { }
	// for (j = 0; isalpha(buf[*i]); (*i)++, j++) {
	// 	oper_a[j] = buf[*i];
	// }
	*oper_a = buf[*i];
	(*i)++;
	// printf("oper_a = %s!\n", oper_a);

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

	// int isDigit = 0;

	for (; !isalpha(buf[*i]); (*i)++) {
		if (isdigit(buf[*i])) {
			// isDigit = 1;
			break;
		}
	}

	// if (isDigit) {
	// 	for (j = 0; isdigit(buf[*i]); (*i)++, j++) {
	// 		oper_b[j] = buf[*i];
	// 	}
	// } else {
	// 	for (j = 0; isalpha(buf[*i]); (*i)++, j++) {
	// 		oper_b[j] = buf[*i];
	// 	}
	// }
	*oper_b = buf[*i];
	(*i)++;

	// printf("oper_b = %s!\n", oper_b);
	
	char *cmnd = malloc(sizeof(char) * 5);

	for (; !isalpha(buf[*i]); (*i)++) {}

	for (j = 0; isalpha(buf[*i]); (*i)++, j++) {
		cmnd[j] = buf[*i];
	}

	// printf("cmnd = %s\n", cmnd);

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

	// printf("num_cell_for_jump = %d!\n", *num_cell_for_jump);

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
	// Stack *oper = malloc(sizeof(Stack));
	// Stack *opnd = malloc(sizeof(Stack));

	for (; !isalpha(buf[i]); i++) { }

	char var_where_store;
	var_where_store = buf[i];
	i++;
	printf("var_where_store = %c\n", var_where_store);
	var *var_store;
	if (!(var_store = get_var(var_where_store))) {
		if (add_var(var_where_store, get_cellNumberForNewVariables())) {
			printf("Sorry \n");
			return 1;
		}
		var_store = get_var(var_where_store);
		// var_store = get_var(name_var);
		// if (var_store)
		// 	printf("new variable  %s :: %d\n", var_store->name, var_store->num_cell);
	}

	// printf("strlen(buf) = %ld\n", strlen(buf) - i + 1);

	char *inf = malloc(sizeof(char) * (strlen(buf) - i + 4));

	if (!inf) {
		printf("Bad alloc\n");
		return 1;
	}

	for (int j = 0; buf[i] != '\0' && buf[i] != '\n'; i++) {
		// printf("%c\n", buf[i]);
		if (isalpha(buf[i]) || isdigit(buf[i]) || isOperation(buf[i])) {
			inf[j] = buf[i];
			j++;
		}
	}
	inf[strlen(inf)] = '$';
	printf("inf = %s\n", inf);

	// printf("inf = %s!\n", inf);
	// printf("strlen inf = %ld\n", strlen(inf));

	Stack *post = malloc(sizeof(Stack));
	init_stack(post);
	Stack *in = malloc(sizeof(Stack));
	init_stack(in);

	int status = 2;
	int j = 0;

	while (status == 2) {
		// printf("%c", inf[j]);
		if (isalpha(inf[j]) || isdigit(inf[j])) {
			// printf("check alpha\n");
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
			// printf("check 1\n");
			if (first == 0 || first == '(' || first == '+' || first == '-') {
				// printf("check 2\n");
				push_stack(in, inf[j]);
				j++;
			} else if (first == '*' || first == '/') {
				// printf("check 2 what\n");
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
			// printf("check 3\n");
			if (first == 0) {
				status = 1;
			} else if (first == '+' || first == '-' || first == '*' || first == '/') {
				// printf("check 4\n");
				push_stack(post, pop_stack(in));
			} else if (first == '(') {
				status = 0;
			}
		}
	}
	printf("!\n");

	// char first = pop_stack(in);
	// if (first == '+' || first == '-' || first == '*' || first == '/') {
	// 	push_stack(post, first);
	// }
	// int t = post->top;
	// printf("post->top = %d\n", post->top);
	// printf("post = ");
	// while (post->top > 0) {
	// 	printf("%c", pop_stack(post));
	// }
	// printf("!\n");
	// printf("in->top = %d\n", in->top);
	// printf("in = ");
	// while (in->top > 0) {
	// 	printf("%c ", pop_stack(in));
	// }

	*i_ = i;
	// post->top = t;
	// printf("post = ");
	// while (post->bot <= post->top) {
	// 	printf("%c", pop_bot_stack(post));
	// }
	// printf("!\n");


	var *tmp_var;
	char name_tmp_var;
	if (post->top > 3) {
		name_tmp_var = 126;
		printf("name_tmp_var = %c\n", name_tmp_var);
		if (!(tmp_var = get_var(name_tmp_var))) {
			if (add_var(name_tmp_var, get_cellNumberForNewVariables())) {
				printf("Sorry \n");
				return 1;
			}
			tmp_var = get_var(name_tmp_var);
			// tmp_var = get_var(name_var);
			// if (tmp_var)
			// 	printf("new variable  %s :: %d\n", tmp_var->name, tmp_var->num_cell);
		}
	}

	while (post->bot <= post->top) {
		char first = pop_bot_stack(post);
		printf("first = %c\n", first);
		if (isalpha(first) || isdigit(first)) {
			printf("check 1\n");
			push_stack(in, first);
		} else if (isOperation(first)) {
			printf("check 2\n");
			char oper_a = pop_stack(in);
			printf("oper_a = %c\n", oper_a);
			char oper_b = pop_stack(in);
			printf("oper_b = %c\n", oper_b);

			if (isalpha(oper_b) || oper_b == name_tmp_var) {
				var *var_a;
				if (!(var_a = get_var(oper_b))) {
					printf("\tnew var\n");
					if (add_var(oper_b, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var_a = get_var(oper_b);
					// var_a = get_var(name_var);
					// if (var_a)
					// 	printf("new variable  %s :: %d\n", var_a->name, var_a->num_cell);
				}
				if (command->num_line < 10) {
					sprintf(command->str, "%s0%d LOAD %d\n", command->str, command->num_line, var_a->num_cell);
				} else {
					sprintf(command->str, "%s%d LOAD %d\n", command->str, command->num_line, var_a->num_cell);
				}
				command->num_line++;
			}

			// if (oper_b == 0) {
			// 	push_stack(post, oper_a);
			// 	push_stack(post, first);
			// 	continue;
			// }
			if (isalpha(oper_a) || oper_a == name_tmp_var) {
				var *var_b;
				if (!(var_b = get_var(oper_a))) {
					printf("\tnew var\n");
					if (add_var(oper_a, get_cellNumberForNewVariables())) {
						printf("Sorry \n");
						return 1;
					}
					var_b = get_var(oper_a);
					// var_b = get_var(name_var);
					// if (var_b)
					// 	printf("new variable  %s :: %d\n", var_b->name, var_b->num_cell);
				}
				if (command->num_line < 10) {
					printf("che\n");
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
				command->num_line++;
			}

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
					sprintf(command->str, "%s0%d STORE %d\n", command->str, command->num_line, var_store->num_cell);
				} else {
					sprintf(command->str, "%s%d STORE %d\n", command->str, command->num_line, var_store->num_cell);
				}
				command->num_line++;
			}
		}
	}

	printf("str = \n%s\n", command->str);

	return 0;
}