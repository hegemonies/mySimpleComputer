#include <stdio.h>
#include <stdlib.h>

#define MEMORY_SIZE 100
#define CELL_SIZE sizeof(int)

//FLAGS
#define E 0b00000001 // перевыполнение при выполнении операции
#define D 0b00000010 // ошибка деления на 0
#define C 0b00000100 // ошибка выхода за границы
#define R 0b00001000 // игнорирование тактовых импульсов
#define P 0b00010000 // неверная команда

int ptr_str[MEMORY_SIZE];

unsigned int flags;

int sc_memoryInit()
{
	for (int i = 0; i < MEMORY_SIZE; i++) {
		ptr_str[i] = 0;
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

void sm_printMemory()
{
	for (int i = 0; i < 10; i++) {
		for (int j = 0; j < 10; j++) {
			printf("+%04d ", ptr_str[i * 10 + j]);
		}
		printf("\n");
	}
}

int sc_regInit()
{
	flags = 0;
	return 0;
}

int sc_regSet(int reg, int value)
{
	if (value == 0 || value == 1) {
		if (reg == E) {
			flags = flags | E;
		}

		if (reg == D) {
			// TODO
		}

		if (reg == C) {
			// TODO
		}

		if (reg == R) {
			// TOOD
		}

		if (reg == P) {
			// TODO
		}
	}

	return 0;
}

int main()
{
	sc_memoryInit();
	//sm_printMemory();
	
	sc_memorySet(0, 9999);
	
	//int tmp = 0;
	//sc_memoryGet(55, &tmp);
	//printf("tmp = %d\n", tmp);
	//sc_memorySave("test.bin");
	
	//sc_memoryLoad("test.bin");
	system("clear");
	sm_printMemory();
}
