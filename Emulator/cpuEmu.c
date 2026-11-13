#include <stdlib.h>
#include <stdint.h>
#include "memEmu.h"

// Functions
void fetch(uint8_t  *pc, uint8_t  *ir);
uint8_t decode(uint8_t *ir, uint8_t *argOut);
int8_t decodeOffset(uint8_t arg);
void execute(uint8_t inst, uint8_t arg);
void InitCycle();

// ISA
void HLT(bool *b);
void LD(uint8_t *ac, uint8_t arg);
void ST(uint8_t *ac, uint8_t arg);
void ADD(uint8_t *ac, uint8_t arg);
void SUB(uint8_t *ac, uint8_t arg);
void JMP(uint8_t *pc, int8_t arg);
void JZ(uint8_t *pc, uint8_t ac, int8_t arg);

// Registers
static uint8_t ProgramCounter = 0;
static uint8_t InstructionRegister = 0;
static uint8_t Accumulator = 0;

// Status
bool isOn = false;
void (*callback)(uint8_t pc, uint8_t ir, uint8_t Ac);

void StartCPU(void (callbackFunction)(uint8_t pc, uint8_t ir, uint8_t Ac)){
  // Reset Registers
  ProgramCounter = 0;
  InstructionRegister = 0;
  Accumulator = 0;
  // Enable
  isOn = true;
  callback = callbackFunction;
  // Start
  InitCycle();
}

void InitCycle(){
  while (isOn){
    // Intruction Cycle
    uint8_t inst;
    uint8_t arg;
    fetch(&ProgramCounter, &InstructionRegister);
    inst = decode(&InstructionRegister, &arg);
    callback(ProgramCounter, InstructionRegister, Accumulator);
    ProgramCounter++;
    execute(inst, arg);
  }
}

void execute(uint8_t inst, uint8_t arg){
  switch(inst){
    case 0: break;
    case 1: HLT(&isOn); break;
    case 2: LD(&Accumulator, arg); break;
    case 3: ST(&Accumulator, arg); break;
    case 4: ADD(&Accumulator, arg); break;
    case 5: SUB(&Accumulator, arg); break;
    case 6: JMP(&ProgramCounter, decodeOffset(arg)); break;
    case 7: JZ(&ProgramCounter, Accumulator, decodeOffset(arg)); break;
  }
}

void fetch(uint8_t  *pc, uint8_t  *ir){
  *ir = Memory_Read(*pc);
}

uint8_t decode(uint8_t *ir, uint8_t *argOut){
  *argOut = *ir & 0b00011111;
  return ( *ir & (0b111 << 5) ) >> 5;
}

int8_t decodeOffset(uint8_t arg) {
  return (arg & 0b00010000)
    ? (int8_t)(arg | 0b11100000)
    : (int8_t)arg;
}

void HLT(bool *b) {
  *b = false;
}
void LD(uint8_t *ac, uint8_t arg){
  *ac = Memory_Read(arg);
}
void ST(uint8_t *ac, uint8_t arg){
  Memory_Write(arg, *ac);
}
void ADD(uint8_t *ac, uint8_t arg){
  *ac += Memory_Read(arg);
}
void SUB(uint8_t *ac, uint8_t arg){
  *ac -= Memory_Read(arg);
}
void JMP(uint8_t *pc, int8_t arg){
  *pc += arg;
}
void JZ(uint8_t *pc, uint8_t ac, int8_t arg){
  if(ac == 0){
    *pc += arg;
  }
}