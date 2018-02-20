#include "memory.h"

int main()
{
	sc_memoryInit();
	//sm_printMemory();
	
	sc_memorySet(0, 65535);
	
	//int tmp = 0;
	//sc_memoryGet(55, &tmp);
	//printf("tmp = %d\n", tmp);
	//sc_memorySave("test.bin");
	
	//sc_memoryLoad("test.bin");
	// system("clear");
	// sm_printMemory();

	printf("flags = ");
	for(int i = 7; i >= 0; --i) {
		printf("%d", (flags >> i) & 1);
		if (i == 4) {
			printf(" ");
		}
	}
	printf("\n");

	int *val = malloc(sizeof(int));

	sc_commandEncode(26, 3, val);

	printf("value = ");
	for(int i = 14; i >= 0; --i) {
		printf("%d", (*val >> i) & 1);
		if (i % 7 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	int *command = malloc(sizeof(int));
	int *operand = malloc(sizeof(int));
	int value = 0b001100111011001;
	sc_commandDecode(value, command, operand);

	printf("value = ");
	for(int i = 14; i >= 0; --i) {
		printf("%d", (value >> i) & 1);
		if (i % 7 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	printf("command = ");
	for(int i = 7; i >= 0; --i) {
		printf("%d", (*command >> i) & 1);
		if (i % 4 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	printf("operand = ");
	for(int i = 7; i >= 0; --i) {
		printf("%d", (*operand >> i) & 1);
		if (i % 4 == 0) {
			printf(" ");
		}
	}
	printf("\n");

	sm_printMemory();

	return 0;
}