#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include <iostream>
#include <string>
#include <windows.h>

class EnvironmentManager {
public:
    static void printWorkingDirectory();
    static void changeDirectory(const std::string& path);
    static void listEnvironmentVariables();
    static void setEnvironmentVariable(const std::string& name, const std::string& value);
    static void unsetEnvironmentVariable(const std::string& name); 
};

#endif
