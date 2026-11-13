#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "utils.c"
#include "Emulator/memEmu.h"
#include "Emulator/cpuEmu.h"
#include "lib/libflags/libflags.h"
#include "compiler.h"

void runtimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac);
void compileError(int line, char inst[4], int arg, uint8_t errorCode);
void preview();

FLAGS flags;
MCBE_FILE mcbeFile;

int main(int argc, char *argv[]) {
  
  if(argc < 2) return 1;

  
  flags = CreateFlags(2);
  flags.flags[0] = 'v';
  flags.flags[1] = 'l';

  int flagsParseStatus = FlagParse(&flags, &argc, argv);

  if(flagsParseStatus != 0) {
    printf("Not valid flags\n");
    return 1;
  }

  FILE *file = fopen(argv[1], "r");
  
  if (file == NULL) {
    printf("Unable to open file.\n");
    return 1;
  }
  
  mcbeFile = Create_MCBE_File(file);
  fclose(file);

  // Emulator Data
  MEMORY mem;

  // Reset Memory
  for(int i = 0; i < 32; i++)
    mem[i] = 0;

  // Load to Memory
  ASM_TO_BIN(mcbeFile, &mem, &compileError);

  // Init Memory
  InitMemory(&mem);
  
  // Print Preview
  if(flags.enabledFlags[0]) preview();

  // Start CPU 
  StartCPU(&runtimeLogs);

  return 0;
}


void runtimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac){
  if (flags.enabledFlags[1]){
    char instAndArg[16];
    char inst[4] = "---";
    int arg = 0;
    sscanf(mcbeFile.lines[pc], "%3s %d", inst, &arg);
    sprintf(instAndArg, "%3s  %+2d", inst, arg);
    printf(" ASM: %s \t| PC: %d \t| IR: 0x%02x - 0b%s \t| Accumulator: %d \n", instAndArg, pc, ir, ToBinaryString(ir), Ac);
  }
    
}

void preview(){
  NewLine();
  printf("┌───────┬───────┬───────┬───────┬────────────┐\n");
  printf("│ Line\t│ Inst\t│ Arg \t│ Hex \t│ Binary     │\n");
  printf("├───────┼───────┼───────┼───────┼────────────┤\n");
  for (int i = 0; i < mcbeFile.length; i++)
  {
    char inst[4] = "---";
    int arg = 0;
    uint8_t memPeek = Memory_Peek(i);
        sscanf(mcbeFile.lines[i], "%3s %d", inst, &arg);
        printf("│ %d \t│ %s \t┇ %d \t│ 0x%02x\t│ 0b%s │\n", i, inst, arg, memPeek, ToBinaryString(memPeek));
  }
  printf("└───────┴───────┴───────┴───────┴────────────┘\n");
  NewLine();
}

void compileError(int line, char inst[4], int arg, uint8_t errorCode) {
  switch (errorCode)
  {
  case 1:
    printf("Value out of range. \n\t LINE: %d | %s %d", line, inst, arg);
    break;
  case 2:
    printf("Memory address 31 is Read-Only. \n\t LINE: %d | %s %d", line, inst, arg);
    break;
  case 3:
    printf("Memory address 30 is Write-Only. \n\t LINE: %d | %s %d", line, inst, arg);
    break;

  default:
    printf("Unknown error. \n\t LINE: %d | %s %d", line, inst, arg);
    break;
  }
}
