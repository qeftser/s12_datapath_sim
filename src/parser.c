#include "parser.h"
#include <stdio.h>
#include <stdlib.h>


void skip_whitespace(FILE *file) {
  int c;
  int exit = 0;
  while (exit == 0 && (c = fgetc(file)) != EOF)  {
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

// assumed endian
unsigned int get_n_bits(FILE *file, int number) {
	skip_whitespace(file);

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

unsigned char get_4_bits(FILE *file) { return get_n_bits(file, 4); }
unsigned char get_8_bits(FILE *file) { return get_n_bits(file, 8); }
unsigned int get_12_bits(FILE *file) { return get_n_bits(file, 12); }

Memory *parse_input_file(char *filename) {
  FILE *file = fopen(filename, "r");
  Memory *m = malloc(sizeof(Memory));

  if (!file) {
    return m;
  }

  printf("The first four are: %i followed by: %i\n", get_4_bits(file),
         get_4_bits(file));

  fclose(file);

  return m;
}
