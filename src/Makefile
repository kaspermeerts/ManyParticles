OBJECTS = system.o main.o render.o setup.o
WARNINGS = -pedantic -Wextra -Wall -Wwrite-strings -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
OPTIM = -O0
CFLAGS = $(WARNINGS) -ansi -march=native -ggdb $(OPTIM)

all: main
optim:
	make -B OPTIM="-O3 -DNDEBUG"

main: $(OBJECTS)
	@echo "LD	main"
	@gcc -o main $(OBJECTS) -lm -lSDL -lGL -lGLU -ggdb

.c.o:
	@echo "CC	$@"
	@gcc -o $@ $(CFLAGS) -c $<

clean:
	rm -f *.o

