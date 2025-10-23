#ifndef __MEMORY__

// This is all of the memory in the program
// `short`s are not optimal here are they are 16 bits 
// and we only need 12, but the memory efficiency gain
// comes at the cost of a lot more complexity for usage
typedef struct {
	unsigned char instructions[256];
	unsigned char instruction_pointer;
	short accumulator;
} Memory;

#define __MEMORY__
#endif
