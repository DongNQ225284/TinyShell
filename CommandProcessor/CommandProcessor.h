#ifndef COMMAND_PROCESS_H
#define COMMAND_PROCESS_H

#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include "FileManager.h"
#include "ProcessManager.h"

class CommandProcess {
public:
    static void executeCommand(const std::string& input);
    
private:
    static void parseCommand(const std::string& input, std::vector<std::string>& args);
};

#endif
