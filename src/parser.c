#include "parser.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

int had_error = 0;

void skip_whitespace(FILE *file) {
  int c;
  int exit = 0;
  while (exit == 0 && (c = fgetc(file)) != EOF) {
    switch (c) {
    case ' ':
    case '\n':
    case '\r':
      break;
    default:
      exit = 1;
    }
  }

  if (c == EOF)
    had_error = 1;

  fseek(file, -1, SEEK_CUR);
}

// this is used to skip the hex instruction indicator
// they are alreyas sequential, and it is redundant to
// parse them from the source file
void skip_addr(FILE *file) {
  skip_whitespace(file);
  fseek(file, 2, SEEK_CUR);
}

// assumed endian
unsigned int get_n_bits(FILE *file, int number) {
  skip_whitespace(file); // could move this to the loop if needed

  unsigned int result = 0;
  int num_read = 0;
  int c;
  while (num_read++ < number && (c = fgetc(file)) != EOF) {
    result <<= 1;
    if (c == '1')
      result |= 1;
  }

  if (c == EOF)
    had_error = 2;

  return result;
}

unsigned char next_4_bits(FILE *file) { return get_n_bits(file, 4); }
unsigned char next_8_bits(FILE *file) { return get_n_bits(file, 8); }
unsigned int next_12_bits(FILE *file) { return get_n_bits(file, 12); }

void parse_mem_file(char *filename) {
  FILE *file = fopen(filename, "r");
  // just assume that this works
  // does the input file exsist
  if (!file) {
		return;
  }

  // update mem with header
  memory.instruction_pointer = next_8_bits(file);
  memory.accumulator = next_12_bits(file);

  int line = 0;
  while (had_error == 0 && line <= 255) {
    // read line
    skip_addr(file);
    OPCODE o = next_4_bits(file);
    BYTE addr = next_8_bits(file);

    // update mem
    memory.instructions[line].opcode = o;
    memory.instructions[line].address = addr;

    line += 1;
  }

  fclose(file);

  if (had_error != 0)
    printf("\n\nFAILED TO PARSE INPUT FILE: unexpected EOF\n\n");
}
