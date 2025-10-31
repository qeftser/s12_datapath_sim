
#include "control.h"
#include "memory.h"
#include "latch.h"
#include "isa.h"
#include <stdio.h>

char * isa_names[] = {
   [OP_JMP]    = "JMP",
   [OP_JN]     = "JN",
   [OP_JZ]     = "JZ",
   [OP_LOAD]   = "LOAD",
   [OP_STORE]  = "STORE",
   [OP_LOADI]  = "LOADI",
   [OP_STOREI] = "STOREI",
   [OP_AND]    = "AND",
   [OP_OR]     = "OR",
   [OP_ADD]    = "ADD",
   [OP_SUB]    = "SUB",
   [OP_HALT]   = "HALT",
};

void print_state() {

   /* clear screen... */
   printf("\e[1;1H\e[2J");

   printf("Control:\n");
   printf("\tactive: ");
   for (int i = 0; i < 4; ++i)
      printf("%d ",control.active[i]);
   printf("\n");
   printf("\tIP usage: %d\n\n",control.instruction_pointer_usage);

   if (control.active[0]) {
      printf("L1: %s %03x\n",isa_names[l1.value.opcode],l1.value.address);
      printf("\tinstruction pos: %d\n",l1.instruction_pos);
      printf("\tvalue: [%d,%d]\n",l1.value.opcode,l1.value.address);
   }
   else printf("\n\n\n");

   if (control.active[1]) {
      printf("L2: %s %03x\n",isa_names[l2.instruction],l2.address);
      printf("\tinstruction pos: %d\n",l2.instruction_pos);
      printf("\tinstruction: %d\n",l2.instruction);
      printf("\taddress: %d\n",l2.address);
   }
   else printf("\n\n\n\n");

   if (control.active[2]) {
      printf("L3: %s %03x\n",isa_names[l3.instruction],l3.address);
      printf("\tinstruction pos: %d\n",l3.instruction_pos);
      printf("\tinstruction: %d\n",l3.instruction);
      printf("\taddress: %d\n",l3.address);
      printf("\tread: [%d,%d]\n",l3.read.opcode,l3.read.address);
   }
   else printf("\n\n\n\n\n");

   if (control.active[3]) {
      printf("L4: %s %03x\n",isa_names[l4.instruction],l4.address);
      printf("\tinstruction pos: %d\n",l4.instruction_pos);
      printf("\tinstruction: %d\n",l4.instruction);
      printf("\taddress: %d\n",l4.address);
      printf("\tread: [%d,%d]\n",l4.read.opcode,l4.read.address);
      printf("\tread2: [%d,%d]\n",l4.read2.opcode,l4.read2.address);
      printf("\talu_res: %d\n",l4.alu_res);
      printf("\tzero_flag: %d\n",l4.zero_flag);
      printf("\tnegative_flag: %d\n",l4.negative_flag);
   }
   else printf("\n\n\n\n\n\n\n\n\n");

   printf("Memory:\n");
   printf("\tIP: %d\n",memory.instruction_pointer);
   printf("\tprev IP: %d\n",memory.prev_instruction_pointer);
   printf("\tAccumulator: %d\n\n",memory.accumulator);
   for (int i = 0; i < 16; ++i) {
      for (int j = 0; j < 16; ++j) {
         int pos = j + i * 16;
         if (memory.instruction_pointer == pos)
            printf("\033[46m");
         switch (control.usage[pos]) {
            case USE_WRITE:
               printf("\033[31mW");
               break;
            case USE_READ_1:
               printf("\033[35mX");
               break;
            case USE_READ_2:
               printf("\033[33mR");
               break;
            case USE_READ_3:
               printf("\033[36mY");
               break;
            default:
               printf("_");
               break;
         }
         printf("%03x\033[0m ",
               memslot_to_accumulator(memory.instructions[pos]));
      }
      printf("\n");
   }

}
