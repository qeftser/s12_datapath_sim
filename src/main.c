#include <stdio.h>
#include "parser.h"

// test main function for testing parser
int main(int argc, char **args) {
	if (argc < 2) {
		return 1;
	}

	Memory *m = parse_input_file(args[1]);
	printf("The instruction pointer is: %i\n", m->instruction_pointer);
	printf("The accumulartor is: %i\n", m->accumulator);

	int num_instructions = 5;
	int current = 0;
	printf("The first %i instructions are:\n", num_instructions);
	while (current++ < num_instructions) {
		printf("%i | %i\n", m->instructions[current].opcode, m->instructions[current].address);
	}
}
