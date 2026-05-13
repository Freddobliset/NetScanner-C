CC = gcc
CFLAGS = -Wall -Iinclude
SRC = src/main.c src/scanner.c src/utils.c
OBJ = $(SRC:.c=.o)
TARGET = bin/netscanner

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f src/*.o $(TARGET)