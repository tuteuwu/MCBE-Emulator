#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "utils.c"
#include "Emulator/memEmu.h"
#include "Emulator/cpuEmu.h"
#include "lib/libflags/libflags.h"
#include "Compiler/compiler.h"

void runtimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac);
void compileError(CompileError error, int line, char codeLine[16]);
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
  MEMORY mem = {0};

  // Compiling And memory load
  int errorLine = 0;
  CompileError err = ASM_TO_BIN(&mcbeFile, &mem, &errorLine);

  if (err != COMPILE_OK){
    compileError(err, errorLine, mcbeFile.lines[errorLine]);
    return 1;
  }

  // Init Memory
  InitMemory(&mem);
  
  // Print Preview
  if(flags.enabledFlags[0]) preview();

  // Start CPU 
  StartCPU(&runtimeLogs);

  return 0;
}

void runtimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac){
  if (flags.enabledFlags[1]) {
    Decoded d = Mnemonic_Decode(ir);
    if(d.mnemonic != NULL) {
      printf("\t\t│ PC: %02d, IR: 0x%02x - 0b%s, Accummulator: %d\n\
              \t│ ASM:%3s %d\n\n",
             pc, ir, ToBinaryString(ir), Ac,
             d.mnemonic->name, d.arg);
    }
  }
}

void preview(){
  NewLine();
  printf("┌───────┬───────┬───────┬───────┬────────────┐\n");
  printf("│ Line\t│ Inst\t│ Arg \t│ Hex \t│ Binary     │\n");
  printf("├───────┼───────┴───────┼───────┼────────────┤\n");
  for (int i = 0; i < mcbeFile.length; i++) {
    char inst[4] = "---";
    int arg = 0;
    uint8_t memPeek = Memory_Peek(i);
    sscanf(mcbeFile.lines[i], "%3s %d", inst, &arg);
    printf("│ %d \t│ %s \t   %d \t│ 0x%02x\t│ 0b%s │\n", i, inst, arg, memPeek, ToBinaryString(memPeek));
  }
  printf("└───────┴───────────────┴───────┴────────────┘\n");
  NewLine();
}

void compileError(CompileError error, int line, char codeLine[16]) {
  switch (error) {
    case ERR_RANGE:
      printf("Value out of range!\n\t Line: %d | %s", line, codeLine);
      break;
    case ERR_READ_ONLY:
      printf("Memory Address 30 is Read-Only!\n\t Line: %d | %s", line, codeLine);
      break;
    case ERR_WRITE_ONLY:
      printf("Memory Address 31 is Write-Only!\n\t Line: %d | %s", line, codeLine);
      break;
    case ERR_UNKNOWN_MNEMONIC:
      printf("Mnemonic unknown!\n\t Line: %d | %s", line, codeLine);
      break;
    default:
      printf("Unknown error!\n\t Line: %d | %s", line, codeLine);
      break;
  }
  NewLine();
}
