#ifndef __MEMORY_H__
#define __MEMORY_H__

#include <stdio.h>
#include <stdlib.h>
#include "MemCell.h"
#include "SpaceList.h"
#include "Process.h"
#include "ProcessQueue.h"
#include "../holes.h"

#define MEM_SIZE 128

/*Used to represent the Memory as well as all info assocaited with it*/
typedef struct Memory {
    int writes;                 /* keeps track of the number of writes done to memory*/
    int processes;              /* keeps track of the total number of processes in memory over time*/
    int holes;                  /* keeps track of the total number of holes in memory over time*/
    int prev_limit;               /* used for next-fit scheduling*/
    float cumulative_mem_usage; /* keeps track of the sum of all mem_usages, used to calcuate cumulative usage at each write*/
    Process * current_process;  /* the process the memory is currently working on writing*/
    SpaceList * free_blocks;    /* list of all free blocks in memory*/
    MemCell ** cell;            /* THE ACTUAL MEMORY, an array of MemCells, each represents each 1 MB block for each 128 in Memory */
}Memory;

typedef enum AllocationType {
    FIRST_FIT,
    BEST_FIT,
    NEXT_FIT,
    WORST_FIT
}AllocationType;

Memory *    M_create                                    ();
int         M_firstFit                                  (Memory * mem, ProcessQueue * mem_queue);
int         M_worstFit                                  (Memory * mem, ProcessQueue * mem_queue);
int         M_bestFit                                   (Memory * mem, ProcessQueue * mem_queue);
int         M_nextFit                                   (Memory * mem, ProcessQueue * mem_queue);
int         M_getInsertPositionAccordingToStrategy      (Memory * mem, ProcessQueue * mem_queue, AllocationType strat);
int         M_updateFreeBlocksList                      (Memory * mem);
int         M_countHoles                                (Memory * mem);
int         M_countProcesses                            (Memory * mem);
int         M_updateFreeBlocksList                      (Memory * mem);
float       M_memUsage                                  (Memory * mem);
void        M_setNextProcess                            (Memory * mem, ProcessQueue * mem_queue);
void        M_calculateStatisticsAndPrint               (Memory * mem, char pid);
void        M_resetRange                                (Memory * mem, int base, int limit);
void        M_insertAt                                  (Memory * mem, int base);
void        M_swap                                      (Memory * mem, ProcessQueue * mem_queue);
void        M_destroy                                   (Memory * mem);
void        M_print                                     (Memory * mem);
void        M_printFinalStatistics                      (Memory * mem);
void        M_executeMemSim                             (Memory * mem, ProcessQueue * mem_queue, AllocationType strat);

#endif /* __MEMORY_H__ */
