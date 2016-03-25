#include "ProcessQueue.h"

/*Creates a new ProcessQueue object*/
ProcessQueue * PQ_create()
{
    ProcessQueue * pq = malloc(sizeof(ProcessQueue));

    if(!pq)
    {
        perror("Could not allocate memory for Process Queue. Exiting\n");
        exit(0);
    }

    pq -> head = pq -> tail = NULL;
    pq -> size = 0;

    return pq;
}

/*Returns the size of the ProcessQueue or -1 if it is null*/
int PQ_getSize(ProcessQueue * pq)
{
    if(pq)
    {
        return pq -> size;
    }

    return -1;
}


/*Used to determine if the ProcessQueue contains any
  elements or not.  Returns 1 if empty, 0 if not*/
int PQ_isEmpty(ProcessQueue * pq)
{
    if(pq)
    {
        if(pq->head == NULL)
        {
            return 1;
        }
    }

    return 0;
}


/*Used to remove the frontmost item from the ProcessQueue*/
Process * PQ_remove(ProcessQueue * pq)
{
    if (pq)
    {
        /*store current head in temp Process*/
        Process * temp = pq -> head;

        /*increment the head of the ProcessQueue*/
        pq -> head = pq -> head -> next;

        /*decrease the size of the ProcessQueue*/
        pq -> size--;

        temp -> next = NULL; /*so rest of queue cannot be accessed by popping head*/

        /*return the temp*/
        return temp;
    }

    return NULL;
}

/*Used to add an item to the ProcessQueue*/
void PQ_add(ProcessQueue * pq, Process * p)
{
    if (pq && p)
    {
        if (pq -> head == NULL)
        {
            /*if ProcessQueue is empty, the first node becomes head and tail*/
            pq -> head = pq -> tail = p;
        }
        else
        {
            /*add to tail*/
            pq -> tail -> next = p;
            pq -> tail = pq -> tail -> next;
        }

        pq -> size++;
    }
}

/*Used to print out the contents of all nodes in the ProcessQueue*/
void PQ_print(ProcessQueue * pq)
{
    printf("printing\n");
    if(pq)
    {
        Process * temp = pq -> head;

        while (temp)
        {
            P_print(temp);
            temp = temp -> next;
        }
    }
}

/*Destroy and deallocate a ProcessQueue object*/
void PQ_destroy(ProcessQueue * pq)
{
    if (pq)
    {
        Process * temp = pq -> head;

        while (temp != NULL)
        {
            pq -> head = pq -> head -> next;
            P_destroy(temp);
            temp = pq -> head;
        }

        free(pq);
        pq = NULL;
    }
}

/*int main()
{
    ProcessQueue * pq = PQ_create();
    Process * p = P_create(97, 10);
    Process * p1 = P_create(98, 20);
    Process * p2 = P_create(99, 30);
    Process * p3 = P_create(100, 40);
    Process * p4 = P_create(101, 50);

    PQ_add(pq, p);
    PQ_add(pq, p1);
    PQ_add(pq, p2);
    PQ_add(pq, p3);

    PQ_print(pq);

    PQ_remove(pq);
    PQ_add(pq, p4);

    PQ_print(pq);


    PQ_destroy(pq);

    return 0;
}*/
