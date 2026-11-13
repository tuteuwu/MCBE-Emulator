#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "Emulator/memEmu.h"
#include "compiler.h"

MCBE_FILE Create_MCBE_File(FILE *file) {
  MCBE_FILE out = {0};
  while (out.length < 32 && fgets(out.lines[out.length], sizeof(out.lines[out.length]), file) != NULL)
    out.length++;
  return out;
}

uint8_t InstructionToBinary(char ints[4], int *arg)
{
  uint8_t bin = 0b0;
  uint8_t arg_mask = 0b00011111;
  if (strcmp(ints, "NOP") == 0)
    bin = 0b000 << 5;
  else if (strcmp(ints, "HLT") == 0)
    bin = 0b001 << 5;
  else if (strcmp(ints, "LD") == 0)
    bin = 0b010 << 5;
  else if (strcmp(ints, "ST") == 0)
    bin = 0b011 << 5;
  else if (strcmp(ints, "ADD") == 0)
    bin = 0b100 << 5;
  else if (strcmp(ints, "SUB") == 0)
    bin = 0b101 << 5;
  else if (strcmp(ints, "JMP") == 0)
    bin = 0b110 << 5;
  else if (strcmp(ints, "JZ") == 0)
    bin = 0b111 << 5;
  else if (strcmp(ints, "SET") == 0)
    arg_mask = 0b11111111;

  bin |= ((uint8_t)*arg & arg_mask);

  return bin;
}

uint8_t ASM_TO_BIN(MCBE_FILE file, MEMORY *memory, void(ErrorCallback)(int line, char inst[4], int arg, uint8_t errorCode)) {
  for (uint8_t i = 0; i < file.length; i++) {
    bool error = false;
    uint8_t errCode = 0;
    char instruction[4] = "";
    int argument = 0;

    sscanf(file.lines[i], "%3s %d", instruction, &argument);

    if (strcmp(instruction, "JMP") == 0 || strcmp(instruction, "JZ") == 0){
      if ((argument < -16 || argument > 15) || (argument + i + 1 > 29 || argument + i + 1 < 0)){
        error = true;
        errCode = 1;
      }
    }
    else if (strcmp(instruction, "SET") == 0) {
      if (argument < -128 || argument > 127){
        error = true;
        errCode = 1;
      }
    } 
    else if (strcmp(instruction, "LD") == 0 && argument == 31){
      error = true;
      errCode = 2;
    }
    else if (strcmp(instruction, "ST") == 0 && argument == 30){
      error = true;
      errCode = 3;
    }
    else if (argument > 31 || argument < 0){
      error = true;
      errCode = 1;
    }
    
    if(error){
      ErrorCallback(i, instruction, argument, errCode);
      return 1;
    }
    (*memory)[i] = InstructionToBinary(instruction, &argument);
  }
  return 0;
}

