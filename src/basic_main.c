#include "basic.h"

int main(int argc, char **args)
{
	// if (argc == 3) {
	// cell_number_for_variables = 100;
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


	// add_var('A', get_cellNumberForNewVariables());
	// add_var('B', get_cellNumberForNewVariables());
	// add_var('C', get_cellNumberForNewVariables());

	// var *av = get_var('A');
	// var *bv = get_var('B');
	// var *cv = get_var('C');
	
	// Stack *a = malloc(sizeof(Stack));

	// init_stack(a);

	// push_stack(a, bv->name);
	// push_stack(a, av->name);
	// push_stack(a, cv->name);

	// printf("a = %c\n", pop_stack(a));
	// printf("a = %c\n", pop_stack(a));
	// printf("a = %c\n", pop_stack(a));

	// printf("av->num_cell = %d\n", av->num_cell);
	// printf("bv->num_cell = %d\n", bv->num_cell);
	// printf("cv->num_cell = %d\n", cv->num_cell);

	// free(a);

	printf("Finish\n");

	return 0;
}