COMPILE = gcc -c
LINK = gcc -o
FLAGS = -Wall -ansi\

all : MemSim

MemSim : holes.o parser.o ProcessQueue.o Process.o Memory.o SpaceList.o MemCell.o
	$(LINK) holes holes.o parser.o ProcessQueue.o Process.o Memory.o SpaceList.o MemCell.o

holes.o : holes.c holes.h
	$(COMPILE) $(FLAGS) holes.c

Memory.o : structures/Memory.c structures/Memory.h
	$(COMPILE) $(FLAGS) structures/Memory.c

MemCell.o : structures/MemCell.c structures/MemCell.h
	$(COMPILE) $(FLAGS) structures/MemCell.c

SpaceList.o : structures/SpaceList.c structures/SpaceList.h
	$(COMPILE) $(FLAGS) structures/SpaceList.c

parser.o :  parser/parser.c parser/parser.h
	$(COMPILE) $(FLAGS) parser/parser.c

ProcessQueue.o : structures/ProcessQueue.c structures/ProcessQueue.h
	$(COMPILE) $(FLAGS) structures/ProcessQueue.c

Process.o : structures/Process.c structures/Process.h
	$(COMPILE) $(FLAGS) structures/Process.c

clean :
	rm *.o
	rm *.exe
