COMPILE = gcc -c
LINK = gcc -o
FLAGS = -Wall -ansi

Memory : Memory.o MemCell.o Process.o SpaceList.o ProcessQueue.o
	$(LINK) Memory Memory.o MemCell.o Process.o SpaceList.o ProcessQueue.o

MemCell : MemCell.o Process.o
	$(LINK) MemCell MemCell.o Process.o

ProcessQueue: ProcessQueue.o Process.o
	$(LINK) ProcessQueue ProcessQueue.o Process.o

Process : Process.o
	$(LINK) Process Process.o

SpaceList : SpaceList.o
	$(LINK) SpaceList SpaceList.o

SpaceList.o : SpaceList.c SpaceList.h
	$(COMPILE) $(FLAGS) SpaceList.c

Memory.o : Memory.c Memory.h
	$(COMPILE) $(FLAGS) Memory.c

MemCell.o : MemCell.c MemCell.h
	$(COMPILE) $(FLAGS) MemCell.c

ProcessQueue.o : ProcessQueue.c ProcessQueue.h
	$(COMPILE) $(FLAGS) ProcessQueue.c

Process.o : Process.c Process.h
	$(COMPILE) $(FLAGS) Process.c

clean :
	rm *.o
	rm *.exe
