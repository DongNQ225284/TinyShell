#include "CommandProcessor.h"
#include <sstream>
void CommandProcessor::parseCommand(const std::string& input, std::vector<std::string>& args) {
    std::istringstream iss(input);
    std::string word;
    while (iss >> word) {
        args.push_back(word);
    }
}

void CommandProcessor::executeCommand(const std::string& input) {
    std::vector<std::string> args;
    parseCommand(input, args);

    if (args.empty()) return; // Không có lệnh

    std::string command = args[0];

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
        std::cout << "Command not found: " << command << std::endl;
    }
}
