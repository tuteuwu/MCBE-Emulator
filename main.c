#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "utils.c"

int main(int argc, char *argv[]) {
  FILE *file = fopen(argv[1], "r");
  char lines[32][16];

  // Emulator Data

  uint8_t Memory[32];
  uint8_t ProgramCounter = 0;
  uint8_t InstructionRegister;
  uint8_t Acumulator = 0;

  // End emulator Data
  
  if (file == NULL) {
    printf("Unable to open file.\n");
    return 1;
  }

  int i = 0;
  while (i < 32 && fgets(lines[i], sizeof(lines[i]), file) != NULL) {
    i++;
  }
  fclose(file);


  for (int j = 0; j < i; j++){
    char inst[4] = "---";
    int arg = 0;
    sscanf(lines[j], "%3s %d", inst, &arg);
    if(arg >= 32){
      printf("value out of range. \n\tLine %d | %s %d\n", j, inst, arg);
      return 1;
    }
    Memory[j] = InstructionToBinary(inst, arg);
  }

  printf("| Line \t| Inst\t| Arg\n");
  for(int j = 0; j < i; j++){
    char inst[4] = "---";
    int arg = 0;
    sscanf(lines[j], "%3s %d", inst, &arg);

    printf("| %d \t| %s \t| %d\n", j, inst, arg);

  }
  printf("END\n");

  return 0;
}



