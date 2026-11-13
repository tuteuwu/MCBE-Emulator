#include <stdio.h>
#include <stdint.h>
#include <string.h>



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
