#include "memory.h"

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
	if (reg == E || reg == D || reg == C || reg == R || reg == P) {
		if (value == 0) {
			flags = flags & (~(reg));
		} if (value == 1) {
			flags = flags | reg;
		} else {
			return 1;
		}
	} else {
		return 1;
	}

	return 0;
}

int sc_regGet(int reg, int *value)
{
	if (!value) {
		return 1;
	}

	if (reg == E) {
		*value = flags & 0x1;
	} else if (reg == D) {
		*value = (flags >> 1) & 0x1;
	} else if (reg == C) {
		*value = (flags >> 2) & 0x1;
	} else if (reg == R) {
		*value = (flags >> 3) & 0x1;
	} else if (reg == P) {
		*value = (flags >> 4) & 0x1;
	} else {
		return 1;
	}

	return 0;
}

int sc_commandEncode(int command, int operand, int *value)
{
	if (!value) {
		return 1;
	}

	// if (command < 10 || command > 76) {
	// 	return 1;
	// }

	if (command < 0x10 ||
		(command > 0x11 && command < 0x20) ||
		(command > 0x21 && command < 0x30) ||
		(command > 0x33 && command < 0x40) ||
		(command > 0x43 && command < 0x51) ||
		command > 0x76) {
		return 1;
	}

	if (operand < 0 || operand > 127) {
		return 1;
	}

	*value = *value | (command << 7);
	*value = *value | operand;

	return 0;
}

int sc_commandDecode(int value, int *command, int *operand)
{
	if (!command || !operand) {
		return 1;
	}

	if ((value >> 14) != 0) {
		return 1;
	}

	*command = value >> 7;
	*operand = value & 0b1111111;

	return 0;
}