#include "asm.h"

void help()
{
	printf("Help:\n");
	printf("./sat [filename from [*.a]] [filename where [*.o]]\n");
}

int asm_string_parser(char *str, int *num_str, int *command, int *num_cell)
{
	int i;
	char *command_ = calloc(0, sizeof(char) * 7);
	*num_str = 0;
	*num_cell = 0;

	int m;
	for (m = 0; str[m] != '\n'; m++) { }
	str[m - 1] = 0;
	// printf("buf = %s\nm = %d\n", str, m);


	for (i = 0; str[i] != ' '; i++) {
		if (isdigit(str[i])) {
			if (i == 0) {
				*num_str += ((int)str[i] - 48) * (10 * i);
			} else {
				*num_str += ((int)str[i] - 48);
			}
		} else {
			printf("Error incorrect format number");
			return 1;
		}
	}

	if (i > 2) {
		printf("Too many line numbers");
		return 1;
	} else if (i < 2) {
		printf("Too small line numbers");
		return 1;
	}
	int k;
	for (k = 0, i = 3; isalpha(str[i]); k++, i++) {
		command_[k] = str[i];
	}

	printf("command_ = %s\n", command_);
	printf("command number = %d\n", get_command(command_));
	printf("i = %d\n", i);

	// printf("num_str = %d\n", *num_str);

	if (i > 8) {
		printf("Too many line numbers");
		return 1;
	} else if (i < 4) {
		printf("Too small line numbers");
		return 1;
	}

	
	if ((*command = get_command(command_)) == 1) {
		printf("Incorrect command");
		return 1;
	}

	for (; !isdigit(str[i]) ; i++) { }

	int j;
	for (j = 0; str[i] != '\0'; i++, j++) {
		// printf("digit test = %d\n", isdigit(str[i]));
		// printf("%c", str[i]);
		if (isdigit(str[i])) {
			// printf("che\n");
			if (j == 0) {
				*num_cell += ((int)str[i] - 48) * (10 * j + 1);
			} else {
				if (((int)str[i] - 48) == 0) {
					*num_cell *= 10;
					continue;
				}
				*num_cell += ((int)str[i] - 48);
			}
		} else {
			if (j == 2 && *num_cell == 0) {
				break;
			} else {
				printf("Error incorrect format number");
				return 1;	
			}
		}
		// printf("num_cell = %d\n", *num_cell);
	}

	if (j > 2) {
		printf("Too many line numbers");
		return 1;
	} else if (j < 2) {
		printf("Too small line numbers");
		return 1;
	}

	return 0;
}

int asm_translate(char *path_from, char *path_where)
{
	int memory_tmp[100] = { 0 };

	FILE *in = fopen(path_from, "r");

	char *buf = NULL;
	size_t len = 0;

	int count_lines = 0;

	int num_line;
	int command;
	int num_cell;

	while (getline(&buf, &len, in) != -1) {
		if (asm_string_parser(buf, &num_line, &command, &num_cell)) {
			fclose(in);
			printf(" in %d line\n", count_lines);
			return 1;
		}

		printf("num_line = %d\n", num_line);
		printf("command = %d\n", command);
		printf("num_cell = %d\n\n", num_cell);

		count_lines++;

		if (sc_commandEncode(command, num_cell, &memory_tmp[num_line])) {
			fclose(in);
			printf("Error encode command\n");
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