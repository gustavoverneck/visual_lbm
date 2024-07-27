#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>

// LBM parameters
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;
const int GRID_SIZE = 50;
const int CELL_SIZE = SCREEN_WIDTH / GRID_SIZE;


struct Cell {
    double density;
};

std::vector<std::vector<Cell>> grid(GRID_SIZE, std::vector<Cell>(GRID_SIZE));
std::vector<std::vector<bool>> obstacles(GRID_SIZE, std::vector<bool>(GRID_SIZE, false));

// Button dimensions
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 50;
const int BUTTON_X = SCREEN_WIDTH - BUTTON_WIDTH - 10;
const int BUTTON_Y = SCREEN_HEIGHT - BUTTON_HEIGHT - 10;


void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, SDL_ALPHA_OPAQUE);
    for (int i = 0; i <= GRID_SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, SCREEN_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, SCREEN_WIDTH, i * CELL_SIZE);
    }
}

void drawObstacles(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (obstacles[i][j]) {
                SDL_Rect fillRect = { i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE };
                SDL_RenderFillRect(renderer, &fillRect);
            }
        }
    }
}

void saveState() {
    std::ofstream outFile("obstacles.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                outFile << obstacles[j][i] << " ";
            }
            outFile << "\n";
        }
        outFile.close();
        std::cout << "State saved to obstacles.txt" << std::endl;
    }
}

void drawButton(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
    SDL_Rect buttonRect = { BUTTON_X, BUTTON_Y, BUTTON_WIDTH, BUTTON_HEIGHT };
    SDL_RenderFillRect(renderer, &buttonRect);
    // Add text to the button using SDL_ttf if desired (not shown here for simplicity)
}

bool isInsideButton(int x, int y) {
    return x > BUTTON_X && x < BUTTON_X + BUTTON_WIDTH && y > BUTTON_Y && y < BUTTON_Y + BUTTON_HEIGHT;
}


int main(int argc, char* args[]) {
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cerr << "SDL could not initialize! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Window* window = SDL_CreateWindow("LBM Grid", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Window could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    SDL_Renderer* renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Renderer could not be created! SDL_Error: " << SDL_GetError() << std::endl;
        return -1;
    }

    bool quit = false;
    SDL_Event e;

    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (isInsideButton(x, y)) {
                    saveState();
                    quit = true;
                } else {
                    int gridX = x / CELL_SIZE;
                    int gridY = y / CELL_SIZE;
                    obstacles[gridX][gridY] = !obstacles[gridX][gridY]; // Toggle obstacle
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);

        drawGrid(renderer);
        drawObstacles(renderer);
        drawButton(renderer);

        SDL_RenderPresent(renderer);
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
