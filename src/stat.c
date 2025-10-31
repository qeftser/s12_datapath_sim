
#include "stat.h"
#include "isa.h"
#include <stdio.h>

#define PRINT_MIX(op) \
   printf("\t%s:\t%d\t(%.2f%%)\n",#op,stats.instruction[OP_##op],\
         100*(double)stats.instruction[OP_##op]/stats.executed)


void print_statistics() {
   printf("\n=========== RUN STATISTICS ===========\n");
   printf("Total instructions executed: %d\n",stats.executed);
   printf("Instruction mix:\n");
   PRINT_MIX(JMP);
   PRINT_MIX(JN);
   PRINT_MIX(JZ);
   PRINT_MIX(LOAD);
   PRINT_MIX(STORE);
   PRINT_MIX(LOADI);
   PRINT_MIX(STOREI);
   PRINT_MIX(AND);
   PRINT_MIX(OR);
   PRINT_MIX(ADD);
   PRINT_MIX(SUB);
   PRINT_MIX(HALT);
   printf("\n");
   printf("Cycles: %d\n",stats.cycles);
   printf("Cycles per instruction: %.2f\n",
          (double)stats.cycles / stats.executed);
   printf("Forwards: %d\n",stats.forwards);
   printf("\tCycles without forwarding: %d\n",
          stats.cycles + stats.forwards);
   printf("\tCPI without forwarding: %.2f\n",
        (double)(stats.cycles+stats.forwards)/stats.executed);
   printf("Stalls: %d\n",stats.stalls);
   printf("Dumps: %d\n",stats.dumps[0]+stats.dumps[1]+
          stats.dumps[2]+stats.dumps[3]);
   printf("\tStage 1 Dumps: %d\n",stats.dumps[0]);
   printf("\tStage 2 Dumps: %d\n",stats.dumps[1]);
   printf("\tStage 3 Dumps: %d\n",stats.dumps[2]);
   printf("\tStage 4 Dumps: %d\n",stats.dumps[3]);
   printf("Average pipeline fill: %.2f%%\n",
         (double)100*(stats.live[0]+stats.live[1]+stats.live[2]+
                     stats.live[3])/(stats.cycles*4));
   printf("\tAverage Stage 1 Fill: %.2f%%\n",
         (double)100*(stats.live[0])/stats.cycles);
   printf("\tAverage Stage 2 Fill: %.2f%%\n",
         (double)100*(stats.live[1])/stats.cycles);
   printf("\tAverage Stage 3 Fill: %.2f%%\n",
         (double)100*(stats.live[2])/stats.cycles);
   printf("\tAverage Stage 4 Fill: %.2f%%\n",
         (double)100*(stats.live[3])/stats.cycles);
   printf("Cycles without pipelining: %d\n",stats.executed*5);
   printf("CPI without pipelining: 5\n"); // xD
   printf("Cycles saved from pipelining: %d\n",
         stats.executed*5 - stats.cycles); // xD
   printf("CPI increase from pipelining: %.2f\n",
    (double)(stats.executed*5 - stats.cycles)/stats.executed);
   printf("======================================\n");
}

#undef PRINT_MIX
