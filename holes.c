#include "holes.h"

int main (int argc, char ** argv)
{
    int running = 1;

    while(running < 5)
    {
        ProcessQueue * mem_queue = parse(argv[1]);
        Memory * memory = M_create();

        switch(running)
        {
            case 4:
                printf("----------BEST FIT----------\n");
                M_executeMemSim(memory, mem_queue, BEST_FIT);
                break;
            case 3:
                printf("----------NEXT FIT----------\n");
                M_executeMemSim(memory, mem_queue, NEXT_FIT);
                break;
            case 2:
                printf("----------WORST FIT----------\n");
                M_executeMemSim(memory, mem_queue, WORST_FIT);
                break;
            case 1:
                printf("----------FIRST FIT----------\n");
                M_executeMemSim(memory, mem_queue, FIRST_FIT);
                break;
            default:
                printf("Fatal Error. Exiting\n");
                exit(0);
                break;
        }
        ++running;
    }

    return 0;
}
