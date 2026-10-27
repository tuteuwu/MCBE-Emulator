#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

uint8_t InstructionToBinary(char ints[4], int arg){
  uint8_t bin = 0b00000000;
  uint8_t arg_mask = 0b00011111;
  if (strcmp(ints, "NOP") == 0) {
    bin = 0b00000000;
  } else if (strcmp(ints, "HLT") == 0) {
    bin = 0b00100000;
  } else if (strcmp(ints, "LD") == 0) {
    bin = 0b01000000;
  } else if (strcmp(ints, "ST") == 0) {
    bin = 0b01100000;
  } else if (strcmp(ints, "ADD") == 0) {
    bin = 0b10000000;
  } else if (strcmp(ints, "SUB") == 0) {
    bin = 0b10100000;
  } else if (strcmp(ints, "JMP") == 0) {
    bin = 0b11000000;
  } else if (strcmp(ints, "JZ") == 0) {
    bin = 0b11100000;
  } else if (strcmp(ints, "DAT") == 0) {
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


