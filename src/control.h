
#ifndef __CONTROL__

#define __CONTROL__

/* the control unit for the pipeline. It will
 * have a running list of the instructions being
 * ran and a couple of flag buffers that mark
 * where any hazards are or any pipline dumps
 * need to happen.                              */

typedef enum {
   USE_NO     = 0,
   USE_READ_1 = 1,
   USE_READ_2 = 2,
   USE_READ_3 = 3,
   USE_WRITE  = 255,
} usage_mark;

typedef struct {
   unsigned char usage[256];
   unsigned char accumulator_usage;
   unsigned char instruction_pointer_usage;
   unsigned char stall;
   unsigned char active[5];
   unsigned char halt;
} Control;

/* clear out all values on the
 * given pipeline backwards up
 * to the stage provided. Rewind
 * the program counter back by
 * this number as well         */
void dump_pipeline(int stage);

/* move the pipeline forward one 
 * step. Do all actions associated
 * with this task                 */
int advance();

extern Control control;

#endif
