OBJECTS = system.o main.o render.o
WARNINGS = -pedantic -Wextra -Wall -Wwrite-strings -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
OPTIM = -O0
CFLAGS = $(WARNINGS) -ansi -march=native -ggdb $(OPTIM)

all: main
optim:
	make -B OPTIM="-O3"

main: $(OBJECTS)
	gcc -o main $(OBJECTS) -lm -lSDL -lGL -lGLU -ggdb

.c.o:
	gcc -o $@ $(CFLAGS) -c $< -DNDEBUG

clean:
	rm -f *.o

