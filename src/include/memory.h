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

unsigned short int flags;


int sc_memoryInit();
int sc_memorySet(int address, int value);
int sc_memoryGet(int address, int *value);
int sc_memorySave(char *filename);
int sc_memoryLoad(char* filename);
void sm_printMemory();
int sc_regInit();
int sc_regSet(int reg, int value);
int sc_regGet(int register, int *value);
int sc_commandEncode(int command, int operand, int *value);
int sc_commandDecode(int value, int *command, int *operand);