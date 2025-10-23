#include "parser.h"
#include "memory.h"
#include <stdio.h>
#include <stdlib.h>

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
    switch (c) {
    case '1':
      result |= 1;
    }
  }
  return result;
}

unsigned char next_4_bits(FILE *file) { return get_n_bits(file, 4); }
unsigned char next_8_bits(FILE *file) { return get_n_bits(file, 8); }
unsigned int next_12_bits(FILE *file) { return get_n_bits(file, 12); }

Memory *parse_input_file(char *filename) {
  FILE *file = fopen(filename, "r");
  // just assume that this works
  Memory *m = malloc(sizeof(Memory));

  // does the input file exsist
  if (!file) {
    return m;
  }

  // update mem with header
  m->instruction_pointer = next_8_bits(file);
  m->accumulator = next_12_bits(file);

  int line = 0;
  while (line++ < 255) {
    // read line
    skip_addr(file);
    OPCODE o = next_4_bits(file);
    BYTE addr = next_8_bits(file);

    // update mem
    m->instructions[line].opcode = o;
    m->instructions[line].address = addr;
  }

  fclose(file);

  return m;
}
