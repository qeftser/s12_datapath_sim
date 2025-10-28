
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
   USE_READ_4 = 4,
   USE_WRITE  = 255,
} usage_mark;

typedef struct {
   unsigned char usage[256];
   unsigned char accumulator_usage;
   unsigned char instruction_pointer_usage;
   unsigned char active[5];
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
void advance();

/* dump a visual representation of the pipeline
 * to the screen - this will clear the screen
 * as well. Useful for debugging and also looks
 * kind of cool :).                             */
void print_state();

extern Control control;

#endif
