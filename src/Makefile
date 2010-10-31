OBJECTS = system.o main.o render.o
WARNINGS = -pedantic -Wextra -Wall -Wwrite-strings -Wshadow -Wcast-qual -Wstrict-prototypes -Wmissing-prototypes
BROWN = 
PROFILE = 
OPTIM = -O3 -DNDEBUG
CFLAGS = $(WARNINGS) -ansi -march=native -ggdb $(OPTIM) $(PROFILE) $(BROWN)

all: main
profile:
	make -B PROFILE="-pg -DNDEBUG"
debug:
	make -B OPTIM="-O0 -DDEBUG"
brown:
	make -B BROWN="-DBROWNIAN -O3 -DNDEBUG"

main: $(OBJECTS)
	@echo "LD	main"
	@gcc -o main $(OBJECTS) -lm -lSDL -lGL -lGLU -ggdb $(PROFILE)

.c.o:
	@echo "CC	$@"
	@gcc -o $@ $(CFLAGS) -c $<

clean:
	rm -f *.o

