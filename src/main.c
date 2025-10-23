#include <stdio.h>
#include "parser.h"

int main(int argc, char **args) {
	if (argc < 2) {
		return 1;
	}

	parse_input_file(args[1]);
}
