#include "Process.h"

/*Creates a new Process object*/
Process * P_create(char pid, int size) /*error check params as they come in from file*/
{
    Process * new_p = malloc(sizeof(Process));

    if (!new_p)
    {
        perror("Could not allocate memory for Process. Exiting.\n");
        exit(0);
    }

    /*set fields*/
    new_p -> pid = pid;
    new_p -> size = size;
    new_p -> base = 0;
    new_p -> limit = 0;
    new_p -> swaps = 0;
    new_p -> next = NULL;

    return new_p;
}

/*Used to get the process id of a given Process*/
int P_getPID(Process * p)
{
    if(!p)
    {
        return -1;
    }

    return p -> pid;
}

/*Used to get the size of a given Process*/
int P_getSize(Process * p)
{
    if(!p)
    {
        return -1;
    }

    return p -> size;
}

/*Used to print contents of a Process*/
void P_print(Process * p)
{
    if (p)
    {
        printf("PID: %c \t Size: %d\n", p -> pid, p -> size);
    }
}

/*Destroy and deallocate a Process*/
void P_destroy(Process * p)
{
    if(p)
    {
        free(p);
        p = NULL;
    }
}

/*int main ()
{
    Process * p = P_create(99, 20);
    P_print(p);
    P_destroy(p);

    return 0;
}*/
