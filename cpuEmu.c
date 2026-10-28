#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>
#include "ttyEmu.c"

// Functions
void fetch(uint8_t *mem, uint8_t  *pc, uint8_t  *ir);
uint8_t decode(uint8_t *ir, uint8_t *argOut);
int8_t decodeOffset(uint8_t arg);
void InstructionCycle();

// ISA
void NOP();
void HLT(bool *b);
void LD(uint8_t *ac, uint8_t *mem, uint8_t arg);
void ST(uint8_t *ac, uint8_t *mem, uint8_t arg);
void ADD(uint8_t *ac, uint8_t *mem, uint8_t arg);
void SUB(uint8_t *ac, uint8_t *mem, uint8_t arg);
void JMP(uint8_t *pc, int8_t arg);
void JZ(uint8_t *pc, uint8_t ac, int8_t arg);

// Registers
static uint8_t Memory[32];
static uint8_t ProgramCounter = 0;
static uint8_t InstructionRegister = 0;
static uint8_t Accumulator = 0;

// Status
static bool isOn = false;
int (*logCallback)(uint8_t pc, uint8_t ir, uint8_t Ac);

void Initialize(uint8_t mem[32], int (logFunction)(uint8_t pc, uint8_t ir, uint8_t Ac)){
  for(int i = 0; i < 32; i++){
    Memory[i] = mem[i];
  }
  ProgramCounter = 0;
  InstructionRegister = 0;
  Accumulator = 0;
  isOn = true;
  logCallback = logFunction;
}

void InitCycle(bool logs){
  while (isOn){
    InstructionCycle();
    if(logs){
      logCallback(ProgramCounter, InstructionRegister, Accumulator);
    }

  }
}

void InstructionCycle(){
  fetch(Memory, &ProgramCounter, &InstructionRegister);
  uint8_t arg;
  uint8_t inst;
  inst = decode(&InstructionRegister, &arg);
  ProgramCounter++;
  switch(inst){
    case 0: NOP(); break;
    case 1: HLT(&isOn); break;
    case 2: LD(&Accumulator, Memory, arg); break;
    case 3: ST(&Accumulator, Memory, arg); break;
    case 4: ADD(&Accumulator, Memory, arg); break;
    case 5: SUB(&Accumulator, Memory, arg); break;
    case 6: JMP(&ProgramCounter, decodeOffset(arg)); break;
    case 7: JZ(&ProgramCounter, Accumulator, decodeOffset(arg)); break;
  }
}

void fetch(uint8_t *mem, uint8_t  *pc, uint8_t  *ir){
  *ir = mem[*pc];
}

uint8_t decode(uint8_t *ir, uint8_t *argOut){
  *argOut = *ir & 0b00011111;
  return ( *ir & 0b11100000 ) >> 5;
}

int8_t decodeOffset(uint8_t arg) {
  return (arg & 0x10)
    ? (int8_t)(arg | 0xE0)
    : (int8_t)arg;
}


void NOP(){}
void HLT(bool *b){
  *b = false;
}
void LD(uint8_t *ac, uint8_t *mem, uint8_t arg){
  if(arg == 30){
    getUserInput(&mem[30]);
  }
  *ac = mem[arg];
}
void ST(uint8_t *ac, uint8_t *mem, uint8_t arg){
  mem[arg] = *ac;
  if(arg == 31){
    printOutput(mem[31]);
  }
}
void ADD(uint8_t *ac, uint8_t *mem, uint8_t arg){
  if(arg == 30){
    getUserInput(&mem[30]);
  }
  *ac += mem[arg];
}
void SUB(uint8_t *ac, uint8_t *mem, uint8_t arg){
  if(arg == 30){
    getUserInput(&mem[30]);
  }
  *ac -= mem[arg];
}
void JMP(uint8_t *pc, int8_t arg){
  *pc += arg;
}
void JZ(uint8_t *pc, uint8_t ac, int8_t arg){
  if(ac == 0){
    *pc += arg;
  }
}