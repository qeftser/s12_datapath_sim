
#include <string.h>
#include "control.h"
#include "memory.h"
#include "latch.h"
#include "logic.h"
#include "isa.h"

void dump_pipeline(int stage) {
   if (stage > 4 || stage < 0)
      return; /* xD */

   if (stage > 3) {
      memset(&l4,0,sizeof(L4));
      memory.instruction_pointer -= 1;
      control.active[3] = 0;
   }
   if (stage > 2) {
      memset(&l3,0,sizeof(L3));
      memory.instruction_pointer -= 1;
      control.active[2] = 0;
   }
   if (stage > 1) {
      memset(&l2,0,sizeof(L2));
      memory.instruction_pointer -= 1;
      control.active[1] = 0;
   }
   if (stage > 0) {
      memset(&l1,0,sizeof(L1));
      memory.instruction_pointer -= 1;
      control.active[0] = 0;
   }

   /* nothing happens if you pass 0 */
}

static int l5_advance() {
   if (!control.active[4]) {

      /* still advance the program counter */
      l5_pc_inc();

      return 1;
   }

   // TODO(qeftser): this guy 

   l5_writeback();

   control.active[4] = 0;
   return 0;
}

static int l4_advance() {
   if (!control.active[2])
      return 1;

   // TODO(qeftser): this guy

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
         if (control.usage[memory.instruction_pointer] == USE_WRITE)
            return 0;
         if (control.usage[memory.instruction_pointer] < USE_READ_1)
            control.usage[memory.instruction_pointer] = USE_READ_1;
         l3_read();
         break;
      case OP_STORE:
         if (control.usage[memory.instruction_pointer] != USE_NO)
            return 0;
         control.usage[memory.instruction_pointer] = USE_WRITE;
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
   if (control.usage[memory.instruction_pointer] == USE_WRITE ||
       control.instruction_pointer_usage) {
      control.active[0] = 0;
      return 0;
   }

   l1_read();
   control.active[0] = 1;

   return 1;
}

void advance() {
   int res;

   res = l5_advance();

   if (res)
      res = l4_advance();

   if (res)
      res = l3_advance();

   if (res)
      res = l2_advance();

   if (res)
      res = l1_advance();
}
