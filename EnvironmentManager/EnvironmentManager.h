#ifndef ENVIRONMENT_MANAGER_H
#define ENVIRONMENT_MANAGER_H

#include <iostream>
#include <string>
#include <windows.h>

using namespace std;

class EnvironmentManager {
public:
    static void printWorkingDirectory();
    static void changeDirectory(const string& path);
    static void listEnvironmentVariables();
    static void setEnvironmentVariable(const string& name, const string& value);
    static void unsetEnvironmentVariable(const string& name); 
};

#endif
