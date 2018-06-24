#include "basic.h"

int main(int argc, char **args)
{
	if (argc == 3) {
		cell_number_for_variables = 100;
		int i = 0;
		if (basic_translator(args[1], args[2], &i)) {
			printf(" in %d line\n", i);
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