
#ifndef __TRACE__

#define __TRACE__
#include <stdio.h>
#include "memory.h"

/* trace code */

typedef struct {
   FILE * fhandle;
   int live;
} Tracer;

void start_trace(char * fname);
void end_trace();

void add_trace(OPCODE opcode, BYTE address);

extern Tracer tracer;

#endif
