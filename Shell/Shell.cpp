#include "Shell.h"
#include <sstream>

Shell::Shell() {
    running = true;
}

void Shell::start() {
    std::string input;
    while (running) {
        std::cout << "tinyShell> ";
        getline(std::cin, input);
        handleCommand(input);
    }
}

void Shell::stop() {
    running = false;
}

void Shell::handleCommand(const std::string& input) {
    if (input.empty()) return; // Nếu nhập rỗng thì bỏ qua
    
    std::vector<std::string> args;
    std::istringstream iss(input);
    std::string word;
    while (iss >> word) {
        args.push_back(word);
    }
    if (args[0] == "exit") {
        stop();
    } else {
        cmdProcessor.executeCommand(input);
    }
}