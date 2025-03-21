#ifndef PROCESS_MANAGER_H
#define PROCESS_MANAGER_H

#include <windows.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

class ProcessManager {
private:
    vector<PROCESS_INFORMATION> processes;
public:
    bool runProcess(const string& command, bool isBackground); 
    void listProcesses(); 
    bool killProcess(DWORD pid); 
    void killAll();
};

#endif