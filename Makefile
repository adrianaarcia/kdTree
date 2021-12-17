CC = gcc
CFLAGS = -std=c99 -pedantic -Wall -g3 -lm

kUnit: kdtree.o location.o kdtree_unit.c
	${CC} ${CFLAGS} -o $@ $^

kdtree.o: kdtree.h kdtree.c 
	${CC} ${CFLAGS} -c kdtree.c

location.o: location.c location.h
	${CC} ${CFLAGS} -c location.c

clean:
	rm -r *.o Unit