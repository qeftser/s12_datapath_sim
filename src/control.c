
#include <string.h>
#include "control.h"
#include "memory.h"
#include "latch.h"
#include "logic.h"
#include "isa.h"
#include "stat.h"
#include "trace.h"

void dump_pipeline(int stage) {
   if (stage > 4 || stage < 1)
      return; /* xD */

   stats.dumps[stage] += 1;

   if (stage > 3) {
      control.usage[l4.instruction_pos] &= ~USE_READ_1;
      switch (l4.instruction) {
         case OP_LOAD:
         case OP_LOADI:
         case OP_STOREI:
         case OP_ADD: case OP_SUB:
         case OP_AND: case OP_OR:
            control.usage[l4.address] &= ~USE_READ_2;
            break;
      }
      memset(&l4,0,sizeof(L4));
      memory.instruction_pointer -= 1;
      control.active[3] = 0;
   }
   if (stage > 2) {
      control.usage[l3.instruction_pos] &= ~USE_READ_1;
      switch (l3.instruction) {
         case OP_LOAD:
         case OP_LOADI:
         case OP_STOREI:
         case OP_ADD: case OP_SUB:
         case OP_AND: case OP_OR:
            control.usage[l3.address] &= ~USE_READ_2;
            break;
         case OP_STORE:
            control.usage[l3.address] = USE_NO;
            break;
      }
      memset(&l3,0,sizeof(L3));
      memory.instruction_pointer -= 1;
      control.active[2] = 0;
   }
   if (stage > 1) {
      control.usage[l2.instruction_pos] &= ~USE_READ_1;
      memset(&l2,0,sizeof(L2));
      memory.instruction_pointer -= 1;
      control.active[1] = 0;
   }
   if (stage > 0) {
      control.usage[l1.instruction_pos] &= ~USE_READ_1;
      memset(&l1,0,sizeof(L1));
      memory.instruction_pointer -= 2;
      control.active[0] = 0;
   }
   memory.prev_instruction_pointer =
      memory.instruction_pointer - 1;

   /* nothing happens if you pass 0 */
}

static int l5_advance() {
   if (!control.active[3]) {

      /* still advance the program counter */
      if (!control.instruction_pointer_usage)
         l5_pc_inc();

      return 1;
   }

   if (control.instruction_pointer_usage) {
      switch (l4.instruction) {
         case OP_JMP:
         case OP_JN:
         case OP_JZ:
            control.instruction_pointer_usage = 0;
            break;
      }
   }

   control.usage[l4.instruction_pos] &= ~USE_READ_1;

   switch (l4.instruction) {
      case OP_LOAD:
      case OP_LOADI:
      case OP_STOREI:
      case OP_ADD: case OP_SUB:
      case OP_OR:  case OP_AND:
         control.usage[l4.address] &= ~USE_READ_2;
         break;
      case OP_STORE:
         control.usage[l4.address] = USE_NO;
         break;
   }

   l5_writeback();

   control.active[3] = 0;

   if (l4.instruction == OP_HALT)
      control.halt = 1;

   return 1;
}

static int l4_advance() {
   if (!control.active[2])
      return 1;

   int to_dump = 0;
   switch (l3.instruction) {
      case OP_JN:
      case OP_JZ:
         l4_flags();
         break;
      case OP_ADD:
      case OP_SUB:
      case OP_OR:
      case OP_AND:
         l4_alu();
         break;
      case OP_STORE:
         l4_mem();
         control.usage[l3.address] = USE_NO;
         break;
      case OP_LOADI:
         if (control.usage[l3.read.address] == USE_WRITE)
            return 0;
         l4_mem();
         break;
      case OP_STOREI:
         switch (control.usage[l3.read.address]) {
            case USE_NO:
               l4_mem();
               break;
            case USE_READ_1:
               to_dump = 2;
               l4_mem();
               break;
            case USE_READ_2:
            case USE_READ_3:
            case USE_WRITE:
               to_dump = 3;
               l4_mem();
               break;
         }
         break;
   }



   /* advance the pipeline */
   control.active[2] = 0;
   control.active[3] = 1;
   l4_forward();
   dump_pipeline(to_dump);

   return 1;
}

static int l3_advance() {
   if (!control.active[1])
      return 1;

   switch (l2.instruction) {
      case OP_JMP:
      case OP_JN:
      case OP_JZ:
         dump_pipeline(1);
         control.instruction_pointer_usage = 1;
         break;
      case OP_LOAD:
      case OP_LOADI:
      case OP_STOREI:
      case OP_ADD: case OP_SUB:
      case OP_OR:  case OP_AND:
         if (control.usage[l2.address] == USE_WRITE)
            return 0;
         control.usage[l2.address] |= USE_READ_2;
         l3_read();
         break;
      case OP_STORE:
         if (l1.instruction_pos == l2.address) {
            dump_pipeline(1);
            memory.prev_instruction_pointer += 1;
         }
         if (control.usage[l2.address] != USE_NO &&
             l2.address != l2.instruction_pos)
            return 0;
         control.usage[l2.address] = USE_WRITE;
         break;
      default:
         break;
   }


   /* advance the pipeline */
   control.active[1] = 0;
   control.active[2] = 1;
   l3_forward();

   return 1;
}

static int l2_advance() {
   if (!control.active[0])
      return 1;

   /* advance the pipeline forward */
   control.active[0] = 0;
   control.active[1] = 1;

   l2_decode();

   return 1;
}

static int l1_advance() {

   /* check for data hazard for the instruction fetch
    * stage of the pipeline.                          */
   if (control.usage[memory.prev_instruction_pointer] == USE_WRITE ||
       control.instruction_pointer_usage) {
      control.active[0] = 0;
      return 0;
   }

   l1_read();
   control.usage[memory.prev_instruction_pointer] |= USE_READ_1;
   control.active[0] = 1;

   return 1;
}

int advance() {
   int res;

   stats.cycles += 1;

   if (control.active[3]) {
      stats.executed += 1;
      stats.instruction[l4.instruction] += 1;
      add_trace(l4.instruction,l4.address);
   }

   res = l5_advance();

   if (res)
      res = l4_advance();

   if (res)
      res = l3_advance();
   else {
      memory.instruction_pointer -= 1;
      memory.prev_instruction_pointer -= 1;
   }

   if (res)
      res = l2_advance();
   else {
      memory.instruction_pointer -= 1;
      memory.prev_instruction_pointer -= 1;
   }

   if (res)
      res = l1_advance();

   if (!res)
      stats.stalls += 1;

   if (control.active[0])
      stats.live[0] += 1;
   if (control.active[1])
      stats.live[1] += 1;
   if (control.active[2])
      stats.live[2] += 1;
   if (control.active[3])
      stats.live[3] += 1;

   return control.halt;
}
