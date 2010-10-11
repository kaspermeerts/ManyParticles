OBJECTS = Particle.o main.o


all: main

main: $(OBJECTS)
	g++ -o main $(OBJECTS)
.cpp.o:
	g++ -o $@ -c $< -W -Wall

clean:
	rm -f *.o

