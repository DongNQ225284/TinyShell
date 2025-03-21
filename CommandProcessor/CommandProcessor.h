#ifndef COMMAND_PROCESS_H
#define COMMAND_PROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "FileManager.h"
#include "ProcessManager.h"

class CommandProcessor {
public:
    void executeCommand(const std::string& input);
    
private:
    void parseCommand(const std::string& input, std::vector<std::string>& args);
};

#endif
