#include <stdio.h>
#include "memory.h"
#include "parser.h"

// test main function for testing parser
int main(int argc, char **args) {
	if (argc < 2) {
		return 1;
	}

	parse_mem_file(args[1]);
	printf("The instruction pointer is: %i\n", memory.instruction_pointer);
	printf("The accumulartor is: %i\n", memory.accumulator);

	int num_instructions = 5;
	int current = 0;
	printf("The first %i instructions are:\n", num_instructions);
	while (current++ < num_instructions) {
		printf("%i | %i\n", memory.instructions[current].opcode, memory.instructions[current].address);
	}
}
