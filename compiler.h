#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdint.h>
#include "Emulator/memEmu.h"


#define MCBE_MAX_LENGTH 32

typedef struct {
  char lines[MCBE_MAX_LENGTH][16];
  uint8_t length;
} MCBE_FILE;

MCBE_FILE Create_MCBE_File(FILE *file);
uint8_t ASM_TO_BIN(MCBE_FILE file, MEMORY *memory, void(ErrorCallback)(int line, char inst[4], int arg, uint8_t errorCode));

#endif