#include <stdio.h>
#include <string.h>
#include "memory.h"
#include "latch.h"
#include "parser.h"

/* give our externed variables a place to live */
Memory memory;
L1 l1;
L2 l2;
L3 l3;
L4 l4;

// test main function for testing parser
int main(int argc, char **argv) {
	if (argc < 2) {
      printf("usage:\n%s <memfile>\n",argv[0]);
		return 1;
	}

   /* initialize the pipeline */
   memset(&memory,0,sizeof(Memory));
   memset(&l1,0,sizeof(L1));
   memset(&l2,0,sizeof(L2));
   memset(&l3,0,sizeof(L3));
   memset(&l4,0,sizeof(L4));

   /* get the user data */
	parse_mem_file(argv[1]);
	printf("The instruction pointer is: %i\n", memory.instruction_pointer);
	printf("The accumulator is: %i\n", memory.accumulator);

   /* run the simulation */
	int num_instructions = 5;
	int current = 0;
	printf("The first %i instructions are:\n", num_instructions);
	while (current++ < num_instructions) {
		printf("%i | %i\n", memory.instructions[current].opcode, memory.instructions[current].address);
	}
}
