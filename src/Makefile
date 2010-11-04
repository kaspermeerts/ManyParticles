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
O0:
	make -B OPTIM="-O0 -DNDEBUG"
O1:
	make -B OPTIM="-O1 -DNDEBUG"
O2:
	make -B OPTIM="-O2 -DNDEBUG"
O3:
	make -B OPTIM="-O3 -DNDEBUG"
brown:
	make -B BROWN="-DBROWNIAN -O3 -DNDEBUG"

main: $(OBJECTS)
	@echo "LD	main"
	@gcc -o main $(OBJECTS) -lm -lSDL -lGL -ggdb $(PROFILE)

.c.o:
	@echo "CC	$@"
	@gcc -o $@ $(CFLAGS) -c $<

clean:
	rm -f *.o

