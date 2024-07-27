#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <tuple>
#include <map>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;


// Buttons dimensions
const int BUTTON_WIDTH = 150;
const int BUTTON_HEIGHT = 50;

// Canva parameters
constexpr int SCREEN_WIDTH = 800+BUTTON_WIDTH;
constexpr int SCREEN_HEIGHT = 800;
constexpr int GRID_HEIGHT = SCREEN_HEIGHT;
constexpr int GRID_WIDTH = SCREEN_WIDTH-BUTTON_WIDTH;

constexpr int GRID_SIZE = 50;
constexpr int CELL_SIZE = GRID_WIDTH / GRID_SIZE;

constexpr int width = BUTTON_WIDTH;
constexpr int height = BUTTON_HEIGHT;

// Colors parameters
std::tuple<int, int, int> LINE_COLOR= {65, 65, 65};  // White color
std::tuple<int, int, int> OBSTACLE_COLOR = {255, 0, 0}; // Red color
std::tuple<int, int, int> INFLOW_COLOR = {0, 0, 255};    // Blue color
std::tuple<int, int, int> PIPEWALL_COLOR = {0, 255, 0};   // Green color
std::tuple<int, int, int> SAVEBUTTON_COLOR = {255, 255, 0}; // Yellow color
std::tuple<int, int, int> TEXT_COLOR = {0, 0 ,0}; // Black
std::tuple<int, int, int> EMPTYBUTTON_COLOR = {0, 0, 0}; // Grey

// Font parameters
const int FONT_SIZE = 24;
const char* FONT_PATH = "static/SIXTY.TTF";


// Data Structures
struct ButtonParams {
    int x;
    int y;
    int width;
    int height;
    std::string text;
    std::tuple<int, int, int> color;
    std::tuple<int, int, int> text_color;
};

class Button {
    public:
        int x;
        int y;
        int width;
        int height;
        std::string text;
        std::tuple<int, int, int> color;
        std::tuple<int, int, int> text_color = TEXT_COLOR;

        Button(const ButtonParams& params) :
            x(params.x),
            y(params.y),
            width(params.width),
            height(params.height),
            text(params.text),
            color(params.color),
            text_color(params.text_color)
        {}
    
        bool isInside(int x, int y) {
                return x > this->x && x < this->x + this->width && y > this->y && y < this->y + this->height;
            }
        
        void drawButton(SDL_Renderer* renderer) {
            SDL_SetRenderDrawColor(renderer, std::get<0>(this->color), std::get<1>(this->color), std::get<2>(this->color), SDL_ALPHA_OPAQUE);
            SDL_Rect buttonRect = { this->x, this->y, this->width, this->height };
            SDL_RenderFillRect(renderer, &buttonRect);
            // Add text to the button using SDL_ttf
        }
        void drawButtonText(SDL_Renderer* renderer, TTF_Font* font) {
            SDL_Color textColor = { std::get<0>(this->text_color), std::get<1>(this->text_color), std::get<2>(this->text_color) };
            SDL_Surface* surfaceMessage = TTF_RenderText_Solid(font, this->text.c_str(), textColor);
            SDL_Texture* message = SDL_CreateTextureFromSurface(renderer, surfaceMessage);

            int textW = 0;
            int textH = 0;
            SDL_QueryTexture(message, NULL, NULL, &textW, &textH);
            SDL_Rect textRect = { this->x + (this->width - textW) / 2, this->y + (this->height - textH) / 2, textW, textH };

            SDL_RenderCopy(renderer, message, NULL, &textRect);
            SDL_FreeSurface(surfaceMessage);
            SDL_DestroyTexture(message);
        }
};


std::vector<std::vector<int>> obstacles(GRID_SIZE, std::vector<int>(GRID_SIZE, 0));
std::vector<Button> buttons;    // Empty vector to store all buttons

// Functions
void drawGrid(SDL_Renderer* renderer) {
    SDL_SetRenderDrawColor(renderer, std::get<0>(LINE_COLOR), std::get<1>(LINE_COLOR), std::get<2>(LINE_COLOR), SDL_ALPHA_OPAQUE);
    for (int i = 0; i <= GRID_SIZE; ++i) {
        SDL_RenderDrawLine(renderer, i * CELL_SIZE, 0, i * CELL_SIZE, GRID_HEIGHT);
        SDL_RenderDrawLine(renderer, 0, i * CELL_SIZE, GRID_WIDTH, i * CELL_SIZE);
    }
}

void drawCells(SDL_Renderer* renderer) {
    for (int i = 0; i < GRID_SIZE; ++i) {
        for (int j = 0; j < GRID_SIZE; ++j) {
            if (obstacles[i][j] == 0) { // Empty
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            }
            else if (obstacles[i][j] == 1) { // Obstacle
                SDL_SetRenderDrawColor(renderer, 255, 0, 0, SDL_ALPHA_OPAQUE);
            }
            else if (obstacles[i][j] == 2) { // Wall
                SDL_SetRenderDrawColor(renderer, 0, 255, 0, SDL_ALPHA_OPAQUE);
            }
            else if (obstacles[i][j] == 3) { // Inflow
                SDL_SetRenderDrawColor(renderer, 0, 0, 255, SDL_ALPHA_OPAQUE);
            }
            else {
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
            }
            SDL_Rect fillRect = { i * CELL_SIZE, j * CELL_SIZE, CELL_SIZE, CELL_SIZE };
            SDL_RenderFillRect(renderer, &fillRect);
        }
    }
}

void createButtons(SDL_Renderer* renderer, std::vector<Button>& buttons){
    // Create a button to save the state
    buttons.push_back(Button({
    .x = SCREEN_WIDTH - BUTTON_WIDTH, 
    .y = 0,
    .width = BUTTON_WIDTH,
    .height = BUTTON_HEIGHT,
    .text = "Save",
    .color = SAVEBUTTON_COLOR,
    .text_color = TEXT_COLOR
    }));

    // Create a button to draw walls
    buttons.push_back(Button({
        .x = SCREEN_WIDTH - BUTTON_WIDTH,
        .y = BUTTON_HEIGHT, .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .text = "Wall",
        .color = PIPEWALL_COLOR,
        .text_color = TEXT_COLOR                 
    }));

    // Create a button to draw obstacles
    buttons.push_back(Button({
        .x = SCREEN_WIDTH - BUTTON_WIDTH,
        .y = 2*BUTTON_HEIGHT, .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .text = "+obst",
        .color = OBSTACLE_COLOR,
        .text_color = TEXT_COLOR
    }));

    // Create a button to draw inflow
    buttons.push_back(Button({
        .x = SCREEN_WIDTH - BUTTON_WIDTH,
        .y = 3*BUTTON_HEIGHT,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .text = "Inflow",
        .color = INFLOW_COLOR,
        .text_color = TEXT_COLOR
    }));

    // Create a button to draw empty space (eraser)
    buttons.push_back(Button({
        .x = SCREEN_WIDTH - BUTTON_WIDTH,
        .y = 4*BUTTON_HEIGHT,
        .width = BUTTON_WIDTH,
        .height = BUTTON_HEIGHT,
        .text = "+empty",
        .color = EMPTYBUTTON_COLOR,
        .text_color = {255, 255, 255}
    }));
}

void saveState() {
    std::ofstream outFile("state.txt");
    if (outFile.is_open()) {
        for (int i = 0; i < GRID_SIZE; ++i) {
            for (int j = 0; j < GRID_SIZE; ++j) {
                outFile << obstacles[j][i] << " ";
            }
            outFile << "\n";
        }
        outFile.close();
        std::cout << "State saved to state.txt" << std::endl;
    }
}



// Main function
int main(int argc, char* args[]) {
    fs::path outputDirectoryPath = "output"; // Path to your output directory
    try {
        if (!fs::exists(outputDirectoryPath)) {
            fs::create_directory(outputDirectoryPath);
            std::cout << "Output directory created successfully." << std::endl;
        } else {
            std::cout << "Output directory already exists." << std::endl;
        }
    } catch (const fs::filesystem_error& e) {
        std::cerr << "Error creating output directory: " << e.what() << std::endl;
    }

    
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

    if (TTF_Init() == -1) {
        std::cerr << "SDL_ttf could not initialize! TTF_Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    TTF_Font* font = TTF_OpenFont(FONT_PATH, FONT_SIZE); // Font size 24
    if (!font) {
        std::cerr << "Failed to load font! TTF_Error: " << TTF_GetError() << std::endl;
        return -1;
    }

    createButtons(renderer, buttons);

    bool quit = false;
    SDL_Event e;
    int action = 0;
    while (!quit) {
        while (SDL_PollEvent(&e) != 0) {
            if (e.type == SDL_QUIT) {
                quit = true;
            } else if (e.type == SDL_MOUSEBUTTONDOWN) {
                int x, y;
                SDL_GetMouseState(&x, &y);
                bool insideButton = false;
                for (Button b : buttons){
                    if (b.isInside(x, y)){
                        if (b.text == "Save"){
                            action = 0;
                            saveState();
                            insideButton = true;
                            std::cout << "Action updated: " << action << "\n";
                            break;
                        }
                        else if (b.text == "Wall"){
                            action = 2;
                            insideButton = true;
                            std::cout << "Action: " << action << "\n";
                            break;                        }
                        else if (b.text == "+obst"){
                            action = 1;
                            insideButton = true;
                            std::cout << "Action updated: " << action << "\n";
                            break; 
                        }
                        else if (b.text == "Inflow"){
                            action = 3;
                            insideButton = true;
                            std::cout << "Action updated: " << action << "\n";
                            break;
                        }
                        else{
                            action = 0;
                            insideButton = true;
                            std::cout << "Action updated: " << action << "\n";
                            break; 
                        }
                    }
                }
                if (!insideButton) {
                    int gridX = x / CELL_SIZE;
                    int gridY = y / CELL_SIZE;
                    obstacles[gridX][gridY] = action; // Toggle obstacle
                }
            }
        }

        SDL_SetRenderDrawColor(renderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(renderer);
        drawCells(renderer);
        drawGrid(renderer);

        for (Button& b : buttons) {
            b.drawButton(renderer);
            b.drawButtonText(renderer, font); // Render button text
        }


        SDL_RenderPresent(renderer);
    }
    
    TTF_CloseFont(font);
    TTF_Quit(); // Quit SDL_ttf
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
