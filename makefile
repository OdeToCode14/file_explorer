CC = g++
CFLAGS = -Wall -std=c++1z
DEPS = createList.h FileSystem.h NormalMode.h
OBJ = console.o createList.o FileSystem.o NormalMode.o
%.o: %.cpp $(DEPS)
	$(CC) $(CFLAGS) -c -o $@ $<

console: $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

