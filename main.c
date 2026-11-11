#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utils.c"
#include "Emulator/memEmu.h"
#include "Emulator/cpuEmu.h"

void runtimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac);

bool logs = false;

int main(int argc, char *argv[]) {
  int fileArg = 1;
  
  if(argc < 2) return 1;
  if(strcmp(argv[1], "-l") == 0){
    printf("Logs enabled\n\n\n");
    fileArg = 2;
    logs = true;
  }

  

  FILE *file = fopen(argv[fileArg], "r");
  char lines[32][16];
  if (file == NULL) {
    printf("Unable to open file.\n");
    return 1;
  }
  uint8_t linesCounter = 0;
  while (linesCounter < 32 && fgets(lines[linesCounter], sizeof(lines[linesCounter]), file) != NULL)
    linesCounter++;
  fclose(file);

  // Emulator Data
  MEMORY mem;

  // Reset Memory
  for(int i = 0; i < 32; i++)
    mem[i] = 0;

  // Load to Memory
  for (int i = 0; i < linesCounter; i++) {
    char inst[4] = "";
    int arg = 0;
    sscanf(lines[i], "%3s %d", inst, &arg);

    if(strcmp(inst, "JMP") == 0 || strcmp(inst, "JZ") == 0) {
      if ((arg < -16 || arg > 15) || (arg + i > 29 || arg + i < 0)){
        printf("value out of range. \n\tLine %d | %s %d\n", i, inst, arg);
        return 1;
      }
    }
    else if(strcmp(inst, "SET") == 0) {
      if(arg < -128 || arg > 127){
        printf("value out of range. \n\tLine %d | %s %d\n", i, inst, arg);
        return 1;
      }
    }
    else if (strcmp(inst, "LD") == 0 && arg == 31) {
      printf("Memory address 31 is Write-Only. \n\tLine %d | %s %d\n", i, inst, arg);
      return 1;
    }
    else if (strcmp(inst, "ST") == 0 && arg == 30) {
      printf("Memory address 30 is Read-Only. \n\tLine %d | %s %d\n", i, inst, arg);
      return 1;
    }
    else {
      if (arg >= 32 || arg < 0) {
        printf("value out of range. \n\tLine %d | %s %d\n", i, inst, arg);
        return 1;
      }
    }
    mem[i] = InstructionToBinary(inst, arg);
  }

  // Init Memory
  InitMemory(&mem);

  // Print Preview
  NewLine();
  printf("┌───────┬───────┬───────┬───────┬────────────┐\n");
  printf("│ Line\t│ Inst\t│ Arg \t│ Hex \t│ Binary     │\n");
  printf("├───────┼───────┼───────┼───────┼────────────┤\n");
  for (int i = 0; i < linesCounter; i++)
  {
    char inst[4] = "---";
    int arg = 0;
    sscanf(lines[i], "%3s %d", inst, &arg);
    printf("│ %d \t│ %s \t│ %d \t│ 0x%x\t│ 0b%s │\n", i, inst, arg, Memory_Read(i), ToBinaryString(Memory_Read(i)));
  }
  printf("└───────┴───────┴───────┴───────┴────────────┘\n");
  printf(" Waiting for Keypress ");
  getchar();

  // Start CPU 
  StartCPU(&runtimeLogs);

  return 0;
}


void runtimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac){
  if (logs)
    printf("| PC: %d \t| IR: 0x%x - 0b%s \t| Accumulator: %d \n", pc, ir, ToBinaryString(ir), Ac);
}

