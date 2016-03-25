#ifndef __PROCESS_H__
#define __PROCESS_H__

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

typedef struct Process {
    char pid;               /* holds the ascii value of the process letter as PID*/
    int size;               /*size in MB of the process*/
    int base;               /*starting address in memory of process*/
    int limit;              /*last address in memory of process*/
    int swaps;              /*number of times the process has been swapped out*/
    struct Process * next;  /*used to chain processes together in queue*/
}Process;

int         P_getPID        (Process * p);
int         P_getSize       (Process * p);
Process *   P_create        (char pid, int size);
void        P_print         (Process * p);
void        P_destroy       (Process * p);

#endif /* __PROCESS_H__ */
