CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = console.h createList.h FileSystem.h NormalMode.h CommandMode.h
OBJ = console.o createList.o FileSystem.o NormalMode.o CommandMode.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

console: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

