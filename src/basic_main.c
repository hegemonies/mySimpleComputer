#include "basic.h"

int main(int argc, char **args)
{
	if (argc == 3) {
		cell_number_for_variables = 100;
		if (basic_translator(args[1], args[2])) {
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