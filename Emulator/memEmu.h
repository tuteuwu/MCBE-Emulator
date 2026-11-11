#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>
#define MEMORY_SIZE 32
typedef uint8_t MEMORY[MEMORY_SIZE];

void InitMemory(MEMORY *mem);

uint8_t Memory_Read(uint8_t address);
void Memory_Write(uint8_t address, uint8_t value);

#endif