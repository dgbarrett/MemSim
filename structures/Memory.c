#include "Memory.h"

/*Creates a new Memory object*/
Memory * M_create()
{
    Memory * new_mem = malloc(sizeof(Memory));

    if (!new_mem)
    {
        perror("Could not allocate memory for Memory. Exiting\n");
        exit(0);
    }

    new_mem -> cell = MC_createTable(MEM_SIZE);
    new_mem -> writes = 0;
    new_mem -> cumulative_mem_usage = 0;
    new_mem -> processes = 0;
    new_mem -> holes = 0;
    new_mem -> prev_limit = -1;
    new_mem -> free_blocks = SL_create();

    M_updateFreeBlocksList(new_mem);

    return new_mem;
}

/*Used to update the SpaceList of free blocks in memory*/
int M_updateFreeBlocksList(Memory * mem)
{
    if(mem)
    {
        /*clear the current SpaceList so we can re-build
          it based on the new state of the Memory*/
        SL_clear(mem -> free_blocks);

        int loop_var = 0;
        int temp_base = 0;
        int temp_limit = 0;

        /*loop through every cell in Memory*/
        while(loop_var < MEM_SIZE)
        {

            /*if the cell is empty we start recording a new open block*/
            if (1 == MC_isFree(mem -> cell[loop_var]))
            {

                /*save the current loop_var as the base of the new open block*/
                temp_base = loop_var;

                /*loop through following MemCells while they are free as well*/
                while(loop_var < MEM_SIZE)
                {
                    if(1 == MC_isFree(mem -> cell[loop_var]))
                    {
                        loop_var++;
                    }
                    else
                    {
                        break;
                    }
                }
                /*loop_var now at last free MemCell in block.
                  Save loop_var as limit address of new open block*/
                temp_limit = loop_var - 1;

                /*add the block to the SpaceList inside Memory*/
                SL_add(mem -> free_blocks, temp_base, temp_limit);


                /*reset temp variables for re-use*/
                temp_base = 0;
                temp_limit = 0;
            }
            loop_var++;

        }

        return 1;
    }

    return -1;
}

/*Used to compute where to insert the next Process into Memory based on
  the AllocationType*/
int M_getInsertPositionAccordingToStrategy(Memory * mem, ProcessQueue * mem_queue, AllocationType strat)
{
    int base = -1;

    if(mem && mem_queue)
    {
        switch(strat)
        {
            case FIRST_FIT:
                base = M_firstFit(mem, mem_queue);
                break;
            case WORST_FIT:
                base = M_worstFit(mem, mem_queue);
                break;
            case BEST_FIT:
                base = M_bestFit(mem, mem_queue);
                break;
            case NEXT_FIT:
                base = M_nextFit(mem, mem_queue);
                break;
            default:
                break;
        }
    }

    return base;
}

/*Returns the base address of the first block able to fit the next Process*/
int M_firstFit(Memory * mem, ProcessQueue * mem_queue)
{


    /*outer loop is for swapping Process' out of Memory*/
    while (1)
    {
        SLNode * temp_sln = mem -> free_blocks -> head;
        /*going through all blocks in SpaceList*/
        while(temp_sln)
        {
            if(temp_sln -> block_size >= mem -> current_process -> size)
            {
                /*if the block is big enough, return its base address*/
                return temp_sln -> base;
            }

            /*move to checking next block if previous was not adequate*/
            temp_sln = temp_sln -> next;
        }

        /*if we search through all of Memory and cannot find a hole big enough,
          we swap out one Process, then try to find a place to insert again*/

        M_swap(mem, mem_queue);
    }

    return -1;
}

/*Returns the base address of the largest block in Memory*/
int M_worstFit (Memory * mem, ProcessQueue * mem_queue)
{
    int largest = 0;
    int base = -1;

    /*allows us to keep swapping out Process' until we have a hole,
      big enough to insert into*/
    while (1)
    {
        SLNode * temp_sln = mem -> free_blocks -> head;

        while(temp_sln)
        {
            /*if the block is big enough to hold the Process*/
            if(temp_sln -> block_size >= mem -> current_process -> size)
            {
                /*we search for the biggest block in the SpaceList by keeping track
                  of the current biggest block and comparing that value to each other block
                  as the loop iterates through all of them*/
                if(temp_sln -> block_size > largest)
                {
                    /*if current block is larger than the value stored in largest,
                      we update largest to the new value, and set the base address to
                      the address of the new larger block*/
                    largest = temp_sln -> block_size;
                    base = temp_sln -> base;
                }
            }

            temp_sln = temp_sln -> next;
        }

        /*if the base is still unset, we swap out a Process,
          otherwise, break and return the base address*/
        if(base == -1)
        {
            M_swap(mem, mem_queue);
        }
        else
        {
            break;
        }
    }

    return base;
}

int M_bestFit (Memory * mem, ProcessQueue * mem_queue)
{
    int temp_base = -1;
    int base = -1;
    int temp_smallest = -1;
    int smallest = -1;
    int count = 0;
    int p_size;

    while(mem && mem_queue)
    {
        SLNode * temp = mem -> free_blocks -> head;
        p_size = mem -> current_process -> size;

        while (temp)
        {
            temp_base = temp -> base;
            temp_smallest = temp -> block_size;

            /*if the one we are examining is a better fit*/
            if (temp_smallest < smallest && temp_smallest >= p_size)
            {
                base = temp_base;
                smallest = temp_smallest;
            }
            else if (count == 0 && (temp_smallest >= p_size))
            {
                base = temp_base;
                smallest = temp_smallest;
            }

            temp = temp -> next;
        }

        if (base == -1)
        {
            M_swap(mem, mem_queue);
        }
        else
        {
            break;
        }
    }

    return base;
}

int M_nextFit(Memory * mem, ProcessQueue * mem_queue)
{
    SLNode * temp_sln = mem -> free_blocks -> head;
    int base;

    while(temp_sln)
    {

        /* if the block is after the previous insertion point in Memory*/
        if (temp_sln -> base > mem -> prev_limit)
        {
            /*if the block can fit the Process we need to insert into it*/
            if (temp_sln -> block_size >= mem -> current_process -> size)
            {
                mem -> prev_limit = mem -> current_process -> limit;
                base = temp_sln -> base;
                printf("badret\n");
                return base;
            }
        }
        temp_sln = temp_sln -> next;
    }

    base = M_firstFit(mem, mem_queue);

    return base;
}

/*Used to swap a Process out of Memory and into the ProcessQueue or out of the system
  if it has finished its execution*/
void M_swap(Memory * mem, ProcessQueue * mem_queue)
{
    int oldest = -1;
    int oldest_base = -1;
    int oldest_limit = -1;
    int loop_var = 0;
    Process * swap;

    while (loop_var < MEM_SIZE)
    {
        /*only take action on occupied MemCells*/
        if(1 == MC_isFull(mem -> cell[loop_var]))
        {
            /*if the oldest value is unset, we set it to the current value
              (first case scenario)*/
            if (oldest == -1)
            {
                /*set the time at which the oldest was written to Memory
                  for comparison with other processes later in loop*/
                oldest = mem -> cell[loop_var] -> write_time;

                /*set base and limit to reset, may be changed before loop exit*/
                oldest_base = mem -> cell[loop_var] -> contents -> base;
                oldest_limit = mem -> cell[loop_var] -> contents -> limit;
            }
            else
            {
                /*if not a first-case scenario, we check if the cell we are looking at
                  is older in Memory than the one we currently consider the oldest.
                  If it is, we replace the values with new oldest values */
                if (mem -> cell[loop_var] -> write_time < oldest)
                {
                    oldest = mem -> cell[loop_var] -> write_time;
                    oldest_base = mem -> cell[loop_var] -> contents -> base;
                    oldest_limit = mem -> cell[loop_var] -> contents -> limit;
                }
            }
        }

        ++loop_var;
    }

    swap = mem -> cell[oldest_base] -> contents;
    swap -> swaps++;

    if (swap -> swaps == 3)
    {
        printf("%c completed and exiting from memory.\n", swap -> pid);
    }
    else
    {
        PQ_add(mem_queue, swap);
        printf("%c swapping out.\n", swap -> pid);
    }

    M_resetRange(mem, oldest_base, oldest_limit);

    M_updateFreeBlocksList(mem);
}

/*Used to reset the MemCells within a specified range to their default values*/
void M_resetRange(Memory * mem, int base, int limit)
{
    if (mem)
    {
        int loop_var = 0;

        for (loop_var = base; loop_var <= limit; loop_var++)
        {
            mem -> cell[loop_var] -> write_time = -1;
            mem -> cell[loop_var] -> contents = NULL;
            mem -> cell[loop_var] -> cell_state = FREE;
        }
    }
}

/*Used to computer the number of holes in memory for statistical purposes*/
int M_countHoles(Memory * mem)
{
    if (mem)
    {
        /*number of holes in Memory is the same as the number
          of free blocks in memory*/
        return mem -> free_blocks -> total_blocks;
    }

    return -1;
}

/*Used to compute the number of processes currently in Memory*/
int M_countProcesses(Memory * mem)
{
    if (mem)
    {
        int loop_var = 0;
        int processes = 0;

        /*looping through all MemCells*/
        while (loop_var < MEM_SIZE)
        {
            /* if a cell is FULL, we have reached a Process in Memory */
            if(1 == MC_isFull(mem -> cell[loop_var]))
            {
                /*increment the number of  processes in Memory*/
                ++processes;

                /*set loop_var equal to the limit of the process you just counted*/
                loop_var += mem -> cell[loop_var] -> contents -> limit;
            }

            /*increment to next cell*/
            ++loop_var;
        }

        return processes;
    }

    return -1;
}

/*Used to compute how much of Memory is currently occupied*/
float M_memUsage(Memory * mem)
{
    SLNode * temp = mem -> free_blocks -> head;
    float unused = 0;
    float memusage = 0;

    while(temp)
    {
        /*summing the size of all free_blocks to determine unused space*/
        unused += temp -> block_size;

        temp = temp -> next;
    }

    /*subtract unused space from the size of Memory, calculate percentage*/
    memusage = ((MEM_SIZE - unused) / MEM_SIZE) * 100;

    return memusage;
}

/*Used to calculate and print the statistics of the current Memory state*/
void M_calculateStatisticsAndPrint(Memory * mem, char pid)
{

    int holes = M_countHoles(mem);
    int processes = M_countProcesses(mem);
    float memusage = M_memUsage(mem);
    float cumulative_usage;



    mem -> holes += holes;
    mem -> processes += processes;
    mem -> cumulative_mem_usage += memusage;

    /*cumulative usage is equal to the total of all usages divided by the number
      of times we have written to memory*/
    cumulative_usage = (mem -> cumulative_mem_usage)/(mem -> writes);

    printf("%c loaded, #processes = %d, #holes = %d, %%memusage = %.0f, cumulative %%memusage = %.0f\n", pid, processes, holes, memusage, cumulative_usage);
}

void M_printFinalStatistics(Memory * mem)
{
    float avg_holes = (float)(mem -> holes)/(mem -> writes);
    float avg_processes = (float)(mem -> processes)/(mem -> writes);
    float avg_mem_usage = (float)(mem ->cumulative_mem_usage)/(mem -> writes);

    printf("Total loads = %d, avg #processes = %.2f, avg #holes = %.2f, cumulative %%mem = %.2f\n\n\n", mem -> writes, avg_processes, avg_holes, avg_mem_usage);
}

/*Used to insert a Process in Memory starting at the address of 'base'*/
void M_insertAt(Memory * mem, int base)
{
    if (mem)
    {
        int loop_var = 0;
        int temp_limit = base + (mem -> current_process -> size);
        char pid = mem -> current_process -> pid;

        /*setting attributes of process about to be inserted into Memory*/
        mem -> current_process -> base = base;
        mem -> current_process -> limit = temp_limit - 1;

        for(loop_var = base; loop_var < temp_limit; loop_var++)
        {
            /*resetting to default*/
            mem -> cell[loop_var] -> contents = mem -> current_process;
            mem -> cell[loop_var] -> cell_state = FULL;
            mem -> cell[loop_var] -> write_time = mem -> writes;
        }

        /*increase total number of writes done by Memory*/
        mem -> writes++;
        mem -> current_process = NULL;

         M_updateFreeBlocksList(mem);


        /*print out what just happened*/
        M_calculateStatisticsAndPrint(mem, pid);
    }
}

/*Used to set the Process which the Memory is working on,
  Process grabbed from ProcessQueue*/
void M_setNextProcess(Memory * mem, ProcessQueue * mem_queue)
{
    if(mem && mem_queue)
    {
        mem -> current_process = PQ_remove(mem_queue);
    }
}

/*dispatches to correct allocation pattern function*/
void M_executeMemSim(Memory * mem, ProcessQueue * mem_queue, AllocationType strat)
{
    if(!mem || !mem_queue)
    {
        return;
    }

    int base = 0;

    while(!PQ_isEmpty(mem_queue))
    {
        /*grab Process from ProcessQueue*/
        M_setNextProcess(mem, mem_queue);

        /*find proper base address based on AllocationType*/
        base = M_getInsertPositionAccordingToStrategy(mem, mem_queue, strat);
        printf("%c base %d\n", mem -> current_process -> pid, base);
        printf("%c size %d\n", mem -> current_process -> pid, mem -> current_process -> size);

        /*insert the Process into Memory, starting at base address*/
        M_insertAt(mem, base);

    }

    M_printFinalStatistics(mem);
}

void M_print(Memory * mem)
{
    int loop_var = 0;

    for (loop_var = 0; loop_var < MEM_SIZE; loop_var++)
    {
        if (MC_isFull(mem -> cell[loop_var]))
        {
            printf("Cell %d: %c\n", loop_var, mem -> cell[loop_var] -> contents -> pid);
        }
        else
        {
            printf("Cell %d: FREE\n", loop_var);
        }
    }
}

/*destroys a Memory instance*/
void M_destroy(Memory * mem)
{
    if(mem)
    {
        MC_destroyTable(mem -> cell, MEM_SIZE);

        free(mem);
        mem = NULL;
    }
}

/*int main()
{
    Memory * mem = M_create();
    ProcessQueue * pq = PQ_create();
    Process * p = P_create('A', 10);
    Process * p2 = P_create('B', 20);
    Process * p3 = P_create('C', 40);
    Process * p4 = P_create('d', 35);
    Process * p5 = P_create('e', 14);
    Process * p6 = P_create('f', 55);
    Process * p7 = P_create('g', 28);
    Process * p8 = P_create('h', 31);

    PQ_add(pq, p);
    PQ_add(pq, p2);
    PQ_add(pq, p3);
    PQ_add(pq, p4);
    PQ_add(pq, p5);
    PQ_add(pq, p6);
    PQ_add(pq, p7);
    PQ_add(pq, p8);

    M_executeMemSim(mem, pq, FIRST_FIT);






    M_swap(mem, pq);

    M_print(mem);

    PQ_print(pq);

    M_print(mem);

    return 0;
}*/




