OBJECTS = math.o main.o Particle.o Box.o World.o


all: main

main: $(OBJECTS)
	g++ -o main $(OBJECTS)
.cpp.o:
	g++ -o $@ -c $< -W -Wall

clean:
	rm -f *.o

