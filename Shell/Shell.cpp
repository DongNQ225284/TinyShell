#include "Shell.h"
#include <sstream>

using namespace std;

Shell::Shell() {
    running = true;
}

void Shell::start() {
    string input;
    while (running) {
        cout << "tinyShell> ";
        getline(cin, input);
        handleCommand(input);
    }
}

void Shell::stop() {
    running = false;
}

void Shell::handleCommand(const string& input) {
    if (input.empty()) return; // Nếu nhập rỗng thì bỏ qua
    
    vector<string> args;
    istringstream iss(input);
    string word;
    while (iss >> word) {
        args.push_back(word);
    }

    if (args[0] == "exit") {
        stop();
    } else {
        cmdProcessor.executeCommand(args);
    }
}
