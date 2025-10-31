#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "latch.h"
#include "parser.h"
#include "control.h"
#include "visual.h"
#include "stat.h"
#include "trace.h"

/* give our externed variables a place to live */
Memory memory;
Control control;
Statistics stats;
Tracer tracer;
L1 l1;
L2 l2;
L3 l3;
L4 l4;

// test main function for testing parser
int main(int argc, char **argv) {
	if (argc < 2 || argc > 3) {
      printf("usage:\n%s [-d] <memfile>\n",argv[0]);
      printf("\n=> passing -d runs in debug mode\n");
		return 1;
	}
   int debug = 0;
   int fpos = 1;
   if (argc == 3) {
      if (!strcmp("-d",argv[1])) {
         debug = 1;
         fpos = 2;
      }
      else if (!strcmp("-d",argv[2]))
         debug = 1;
   }

   /* initialize the pipeline */
   memset(&memory,0,sizeof(Memory));
   memset(&control,0,sizeof(Control));
   memset(&tracer,0,sizeof(Tracer));
   memset(&stats,0,sizeof(Statistics));
   memset(&l1,0,sizeof(L1));
   memset(&l2,0,sizeof(L2));
   memset(&l3,0,sizeof(L3));
   memset(&l4,0,sizeof(L4));

   /* get the user data */
   printf("loading file %s...\n",argv[fpos]);
   parse_mem_file(argv[fpos]);

   char buf[1024];
   strcpy(buf,argv[fpos]);
   strcat(buf,".trace");
   printf("saving trace to %s...\n",buf);
   start_trace(buf);

   if (debug) {
      printf("The instruction pointer is: %i\n", memory.instruction_pointer);
      printf("The accumulator is: %i\n", memory.accumulator);

      /* run the simulation */
      int num_instructions = 5;
      int current = 0;
      printf("The first %i instructions are:\n", num_instructions);
      while (current++ < num_instructions) {
         printf("%i | %i\n",
                memory.instructions[current].opcode,
                memory.instructions[current].address);
      }

      fprintf(stderr,"(enter to advance)..");
      getchar();

      do {
         print_state();

         /* pause until user types enter.. */
         fprintf(stderr,"(enter to advance)..");
         getchar();
      } while (!advance());
   }
   else {
      while (!advance());
   }
   
   end_trace();
   print_statistics();

   return 0;
}
