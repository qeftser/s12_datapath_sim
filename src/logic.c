
#include "logic.h"
#include "memory.h"
#include "latch.h"
#include "isa.h"
#include "control.h"

void l1_read() {
   l1.value = memory.instructions[memory.prev_instruction_pointer];
   l1.instruction_pos = memory.prev_instruction_pointer;
}

void l2_decode() {
   l2.instruction = l1.value.opcode;
   l2.address = l1.value.address;
   l2.instruction_pos = l1.instruction_pos;
}

void l3_forward() {
   l3.instruction = l2.instruction;
   l3.address = l2.address;
   l3.instruction_pos = l2.instruction_pos;
}

void l3_read() {
   l3.read = memory.instructions[l2.address];
}

void l4_forward() {
   l4.instruction = l3.instruction;
   l4.address = l3.address;
   l4.read = l3.read;
   l4.instruction_pos = l3.instruction_pos;
}

void l4_flags() {
   if (memory.accumulator < 0)
      l4.negative_flag = 1;
   else l4.negative_flag = 0;

   if (memory.accumulator == 0)
      l4.zero_flag = 1;
   else l4.zero_flag = 0;
}

static ACCUMULATOR adhere_12bit(ACCUMULATOR value) {
   if (value > 2047)
      return value - 4096;
   if (value < -2048)
      return value + 4096;
   return value;
}

void l4_alu() {
   ACCUMULATOR rhs = memslot_to_accumulator(l3.read);
   ACCUMULATOR lhs;

   /* forwarding on the ALU result */
   switch (l4.instruction) {
      case OP_AND:
      case OP_OR:
      case OP_ADD:
      case OP_SUB:
         lhs = l4.alu_res;
         break;
      default:
         lhs = memory.accumulator;
         break;
   }

   switch (l3.instruction) {
      case OP_AND:
         l4.alu_res = lhs & rhs;
         break;
      case OP_OR:
         l4.alu_res = lhs | rhs;
         break;
      case OP_ADD:
         {
            ACCUMULATOR interm = lhs + rhs;
            l4.alu_res = adhere_12bit(interm);
         }
         break;
      case OP_SUB:
         {
            ACCUMULATOR interm = lhs - rhs;
            l4.alu_res = adhere_12bit(interm);
         }
         break;
      default:
         break;
   }
}

void l4_mem() {
   switch (l3.instruction) {
      case OP_STORE:
         memory.instructions[l3.address].opcode 
            = (memory.accumulator >> 8);
         memory.instructions[l3.address].address 
            = (memory.accumulator & 0xff);
         break;
      case OP_LOADI:
         {
            ACCUMULATOR target = memslot_to_accumulator(l3.read);
            l4.read2 = memory.instructions[target&0xff];
         }
         break;
      case OP_STOREI:
         {
            ACCUMULATOR target = memslot_to_accumulator(l3.read);
            memory.instructions[target&0xff].opcode 
               = (memory.accumulator >> 8);
            memory.instructions[target&0xff].address
               = (memory.accumulator & 0xff);
         }
         break;
      default:
         break;
   }
}

void l5_writeback() {
   memory.prev_instruction_pointer 
      = memory.instruction_pointer;
   switch (l4.instruction) {
      case OP_JMP:
         memory.instruction_pointer = l4.address + 1;
         memory.prev_instruction_pointer = l4.address;
         break;
      case OP_JN:
         if (l4.negative_flag) {
            memory.instruction_pointer = l4.address + 1;
            memory.prev_instruction_pointer = l4.address;
         }
         else
            memory.instruction_pointer += 1;
         break;
      case OP_JZ:
         if (l4.zero_flag) {
            memory.instruction_pointer = l4.address + 1;
            memory.prev_instruction_pointer = l4.address;
         }
         else
            memory.instruction_pointer += 1;
         break;
      default:
         if (!control.instruction_pointer_usage && !control.stall)
            memory.instruction_pointer += 1;
         break;
   }

   switch (l4.instruction) {
      case OP_LOAD:
         memory.accumulator = memslot_to_accumulator(l4.read);
         break;
      case OP_LOADI:
         memory.accumulator = memslot_to_accumulator(l4.read2);
         break;
      case OP_AND:
      case OP_OR:
      case OP_ADD:
      case OP_SUB:
         memory.accumulator = l4.alu_res;
         break;
      default:
         break;
   }
}

void l5_pc_inc() {
   /* kinda sad... */
   memory.prev_instruction_pointer 
      = memory.instruction_pointer;
   memory.instruction_pointer += 1;
}
