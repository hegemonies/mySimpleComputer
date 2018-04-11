#include "cpu.h"
#include "helper.h"

int CU()
{
	int command;
	int operand;
	if (sc_commandDecode(ptr_str[instCount], &command, &operand)) {
		return 1;
	}

	if (operand > 99) {
		return 1;
	}

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
		case ADD:
			accum += ptr_str[operand];
			break;
		case SUB:
			accum -= ptr_str[operand];
			break;
		case DIVIDE:
			accum /= ptr_str[operand];
			break;
		case MUL:
			accum *= ptr_str[operand];
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
		default:
			return 1;
	}


	return 0;
}