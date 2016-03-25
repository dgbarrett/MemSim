#include <stdio.h>
#include <stdlib.h>

int main(int argc, char ** argv)
{
    FILE * fp = fopen("test.txt", "w");
    int lines = atoi(argv[1]);
    int size = atoi(argv[2]);

    if (fp)
    {
        for (lines; lines > 0; lines--)
        {
            int ran = (rand() % size) + 1;

            fprintf(fp, "A %d\n", ran);
        }
    }

    fclose(fp);
}

