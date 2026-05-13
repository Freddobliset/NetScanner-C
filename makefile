CC = gcc
CFLAGS = -Wall -Iinclude
SRC_DIR = src
BIN_DIR = bin

# Find all .c files in src/
SRC = $(wildcard $(SRC_DIR)/*.c)
# Define object files (e.g., src/main.o)
OBJ = $(SRC:.c=.o)
# Final output name
TARGET = $(BIN_DIR)/netscanner

all: $(TARGET)

# Link the object files into the final binary in bin/
$(TARGET): $(OBJ)
	@mkdir -p $(BIN_DIR)
	$(CC) $(OBJ) -o $(TARGET)

# Compile .c files into .o files
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up src folder and bin folder
clean:
	rm -f $(SRC_DIR)/*.o $(TARGET)