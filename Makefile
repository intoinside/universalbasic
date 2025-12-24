CC = gcc
CFLAGS = -Iinclude -Wall -Wextra -std=c99
SRC = src/core/tokenizer.c src/core/parser.c src/core/program.c src/core/runtime.c src/platform/stdio/pal_stdio.c main.c
OBJ = $(SRC:.c=.o)
TARGET = basic.exe

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)
