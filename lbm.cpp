#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>

class LBM {
    public:
        int n;
        int m;
        // Vetor de vetores para armazenar a matriz
        std::vector<std::vector<int>> grid;
        
        // Initializes the LBM object and reads the input file
        int init(std::string input_dir_) {
            // Read the input file
            std::ifstream inputFile(input_dir_);
            // Check if the file is successfully opened 
            if (!inputFile.is_open()) { 
                std::cerr << "Error opening the file!" << std::endl; 
            }
            // Read the grid informations at input file
            inputFile >> n >> m;

            // Variable to store the values of the each line
            std::string line;

            // Read file line by line
            while (std::getline(inputFile, line)) {
                // Vetor para armazenar uma linha da matriz
                std::vector<int> matrixLine;
                // Uses a stringstream to divide the line into individual elements
                std::stringstream ss(line);
                int valor;
                // Lê cada valor da linha
                while (ss >> valor) {
                    matrixLine.push_back(valor);
                }
                // Adds line to the matrix
                grid.push_back(matrixLine);
            }

            // Fecha o arquivo
            inputFile.close();
            return 1;
        };

        int run() {
            std::cout << "Running the LBM simulation" << std::endl;
            return 1;
        };
};

int main(){
    std::string input_dir = "output/state.txt";
    
    // Create a new LBM object
    LBM lbm;

    // Initialize the LBM object
    lbm.init(input_dir);

    // Run the LBM simulation
    if (!lbm.run()) {
        std::cerr << "Error running the LBM simulation" << std::endl;
    }

    return 0;
}	