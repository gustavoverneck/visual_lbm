# Compiler and Flags
CC = g++
CFLAGS = -Wall -std=c++20

# Targer Executable and Dependencies
TARGET = main
OBJS = $(TARGET).o

# SDL2 Libraries and Include Paths
SDL_LIBS = -lSDL2 -lSDL2_ttf -lstdc++fs

# Build Rules
all: $(TARGET)

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJS) $(SDL_LIBS)

$(TARGET).o: $(TARGET).cpp
	$(CC) $(CFLAGS) -c $(TARGET).cpp $(SDL_LIBS)

# Clean
clean:
	rm -f $(TARGET) *.o
	rm -f -r output
	rm -f -r state.txt

# Run
run:
	./$(TARGET)