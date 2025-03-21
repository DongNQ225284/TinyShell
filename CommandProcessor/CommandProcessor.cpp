#include "CommandProcessor.h"

using namespace std;

void CommandProcess::parseCommand(const string& input, vector<string>& args) {
    istringstream iss(input);
    string word;
    while (iss >> word) {
        args.push_back(word);
    }
}

void CommandProcess::executeCommand(const string& input) {
    vector<string> args;
    parseCommand(input, args);

    if (args.empty()) return; // Không có lệnh

    string command = args[0];

    if (command == "exit") {

    } 
    else if (command == "help") {
 
    } 
    else if (command == "ls") {
    
    } 
    else if (command == "mkdir" && args.size() > 1) {
    
    } 
    else if (command == "rmdir" && args.size() > 1) {
    
    } 
    else if (command == "cp" && args.size() > 2) {
    
    } 
    else if (command == "mv" && args.size() > 2) {
    
    } 
    else if (command == "rm" && args.size() > 1) {
    
    } 
    else if (command == "pwd") {
    
    } 
    else if (command == "cd" && args.size() > 1) {

    } 
    else if (command == "run" && args.size() > 1) {

    } 
    else {
        cout << "Command not found: " << command << endl;
    }
}
