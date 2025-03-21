#ifndef SHELL_H
#define SHELL_H

#include "CommandProcessor.h"
#include <iostream>
#include <vector>
#include <string>


class Shell {
private:
    bool running; 
    CommandProcessor cmdProcessor;

public:
    Shell();
    void start();  
    void stop(); 
    void handleCommand(const std::string& input); 
};

#endif