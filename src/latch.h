
#ifndef __LATCHES__

#define __LATCHES__
#include "memory.h"

/* the latches used in the pipeline. These are 
 * oneoffs that store information unique to each
 * stage.                                       */

typedef struct {
   MemorySlot value;
} L1;

typedef struct {
   OPCODE instruction;
   BYTE address;
} L2;

typedef struct {
   OPCODE instruction;
   BYTE address;
   MemorySlot read;
} L3;

typedef struct {
   unsigned int zero_flag:1;
   unsigned int negative_flag:1;
   OPCODE instruction;
   BYTE address;
   MemorySlot read;
   ACCUMULATOR alu_res;
   MemorySlot read2;
} L4;

extern L1 l1;
extern L2 l2;
extern L3 l3;
extern L4 l4;

#endif
