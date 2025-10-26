#ifndef __MEMORY__

#define BYTE unsigned char
#define OPCODE unsigned char
#define ACCUMULATOR short

typedef struct {
  OPCODE opcode;
  BYTE address;
} MemorySlot;

#define memslot_to_accumulator(x) \
   ((ACCUMULATOR)(((ACCUMULATOR)((x).opcode) << 8) | ((x).address)))

// This is all of the memory in the program
// `short`s are not optimal here are they are 16 bits
// and we only need 12, but the memory efficiency gain
// comes at the cost of a lot more complexity for usage
typedef struct {
  MemorySlot instructions[256];
  BYTE instruction_pointer;
  ACCUMULATOR accumulator;
} Memory;

extern Memory memory;

#define __MEMORY__
#endif
