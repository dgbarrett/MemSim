#include "MemCell.h"

/*Creates a new MemCell object*/
MemCell * MC_create()
{
    MemCell * new_mc = malloc(sizeof(MemCell));

    if (!new_mc)
    {
        perror("Could not allocate memory for MemCell. Exiting.\n");
        exit(0);
    }

    MC_setState(new_mc, FREE);
    new_mc -> write_time = -1;
    new_mc -> contents = NULL;

    return new_mc;
}

/*Creates a table of MemCells, size is determined by the value passed
  to the function*/
MemCell ** MC_createTable(int size)
{
    /*checking for invalid size (0 or less)*/
    if(size < 1)
    {
        perror("Invalid Memory size.  Exiting\n");
        exit(0);
    }

    MemCell ** table = malloc(size * sizeof(MemCell));
    int loop_var = 0;

    if (!table)
    {
        perror("Could not allocate memory for MemCell **. Exiting\n");
        exit(0);
    }

    for (loop_var = 0; loop_var < size; loop_var++)
    {
        /*allocating space for each MemCell*/
        table[loop_var] = MC_create();

    }

    return table;
}

/*Used to determine if a MemCell is full*/
int MC_isFree(MemCell * mc)
{
    if (mc)
    {
        if (mc -> cell_state == FREE)
        {
            return 1;
        }
    }

    return 0;
}

/*Used to determine if a MemCell is full*/
int MC_isFull(MemCell * mc)
{
    if (mc)
    {
        if (mc -> cell_state == FULL)
        {
            return 1;
        }
    }

    return 0;
}

/*Used to set the contents of a particular MemCell*/
void MC_setContents(MemCell * mc, Process * p, int write_time)
{
    if (mc && p && (write_time > 0))
    {
        mc -> contents = p;
        mc -> cell_state = FULL;
        mc -> write_time = write_time;
    }
    else
    {
        printf("MC_setContents condition failed. Doing Nothing.\n");
    }
}

/*Used to set the state of a MemCell*/
void MC_setState(MemCell * mc, State new_state)
{
    if(mc)
    {
        switch(new_state)
        {
            case FULL: /*checking to make sure valid State passed to function*/
            case FREE:
                mc -> cell_state = new_state;
                break;
            default:
                perror("Unrecongnized State Change. Doing Nothing.\n");
                break;
        }
    }
}

/*Used to print the information stored within a MemCell*/
void MC_print(MemCell * mc)
{
    if (mc)
    {
        /*printing the state of the MemCell*/
        if (mc -> cell_state == FREE)
        {
            printf("Cell State: FREE\n");
        }
        else if (mc -> cell_state == FULL)
        {
            printf("Cell State: FULL\n");
        }
        else
        {
            printf("Cell State: CSERROR\n");
        }

        /*printing MemCell Process contents*/
        if (mc -> contents)
        {
            P_print(mc -> contents);
        }
        else
        {
            printf("PID: nopid \t Size: 0");
        }

        /*printing write time*/
        printf("Write Time: %d\n\n", mc -> write_time);
    }
}

/*Destroys and deallocates a MemCell*/
void MC_destroy(MemCell * mc)
{
    if(mc)
    {
        if(mc -> contents)
        {
            P_destroy(mc -> contents);
        }

        free(mc);
        mc = NULL;
    }
}

/*Destroys and deallocates a table of MemCells*/
void MC_destroyTable(MemCell ** mc, int size)
{
    int loop_var = 0;

    if(mc)
    {
        for (loop_var = 0; loop_var < size; loop_var++)
        {
            MC_destroy(mc[loop_var]);
        }
    }

    free(mc);
    mc = NULL;
}

/*int main()
{
    MemCell ** table = MC_createTable(128);

    Process * p = P_create('A', 12);
    Process * p2 = P_create('B', 112);
    Process * p3 = P_create('C', 12);
    Process * p4 = P_create('D', 111);
    Process * p5 = P_create('E', 31);


    MC_setContents(table[0], p,1);
    MC_setContents(table[1], p2,1);
    MC_setContents(table[2], p3,1);
    MC_setContents(table[3], p4,1);
    MC_setContents(table[4], p5,1);

    MC_print(table[0]);
    MC_print(table[1]);

    return 0;
}*/
