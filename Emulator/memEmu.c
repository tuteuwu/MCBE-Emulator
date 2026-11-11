#include <stdlib.h>
#include <stdint.h>

#define MEMORY_SIZE 32

typedef uint8_t memory[MEMORY_SIZE];

memory ram;


void InitMemory(memory mem){
  for(int i = 0; i < MEMORY_SIZE; i++) ram[i] = mem[i];
}

uint8_t Memory_Read(uint8_t address){
  return ram[address];
}
void Memory_Write(uint8_t address, uint8_t value){
  ram[address] = value;
}
