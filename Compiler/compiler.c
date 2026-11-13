#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>

#include "../Emulator/memEmu.h"
#include "compiler.h"

MCBE_FILE Create_MCBE_File(FILE *file) {
  MCBE_FILE out = {0};
  while (out.length < 32 && fgets(out.lines[out.length], sizeof(out.lines[out.length]), file) != NULL)
    out.length++;
  return out;
}

static const Mnemonic *FindMnemonic(const char *name) {
  for (size_t i = 0; i < sizeof MNEMONICS / sizeof *MNEMONICS; i++)
    if (strcmp(name, MNEMONICS[i].name) == 0)
      return &MNEMONICS[i];
  return NULL;
}

static CompileError Validate(const Mnemonic *m, int arg, int line) {
  switch (m->arg)
  {
    case ARG_NONE:
      return COMPILE_OK;
    case ARG_LITERAL:
      return (arg >= -128 && arg <= 127) ? COMPILE_OK : ERR_RANGE;
    case ARG_OFFSET: {
      int target = line + 1 + arg;
      if (arg < -16 || arg > 15)
        return ERR_RANGE;
      if (target < 0 || target > 29)
        return ERR_RANGE;
      return COMPILE_OK;
    }
    case ARG_ADDR:
      if (arg < 0 || arg >= MEMORY_SIZE)
        return ERR_RANGE;
      if (m->opcode == 2 && arg == 31)
        return ERR_WRITE_ONLY;
      if (m->opcode == 3 && arg == 30)
        return ERR_READ_ONLY;
      return COMPILE_OK;
  }
  return COMPILE_OK;
}

CompileError ASM_TO_BIN(const MCBE_FILE *file, MEMORY *memory, int *errorLine){

  for (int i = 0; i < file->length; i++){

    char intruction[8] = {0};
    int argument = 0;
    int fields = sscanf(file->lines[i], "%7s %d", intruction, &argument);

    if (fields < 1) continue;
    if (fields < 2) argument = 0;

    const Mnemonic *m = FindMnemonic(intruction);
    if(m == NULL){
      *errorLine = i;
      return ERR_UNKNOWN_MNEMONIC;
    }

    CompileError error = Validate(m, argument, i);

    if(error != COMPILE_OK){
      *errorLine = i;
      return error;
    }

    if (m->kind == OP_DIRECTIVE)
      (*memory)[i] = (uint8_t)argument;
    else
      (*memory)[i] = (uint8_t)((m->opcode << 5) | ((uint8_t)argument & 0b00011111));
  }
  return COMPILE_OK;
}

const Mnemonic *MnemonicFromOpcode(uint8_t opcode) {
  for (size_t i = 0; i < sizeof MNEMONICS / sizeof *MNEMONICS; i++)
    if (MNEMONICS[i].kind == OP_INSTRUCTION && MNEMONICS[i].opcode == opcode)
      return &MNEMONICS[i];
  return NULL;
}

Decoded Mnemonic_Decode(uint8_t byte) {
  Decoded d = {NULL, 0};
  uint8_t raw = byte & 0b00011111;

  d.mnemonic = MnemonicFromOpcode((byte & 0b11100000) >> 5);
  if (d.mnemonic == NULL)
    return d;

  switch (d.mnemonic->arg) {
    case ARG_NONE:
      d.arg = 0;
      break;
    case ARG_OFFSET:
      d.arg = (raw & 0b00010000) ? (int)raw - 32 : (int)raw;
      break;
    case ARG_ADDR:
      d.arg = raw;
  }
  return d;
}