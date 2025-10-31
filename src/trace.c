
#include "trace.h"
#include "visual.h"

void start_trace(char * fname) {
   if (tracer.live)
      return;

   tracer.live = 1;
   tracer.fhandle = fopen(fname,"w");
}

void end_trace() {
   if (!tracer.live)
      return;

   fclose(tracer.fhandle);
   tracer.live = 0;

}

void add_trace(OPCODE opcode, BYTE address) {
   if (!tracer.live)
      return;

   fprintf(tracer.fhandle,"%s %x\n",isa_names[opcode],address);
}

