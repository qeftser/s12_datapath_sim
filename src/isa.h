
#ifndef __ISA__

#define __ISA__

/* stuff to make it eaiser to work
 * with isa related information. */

typedef enum {
   OP_JMP    = 0b0000,
   OP_JN     = 0b0001,
   OP_JZ     = 0b0010,
   OP_LOAD   = 0b0100,
   OP_STORE  = 0b0101,
   OP_LOADI  = 0b0110,
   OP_STOREI = 0b0111,
   OP_AND    = 0b1000,
   OP_OR     = 0b1001,
   OP_ADD    = 0b1010,
   OP_SUB    = 0b1011,
   OP_HALT   = 0b1111,
} isa_opcode;

#endif
