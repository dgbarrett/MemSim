COMPILE = gcc -c
LINK = gcc -o
FLAGS = -Wall -ansi

parser : parser.o ProcessQueue.o Process.o
	$(LINK) parser parser.o ProcessQueue.o Process.o

parser.o : parser.c parser.h
	$(COMPILE) $(FLAGS) parser.c

ProcessQueue.o : ../structures/ProcessQueue.c ../structures/ProcessQueue.h
	$(COMPILE) $(FLAGS) ../structures/ProcessQueue.c

Process.o : ../structures/Process.c ../structures/Process.h
	$(COMPILE) $(FLAGS) ../structures/Process.c

clean :
	rm *.o
	rm *.exe
