#ifndef MEMORY_H
#define MEMORY_H

#include <stdint.h>

typedef uint8_t memory[32];

uint8_t Memory_Read(uint8_t address);
void Memory_Write(uint8_t address, uint8_t value);

#endif