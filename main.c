#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 100
#define CELL_SIZE sizeof(int)

//FLAGS
#define OA 1 // output abroad


int *ptr_str = NULL;

int sc_memoryInit()
{
	// static int *ptr[100] = { 0 };
	ptr_str = calloc(100, sizeof(int));
	
	if (!ptr_str) {
		return 1;
	}
	
	return 0;
}

int sc_memorySet(int address, int value)
{
	if (address < 0 || address > 99) {
		//flag error;
		return 1;
	}
	ptr_str[address] = value;
	
	return 0;
}

int sc_memoryGet(int address, int *value)
{
	if (address < 0 || address > 99) {
		return 1;
	}
	
	*value = ptr_str[address];
	
	return 0;
} 

int sc_memorySave(char *filename)
{
	FILE *ptrFile = fopen(filename, "wb");

	fwrite(ptr_str, CELL_SIZE, MEMORY_SIZE, ptrFile);
	fclose(ptrFile);
	return 0;
}

int sc_memoryLoad(char* filename)
{
	FILE *ptrFile = fopen(filename, "rb");
	
	fread(ptr_str, CELL_SIZE, MEMORY_SIZE, ptrFile);
	
	fclose(ptrFile);

	return 0;
}

int main()
{
	sc_memoryInit();
	for (int i = 0; i < 100; i++) {
		printf("%d = %d\n", i, ptr_str[i]);
	}
	
	//sc_memorySet(55, 942);
	
	//int tmp = 0;
	//sc_memoryGet(55, &tmp);
	//printf("tmp = %d\n", tmp);
	//sc_memorySave("test.bin");
	
	sc_memoryLoad("test.bin");
	for (int i = 0; i < 100; i++) {
		printf("%d = %d\n", i, ptr_str[i]);
	}
}
