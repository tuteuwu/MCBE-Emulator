#ifndef COMPILER_H
#define COMPILER_H

#include <stdio.h>
#include <stdint.h>
#include "../Emulator/memEmu.h"


#define MCBE_MAX_LENGTH 32

typedef struct {
  char lines[MCBE_MAX_LENGTH][16];
  uint8_t length;
} MCBE_FILE;

typedef enum {
  COMPILE_OK = 0,
  ERR_UNKNOWN_MNEMONIC,
  ERR_RANGE,
  ERR_WRITE_ONLY,
  ERR_READ_ONLY,
} CompileError;

typedef enum { ARG_NONE, ARG_ADDR, ARG_OFFSET, ARG_LITERAL} ArgKind;

typedef enum { OP_INSTRUCTION, OP_DIRECTIVE } OpKind;

typedef struct {
  const char *name;
  OpKind kind;
  uint8_t opcode;
  ArgKind arg;
} Mnemonic;

static const Mnemonic MNEMONICS[] = {
  {"NOP", OP_INSTRUCTION, 0, ARG_NONE   },
  {"HLT", OP_INSTRUCTION, 1, ARG_NONE   },
  {"LD",  OP_INSTRUCTION, 2, ARG_ADDR   },
  {"ST",  OP_INSTRUCTION, 3, ARG_ADDR   },
  {"ADD", OP_INSTRUCTION, 4, ARG_ADDR   },
  {"SUB", OP_INSTRUCTION, 5, ARG_ADDR   },
  {"JMP", OP_INSTRUCTION, 6, ARG_OFFSET },
  {"JZ",  OP_INSTRUCTION, 7, ARG_OFFSET },
  {"SET", OP_DIRECTIVE,   0, ARG_LITERAL}, 
};

typedef struct
{
  const Mnemonic *mnemonic;
  int arg;
} Decoded;

MCBE_FILE Create_MCBE_File(FILE *file);
CompileError ASM_TO_BIN(const MCBE_FILE *file, MEMORY *memory, int *errorLine);
Decoded Mnemonic_Decode(uint8_t byte);

#endif