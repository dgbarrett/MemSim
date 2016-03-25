#ifndef __PROCESSQUEUE_H__
#define __PROCESSQUEUE_H__

#include <stdio.h>
#include <stdlib.h>
#include "Process.h"

/*Used to hold processes that are waiting to be put into Memory*/
typedef struct ProcessQueue {
    Process * head;            /*pointer to head of queue*/
    Process * tail;            /*pointer to the tail of the queue*/
    int size;                  /*number of items in the queue*/
}ProcessQueue;

int             PQ_getSize      (ProcessQueue * pq);
int             PQ_isEmpty      (ProcessQueue * pq);
Process *       PQ_remove       (ProcessQueue * pq);
ProcessQueue *  PQ_create       ();
void            PQ_add          (ProcessQueue * pq, Process * p);
void            PQ_destroy      (ProcessQueue * pq);
void            PQ_print        (ProcessQueue * pq);

#endif /* __PROCESSQUEUE_H__ */
