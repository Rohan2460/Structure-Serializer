CC = gcc
CFLAGS = -Wall -Wextra -g

TARGET = serialize
OBJ = serialize.o test.o 

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(CFLAGS) -o $@ $^

test.o: test.c serialize.h
	$(CC) $(CFLAGS) -c test.c

serialize.o: serialize.c serialize.h
	$(CC) $(CFLAGS) -c serialize.c

clean: 
	rm $(TARGET) $(OBJ)