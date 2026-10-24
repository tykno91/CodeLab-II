#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <random>
#include <algorithm>

// Structure to hold a single joke's setup and punchline
struct Joke {
    std::string setup;
    std::string punchline;
};

// Reads jokes from file and parses them using '?' as the separator
std::vector<Joke> loadJokes(const std::string& filename) {
    std::vector<Joke> jokes;
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cerr << "Error: Could not open " << filename << "\n";
        return jokes;
    }

    std::string line;
    while (std::getline(file, line)) {
        if (line.empty()) continue;
        size_t qmarkPos = line.find('?');
        if (qmarkPos != std::string::npos) {
            jokes.push_back({
                line.substr(0, qmarkPos + 1),       // Setup including '?'
                line.substr(qmarkPos + 1)           // Punchline
            });
        }
    }
    return jokes;
}

// Converts a string to lowercase for case-insensitive comparison
std::string toLower(std::string text) {
    std::transform(text.begin(), text.end(), text.begin(), [](unsigned char c) {
        return std::tolower(c);
    });
    return text;
}

int main() {
    std::vector<Joke> jokes = loadJokes("randomJokes.txt");
    if (jokes.empty()) {
        std::cerr << "No jokes found or file failed to load.\n";
        return 1;
    }

    // Set up modern C++ random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<size_t> dist(0, jokes.size() - 1);

    std::cout << "====================================================\n";
    std::cout << "          Alexa Joke Teller Machine                 \n";
    std::cout << " Type: 'Alexa, tell me a joke' or 'quit' to exit    \n";
    std::cout << "====================================================\n";

    std::string userInput;
    while (true) {
        std::cout << "\nYou: ";
        std::getline(std::cin, userInput);

        std::string cleanedInput = toLower(userInput);

        if (cleanedInput == "quit") {
            std::cout << "Alexa: Goodbye!\n";
            break;
        } else if (cleanedInput == "alexa, tell me a joke" || cleanedInput == "alexa tell me a joke") {
            const Joke& selectedJoke = jokes[dist(gen)];

            std::cout << "Alexa: " << selectedJoke.setup << "\n";
            std::cout << "[Press Enter to see the punchline...]";
            std::cin.get();

            std::cout << "Alexa: " << selectedJoke.punchline << "\n";
        } else {
            std::cout << "Alexa: Sorry, I only understand 'Alexa, tell me a joke' or 'quit'.\n";
        }
    }

    return 0;
}