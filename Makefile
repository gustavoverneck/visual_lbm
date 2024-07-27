# Compiler and Flags
CC = g++
CFLAGS = -Wall -std=c++20

# Targer Executable and Dependencies
TARGET = main
OBJS = $(TARGET).o

# SDL2 Libraries and Include Paths
SDL_LIBS = -lSDL2

# Build Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(SDL_LIBS)

$(TARGET).o: $(TARGET).cpp
	$(CC) $(CFLAGS) -c $(TARGET).cpp $(SDL_LIBS)

# Clean
clean:
	rm -f $(TARGET) *.o

# Run
run:
	./$(TARGET)