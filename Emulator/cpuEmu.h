#ifndef CPU_H
#define CPU_H

#include <stdint.h>

void StartCPU(void(callbackFunction)(uint8_t pc, uint8_t ir, uint8_t Ac));

#endif