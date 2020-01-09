CFLAGS = -O2
CC = g++

all: main.o arrow.o wall.o
	$(CC) $(CFLAGS) -o raycast main.o arrow.o wall.o -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

arrow.o: arrow.cpp
	$(CC) $(CFLAGS) -c arrow.cpp

wall.o: wall.cpp
	$(CC) $(CFLAGS) -c wall.cpp

clean:
	rm -f core *.o