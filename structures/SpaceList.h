#ifndef __SPACELIST_H__
#define __SPACELIST_H__

#include <stdio.h>
#include <stdlib.h>


/*Node type of the SpaceList*/
typedef struct SLNode{
    int block_size;          /*Size of the open block in memory*/
    int base;                /*Starting address of the block*/
    int limit;               /*Ending address of the block*/
    struct SLNode * next;    /*pointer to the next item (next free block)*/
}SLNode;

/*The SpaceList holds a list of the free blocks in memory*/
typedef struct SpaceList{
    SLNode * head;      /*start of the list*/
    SLNode * tail;      /*end of the list*/
    SLNode * itr;       /*used to iterate over the list, allows you to call the getNext function
                        in a loop to individually retreive each item from the list*/
    int total_blocks;   /*number of free blocks in Memory*/
}SpaceList;

/*SpaceList functions*/
int             SL_isEmtpy      (SpaceList * sl);
SpaceList *     SL_create       ();
SLNode *        SL_getNext      (SpaceList * sl);
void            SL_add          (SpaceList * sl, int base, int limit);
void            SL_print        (SpaceList * sl);
void            SL_destroy      (SpaceList * sl);
void            SL_clear        (SpaceList * sl);

/*SLNode functions*/
SLNode *        SLN_create(int base, int limit);
void            SLN_print(SLNode * sln);
void            SLN_destroy(SLNode * sln);

#endif /* __SPACELIST_H__ */
