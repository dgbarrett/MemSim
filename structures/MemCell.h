#ifndef __MEMCELL_H__
#define __MEMCELL_H__

#include <stdio.h>
#include <stdlib.h>
#include "Process.h"

/*the State type is used to represent the possible
  states of a MemCell*/
typedef enum State{
    FULL,
    FREE
}State;

/*A MemCell is used to represent 1MB of memory, an array of
  MemCells is used to represent the entirety of RAM*/
typedef struct MemCell {
    int write_time;       /*indicates when the cell was last written into*/
    State cell_state;     /*tells whether the cell is occupied or not*/
    Process * contents;   /*process that occupies the memory cell, NULL if cell is free*/
}MemCell;

int             MC_isFree           (MemCell * mc);
int             MC_isFull           (MemCell * mc);
MemCell *       MC_create           ();
MemCell **      MC_createTable      (int size);
void            MC_setContents      (MemCell * mc, Process * p, int write_time);
void            MC_setState         (MemCell * mc, State new_state);
void            MC_print            (MemCell * mc);
void            MC_destroy          (MemCell * mc);
void            MC_destroyTable     (MemCell ** mc, int size);

#endif /* __MEMCELL_H__ */
