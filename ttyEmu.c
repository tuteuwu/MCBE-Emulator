#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

void getUserInput(uint8_t *output){
  int input = 0;
  printf("Input > ");
  if ( scanf("%d", &input) == 1 ){
    if(input > 127 || input < -128){
      printf("Not valid input. Using 0\n");
      *output = (uint8_t)input;
    }else{
      *output = (uint8_t)input;
    }
  } else {
    printf("Not valid input. Using 0\n");
    *output = (uint8_t)input;
  }
}

void printOutput(uint8_t input){
  printf("Output > ");
  printf("%d\n", (int8_t)input);
}