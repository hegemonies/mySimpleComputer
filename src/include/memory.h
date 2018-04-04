#ifndef MEMORY_H
#define MEMORY_H

#include "terminal.h"

#define MEMORY_SIZE 100
#define CELL_SIZE sizeof(int)

//FLAGS
#define OD 0b00000001 // переполнение при выполнении операции 
					 // overfulfillment during operation
#define DE 0b00000010 // ошибка деления на 0
					 // division error by 0
#define EG 0b00000100 // ошибка выхода за границы памяти
					 // error of going beyond borders
#define CI 0b00001000 // игнорирование тактовых импульсов
					 // clock ignoring
#define IC 0b00010000 // неверная команда
					 // invalid command


int ptr_str[MEMORY_SIZE];

unsigned short int flags;

int accum;

int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char* filename);
void sm_printMemory(int x, int y);
int sc_regInit();
int sc_regSet(int reg, int value);
int sc_regGet(int register, int *value);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int value, int *command, int *operand);

#endif
