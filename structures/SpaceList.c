#include "SpaceList.h"

/*Create a new SpaceList object*/
SpaceList * SL_create()
{
    SpaceList * new_sl = malloc(sizeof(SpaceList));

    if(!new_sl)
    {
        perror("Could not allocate memory for SpaceList *. Exiting.\n");
        exit(0);
    }

    /*nothing in list, so all pointers point to null*/
    new_sl -> head = new_sl -> tail = new_sl -> itr = NULL;
    new_sl -> total_blocks = 0;

    return new_sl;
}

/*Create a new SLNode object for insertion into a SpaceList.
  Pass the create function base and limit Memory addresses to initialize*/
SLNode * SLN_create(int base, int limit)
{
    SLNode * new_sln = malloc(sizeof(SLNode));

    if(!new_sln)
    {
        perror("Could not allocate memory for SLNode *. Exiting\n");
        exit(0);
    }

    /*the block size is equal to the distance between the base and limit addresses*/
    new_sln -> block_size = (limit - base) + 1;
    new_sln -> base = base;
    new_sln -> limit = limit;
    new_sln -> next = NULL;

    return new_sln;
}

/*Used to determine if a SpaceList is empty or not
  Returns 1 if the list is empty
  Returns 0 if the list has more than 0 members*/
int SL_isEmtpy(SpaceList * sl)
{
    if(sl)
    {
        if(sl -> total_blocks == 0)
        {
            return 1;
        }
    }

    return 0;
}

/*Returns the next item in the SpaceList*/
SLNode * SL_getNext(SpaceList * sl)
{
    if (sl)
    {
        /*Store the current place in the list in a temp variable*/
        SLNode * temp =  sl -> itr;
        /*Move the iterator the next item*/
        sl -> itr = sl -> itr -> next;
        /*Return item stored in temp*/
        return temp;
    }

    return NULL;
}

/*Add a new SLNode onto the SpaceList.
  SLNode is created inside this function*/
void SL_add(SpaceList * sl, int base, int limit)
{
    if(sl)
    {
        /*create new node with passed base and limit addresses*/
        SLNode * new_sln = SLN_create(base, limit);

        /*if the SpaceList is empty...*/
        if(sl -> head == NULL)
        {
            /*first node is both head and tail until another added*/
            sl -> head = sl -> tail =  new_sln;
            /*iterator now points to the front of the list as well*/
            sl -> itr = sl -> head;
        }
        else
        {
            /*always add nodes at end of list*/

            /*node after the current tail points to the SLnode we are adding*/
            sl -> tail -> next = new_sln;
            /*point the tail to the newly added SLNode*/
            sl -> tail = sl -> tail -> next;
        }

        /*increase the total size of the list each time we add a SLNode*/
        sl -> total_blocks++;
    }
}

/*Print out all of the info stored in each SLNode in the SpaceList*/
void SL_print(SpaceList * sl)
{
    if(sl)
    {

        if(SL_isEmtpy(sl))
        {
            /*print error message if trying to print an empty list*/
            printf("Empty List\n");
        }
        else
        {
            SLNode * temp = sl -> head;

            /*start at head of SpaceList and go until you reach a NULL node
              (the end of the list)*/
            while (temp != NULL) /*print then iteratet*/
            {
                SLN_print(temp);
                temp = temp -> next;
            }
        }

        /*print the number of blocks in the list*/
        printf("Total Free Blocks: %d\n\n", sl -> total_blocks);
    }
}

/*Clears the SpaceList of all value but does not deallocate
  the memory originally allocated to the SpaceList * in SL_create()*/
void SL_clear(SpaceList * sl)
{
    if(sl)
    {
        SLNode * temp = sl -> head;

        /*deallocating memory allocated to each SLNode in the list*/
        while (temp != NULL)
        {
            sl -> head = sl -> head -> next;
            SLN_destroy(temp);
            temp = sl -> head;
        }

        /*resetting the values of the SpaceList to their defaults*/
        sl -> head = NULL;
        sl -> tail = NULL;
        sl -> itr = NULL;

        sl -> total_blocks = 0;
    }
}

/*Destroy and deallocate a SpaceList*/
void SL_destroy(SpaceList * sl)
{
    if(sl)
    {
        SLNode * temp = sl -> head;

        /*free each node in the list*/
        while (temp != NULL)
        {
            sl -> head = sl -> head -> next;
            SLN_destroy(temp);
            temp = sl -> head;
        }

        /*free the pointer to the object*/
        free(sl);
        sl = NULL;
    }
}

/*Used to print the contents of one SLNode*/
void SLN_print(SLNode * sln)
{
    if(sln)
    {
        printf("Size: %d\n", sln -> block_size);
        printf("Base: %d\n", sln -> base);
        printf("Limit: %d\n\n", sln -> limit);
    }
}

/*Destroy and deallocate a SLNode*/
void SLN_destroy(SLNode * sln)
{
    if(sln)
    {
        free(sln);
        sln = NULL;
    }
}

/*int main()
{
    SpaceList * sl = SL_create();
    SL_add(sl, 0, 19);
    SL_add(sl, 40, 44);
    SL_print(sl);

    SL_destroy(sl);

    return 0;

}*/
