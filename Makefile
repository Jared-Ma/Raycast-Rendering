CFLAGS = -O2
CC = g++

all: main.o arrow.o wall.o ray.o helper.o
	$(CC) $(CFLAGS) -o raycast main.o arrow.o wall.o ray.o helper.o -lsfml-graphics -lsfml-window -lsfml-system

main.o: main.cpp
	$(CC) $(CFLAGS) -c main.cpp

arrow.o: arrow.cpp
	$(CC) $(CFLAGS) -c arrow.cpp

wall.o: wall.cpp
	$(CC) $(CFLAGS) -c wall.cpp

ray.o: ray.cpp
	$(CC) $(CFLAGS) -c ray.cpp

helper.o: helper.cpp
	$(CC) $(CFLAGS) -c helper.cpp

clean:
	rm -f core *.o