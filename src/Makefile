OBJECTS = system.o


all: main

main: $(OBJECTS)
	gcc -o main $(OBJECTS) -lm

.c.o:
	gcc -o $@ -c $< -W -Wall -ggdb

clean:
	rm -f *.o

