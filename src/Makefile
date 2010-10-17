OBJECTS = system.o
WARNINGS = -pedantic -Wextra -Wall -Wwrite-strings -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
CFLAGS = $(WARNINGS) -ansi -march=native -O3


all: main

main: $(OBJECTS)
	gcc -o main $(OBJECTS) -lm

.c.o:
	gcc -o $@ $(CFLAGS) -c $<

clean:
	rm -f *.o

