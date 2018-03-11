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
		sc_regSet(EG, 1);
		return 1;
	}

	ptr_str[address] = value;
	
	return 0;
}

int sc_memoryGet(int address, int *value)
{
	if (address < 0 || address > 99) {
		sc_regSet(EG, 1);
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

void sm_printMemory(int x, int y)
{
	for (int i = 0; i < 10; i++) {
		mt_gotoXY(y + i, x);
		for (int j = 0; j < 10; j++) {
			printf("+%04x ", ptr_str[i * 10 + j]);
		}
	}
}

int sc_regInit()
{
	flags = 0;
	return 0;
}

int sc_regSet(int reg, int value)
{
	if (reg == OD || reg == DE || reg == EG || reg == CI || reg == IC) {
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

	if (reg == OD) {
		*value = flags & 0x1;
	} else if (reg == DE) {
		*value = (flags >> 1) & 0x1;
	} else if (reg == EG) {
		*value = (flags >> 2) & 0x1;
	} else if (reg == CI) {
		*value = (flags >> 3) & 0x1;
	} else if (reg == IC) {
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

	if (command < 10 ||
		(command > 11 && command < 20) ||
		(command > 21 && command < 30) ||
		(command > 33 && command < 40) ||
		(command > 43 && command < 51) ||
		command > 76) {
		sc_regSet(IC, 1);
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