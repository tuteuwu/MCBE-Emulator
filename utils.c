#include <stdio.h>
#include <stdint.h>
#include <string.h>

uint8_t InstructionToBinary(char ints[4], int arg){
  uint8_t bin = 0b0;
  uint8_t arg_mask = 0b11111;
  if (strcmp(ints, "NOP") == 0) {
    bin = 0b000 << 5;
  } else if (strcmp(ints, "HLT") == 0) {
    bin = 0b001 << 5;
  } else if (strcmp(ints, "LD") == 0) {
    bin = 0b010 << 5;
  } else if (strcmp(ints, "ST") == 0) {
    bin = 0b011 << 5;
  } else if (strcmp(ints, "ADD") == 0) {
    bin = 0b100 << 5;
  } else if (strcmp(ints, "SUB") == 0) {
    bin = 0b101 << 5;
  } else if (strcmp(ints, "JMP") == 0) {
    bin = 0b110 << 5;
  } else if (strcmp(ints, "JZ") == 0) {
    bin = 0b111 << 5;
  } else if (strcmp(ints, "SET") == 0) {
    arg_mask = 0b11111111;
  }

  bin |= ((uint8_t)arg & arg_mask);

  return bin;
}


char *ToBinaryString(uint8_t bin) {
  static char str[9];
  for (int i = 7; i >= 0; i--) {
    str[7 - i] = ((bin >> i) & 1) + '0';
  }

  str[8] = '\0';
  return str;
}

static inline void NewLine(void) {
  putchar('\n');
}
