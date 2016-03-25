#include "parser.h"

ProcessQueue * parse(char * filename)
{
    FILE * fp = fopen(filename, "r");
    ProcessQueue * pq = PQ_create();
    Process * temp = NULL;

    char pid = '\0';
    int size = 0;

    if(!fp)
    {
        perror("Could not open file. Exiting.\n");
        exit(0);
    }

    while(fscanf(fp, " %c %d\n", &pid, &size) != EOF) /*need space before %c to protect against newline*/
    {
        temp = P_create(pid, size);

        PQ_add(pq, temp);
    }

    return pq;
}
