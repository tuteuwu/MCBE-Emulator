#include <stdlib.h>
#include <stdint.h>
#include "ttyEmu.c"

#define MEMORY_SIZE 32

typedef uint8_t MEMORY[MEMORY_SIZE];

static MEMORY systemMemory;

void InitMemory(MEMORY *mem){
  for (uint8_t i = 0; i < MEMORY_SIZE; i++)
    systemMemory[i] = (*mem)[i];
}

uint8_t Memory_Read(uint8_t address){
  if(address == 30)
    getUserInput(&systemMemory[address]);

  return systemMemory[address];
}
void Memory_Write(uint8_t address, uint8_t value){
  systemMemory[address] = value;
  if (address == 31)
    printOutput(systemMemory[address]);
  
}

uint8_t Memory_Peek(uint8_t address) {
  return (address < MEMORY_SIZE) ? systemMemory[address] : 0;
}