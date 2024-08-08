# Compilador e flags
CC = g++
CFLAGS = -Wall -std=c++20 -g

# SDL2 Libraries and Include Paths
SDL_LIBS = -lSDL2 -lSDL2_ttf -lstdc++fs

# Arquivos de origem e objetos
SRC_VISUAL = visual.cpp
SRC_LBM = lbm.cpp
OBJ_VISUAL = $(SRC_VISUAL:.cpp=.o)
OBJ_LBM = $(SRC_LBM:.cpp=.o)

# Executáveis
TARGET_VISUAL = visual
TARGET_LBM = lbm

# Build Rules
all: $(TARGET_VISUAL) $(TARGET_LBM)

$(TARGET_VISUAL): $(OBJ_VISUAL)
	$(CC) $(CFLAGS) -o $@ $^ $(SDL_LIBS)

$(TARGET_LBM): $(OBJ_LBM)
	$(CC) $(CFLAGS) -o $@ $^

%.o: %.cpp
	$(CC) $(CFLAGS) -c $< -o $@

# Clean
clean:
	rm -f $(TARGET_VISUAL) $(TARGET_LBM) *.o
	rm -f -r output
	rm -f -r state.txt

# Run
run: $(TARGET_VISUAL) $(TARGET_LBM)
	./$(TARGET_VISUAL)
	./$(TARGET_LBM)
