#include "basic.h"

int main(int argc, char **args)
{
	// if (argc == 3) {
	// 	cell_number_for_variables = 100;
	// 	if (basic_translator(args[1], args[2])) {
	// 		printf("Please rewrite your code.\n");
	// 		return 1;
	// 	}
	// } else {
	// 	help();
	// 	return 1;
	// }
	cell_number_for_variables = 100;
	// basic_translator("t.bsc", "t.o");
	// basic_translator("test_if.bsc", "t.o");
	basic_translator("test_let.bsc", "t.o");
	
	// Stack *a = malloc(sizeof(Stack));

	// init_stack(a);

	// push_stack(a, 'a');
	// push_stack(a, 'b');

	// printf("a = %d\n", pop_stack(a));
	// printf("a = %c\n", pop_stack(a));

	// free(a);

	printf("Finish\n");

	return 0;
}