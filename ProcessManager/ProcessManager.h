#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

class ProcessManager {
private:
    std::vector<PROCESS_INFORMATION> processes;
public:
    bool runProcess(const std::string& command, bool isBackground); 
    void listProcesses(); 
    bool killProcess(DWORD pid); 
    void killAll();
};

#endif