#include "cpu.h"
#include "helper.h"

int ALU(int command, int operand)
{
	if (operand > 99) {
		return 1;
	}

	switch (command) {
		case ADD:
			accum += ptr_str[operand];
			break;
		case SUB:
			accum -= ptr_str[operand];
			break;
		case DIVIDE:
			if (ptr_str[operand] == 0) {
				sc_regSet(DE, 1);
				break;
			}
			accum /= ptr_str[operand];
			break;
		case MUL:
			accum *= ptr_str[operand];
			break;
		default:
			return 1;
	}


	return 0;
}

int CU()
{
	int command = 0;
	int operand = 0;

	if (sc_commandDecode(ptr_str[instCount], &command, &operand)) {
		printf("che 1\n");
		return 1;
	}

	if (command > 33 || command < 30) {
		switch (command) {
			case READ:
				break;
			case WRITE:
				break;
			case LOAD:
				accum = ptr_str[operand];
				break;
			case STORE:
				ptr_str[operand] = accum;
				break;

			case JUMP:
				instCount = operand;
				break;
			case JNEG:
				if (accum < 0) {
					instCount = operand;
				}
				break;
			case JZ:
				if (accum == 0) {
					instCount = operand;
				}
				break;
			case HALT:
				return 2;
				break;
		}
		printf("che 2\n");
	} else {
		printf("che 3\n");
		if (ALU(command, operand)) {
			printf("che 4\n");
			return 1;
		}
	}
	
	instCount++;

	return 0;
}