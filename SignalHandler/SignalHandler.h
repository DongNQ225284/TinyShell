#ifndef SIGNALHANDLER_H
#define SIGNALHANDLER_H

#include <csignal>
#include <iostream>
#include "Shell.h"

class SignalHandler {
public:
    static void registerHandlers(); 
    static void handleSignal(int signal);
};

#endif
