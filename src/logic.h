
#ifndef __LOGIC__

#define __LOGIC__

/* all of the logical operations that can occur
 * in the datapath. These will have the form l*_<op>
 * where * is the stage that the action occurs in, or
 * the latch that it writes to, and <op> is the action
 * that is performed on the pipeline.                 */

/* place the value under the instruction
 * pointer into L1                      */
void l1_read();

/* place the decoded value of the
 * memory in L1 into L2          */
void l2_decode();

/* place any values that are simply passed
 * through the third stage into L3        */
void l3_forward();

/* read from the address in L2 and place
 * the result in L3                     */
void l3_read();

/* place any values that are passed 
 * through the fourth stage into L4 */
void l4_forward();

/* compute the flags (zero and negative)
 * using the current value of the accumulator */
void l4_flags();

/* perform the alu operation using the instruction
 * in L3, address read and current accumulator and
 * place the result in L4                         */
void l4_alu();

/* perform a read or write operation with the memory
 * unit of stage four. The opcode stored in L3
 * determines what is done. This is either an
 * indirect read, write, or indirect write. In 
 * the case of a read, store the result in L4   */
void l4_mem();

/* update the accumulator using one of the value
 * stored in L4. The value used is determined by
 * the opcode in L4.                             */
void l5_writeback();

/* increment the program counter. This will 
 * normally be done by the above function. 
 * However, sometimes there will not be 
 * instructions in the pipeline. This gets
 * called in that case to move things forward
 * without dealing with writeback of garbage
 * data.                                     */
void l5_pc_inc();

#endif
