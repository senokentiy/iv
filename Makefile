CC = gcc
CFLAGS = -g -Wall -Wextra
LIB = `sdl2-config --cflags --libs`
TARGET = iv
SOURCES = main.c

all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $(TARGET) $(SOURCES) $(LIB)

clean:
	rm -f $(TARGET)

