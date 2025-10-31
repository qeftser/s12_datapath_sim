
#ifndef __STAT__

#define __STAT__

/* statistics collected during runtime */

typedef struct {
   int forwards;
   int stalls;
   int dumps[4];
   int executed;
   int cycles;
   int instruction[16];
   int live[4];
} Statistics;

extern Statistics stats;

void print_statistics();

#endif
