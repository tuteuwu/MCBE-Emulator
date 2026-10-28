#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "utils.c"
#include "cpuEmu.c"

int runTimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac);

int main(int argc, char *argv[]) {
  int fileArg = 1;
  bool logs = false;
  if(strcmp(argv[1], "-l") == 0){
    printf("Logs enabled\n");
    fileArg = 2;
    logs = true;
  }

  

  FILE *file = fopen(argv[fileArg], "r");
  char lines[32][16];

  // Emulator Data

  uint8_t Memory[32];
  for(int i = 0; i < 32; i++){
    Memory[i] = 0;
  }
  
  if (file == NULL) {
    printf("Unable to open file.\n");
    return 1;
  }

  int linesCounter = 0;
  while (linesCounter < 32 && fgets(lines[linesCounter], sizeof(lines[linesCounter]), file) != NULL) {
    linesCounter++;
  }
  fclose(file);
  // Load to Memory
  for (int j = 0; j < linesCounter; j++){
    char inst[4] = "";
    int arg = 0;
    int n = sscanf(lines[j], "%3s %d", inst, &arg);

    if(strcmp(inst, "JMP") == 0 || strcmp(inst, "JZ") == 0){
      if(arg < -16 || arg > 15){
        printf("value out of range. \n\tLine %d | %s %d\n", j, inst, arg);
        return 1;
      }
    } else if(strcmp(inst, "SET") == 0) {
      if(arg < -128 || arg > 127){
        printf("value out of range. \n\tLine %d | %s %d\n", j, inst, arg);
        return 1;
      }
    } else {
      if (arg >= 32 || arg < 0){
        printf("value out of range. \n\tLine %d | %s %d\n", j, inst, arg);
        return 1;
      }
    }

    Memory[j] = InstructionToBinary(inst, arg);
  }


  // Start CPU 
  Initialize(Memory, &runTimeLogs);
  InitCycle(logs);

  // Print Preview
  if(logs){
    printf("\n| Line \t| Inst\t| Arg\t| Hex\n");
    for(int j = 0; j < linesCounter; j++){
      char inst[4] = "---";
      int arg = 0;
      sscanf(lines[j], "%3s %d", inst, &arg);
      printf("| %d \t| %s \t| %d \t| %x\n", j, inst, arg, Memory[j]);
    }
  }


  return 0;
}


int runTimeLogs(uint8_t pc, uint8_t ir, uint8_t Ac){
  printf("| PC: %d \t| IR: 0x%x \t| Accumulator: %d \n", pc, ir, Ac);
}

