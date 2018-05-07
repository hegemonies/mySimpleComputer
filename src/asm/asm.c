#include "asm.h"

#define enemy_ 10

void help()
{
	printf("Help:\n");
	printf("./sat [filename from [*.a]] [filename where [*.o]]\n");
}

int asm_string_parser(char *str, int *num_str, int *command, int *num_cell, int *i)
{
	char *command_ = calloc(0, sizeof(char) * 7);
	*num_str = 0;
	*num_cell = 0;

	for (*i = 0; str[*i] != ' '; (*i) += 1) {
		if (isdigit(str[*i])) {
			if (*i == 0) {
				*num_str += ((int)str[*i] - 48) * 10;
			} else {
				*num_str += ((int)str[*i] - 48);
			}
		} else {
			printf("Error incorrect format number");
			return 1;
		}
	}

	if (*i > 2) {
		printf("Too many line numbers");
		return 1;
	} else if (*i < 2) {
		printf("Too small line numbers");
		return 1;
	}

	int isEqually = 0;

	if (str[3] != '=') {
		int k;
		for (k = 0, *i = 3; isalpha(str[*i]); k++, (*i) += 1) {
			command_[k] = str[*i];
		}

		if (*i > 8) {
			printf("Too many line numbers");
			return 1;
		} else if (*i < 4) {
			printf("Too small line numbers");
			return 1;
		}

		
		if ((*command = get_command(command_)) == 1) {
			printf("Incorrect command");
			return 1;
		}
	} else {
		isEqually = 1;
		command_ = "EQUALLY";
		if ((*command = get_command(command_)) == 1) {
			printf("Incorrect command");
			return 1;
		}
	}

	for (; !isdigit(str[*i]); (*i)++) { }

	int j;

	int tmp_num_cell[6];
	for (int k = 0; k < 6; k++)
		tmp_num_cell[k] = enemy_;

	for (j = 0; str[*i] != '\0' && str[*i] != ' ' && str[*i] != '\n'; (*i)++, j++) {
		if (isdigit(str[*i])) {
			tmp_num_cell[j] = (int)str[*i] - 48;
		} else {
			if (j == 2 && *num_cell == 0) {
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
			*num_cell += tnc[k];
		} else {
			*num_cell += tnc[k] * (10 * k);
		}
	}

	if (j > 2 && !isEqually) {
		printf("Too many line numbers");
		return 1;
	} else if (j < 2 && !isEqually) {
		printf("Too small line numbers");
		return 1;
	}

	// free(command_); // TODO: why dont work?

	return 0;
}

int asm_translate(char *path_from, char *path_where)
{
	int memory_tmp[100] = { 0 };

	FILE *in = fopen(path_from, "r");

	if (!in) {
		printf("No such file.");
		return 1;
	}

	char *buf = NULL;
	size_t len = 0;

	int count_lines = 0;

	int num_line;
	int command;
	int num_cell;

	while (getline(&buf, &len, in) != -1) {
		int i = 0;
		if (asm_string_parser(buf, &num_line, &command, &num_cell, &i)) {
			fclose(in);
			printf(" in %d line\n", count_lines);
			printf("%s", buf);
			for (; i != 0; i--) {}
			mt_ssetbgcolor(red);
			printf("\E(0");
			printf("o");
			printf("\E(B");
			mt_stopcolor();
			printf(" There is error\n");

			return 1;
		}

		count_lines++;

		if (sc_commandEncode(command, num_cell, &memory_tmp[num_line])) {
			fclose(in);
			printf("Error encode command\n");
			printf(" in %d line\n", count_lines);
			return 1;
		}
	}

	fclose(in);

	FILE *out = fopen(path_where, "w");

	for (int i = 0; i < 100; i++) {
		fprintf(out, "%d ", memory_tmp[i]);
	}

	fclose(out);

	return 0;
}

int main(int argc, char **args)
{
	if (argc == 3) {
		if (asm_translate(args[1], args[2])) {
			printf("Please rewrite your code.\n");
			return 1;
		}
	} else {
		help();
		return 1;
	}

	printf("Finish\n");

	return 0;
}