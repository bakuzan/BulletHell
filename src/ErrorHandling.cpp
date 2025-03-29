#include <iostream>
#include <fstream>
#include <csignal>
#include <cstdlib>
#include <ctime>

#include "ErrorHandling.h"

void LogCrashToFile(const std::string &errorMessage)
{
    std::ofstream errorLog("./logs/crash_log.txt", std::ios::app);

    if (errorLog.is_open())
    {
        // Timestamp for better error tracking
        std::time_t now = std::time(nullptr);
        errorLog << "Timestamp: " << std::ctime(&now);
        errorLog << "Critical Error: " << errorMessage << "\n"
                 << std::endl;
        errorLog.close();
    }
    else
    {
        std::cerr << "Unable to open crash log file!" << std::endl;
    }
}

void SignalHandler(int signal)
{
    std::string errorMessage;

    switch (signal)
    {
    case SIGSEGV:
        errorMessage = "Segmentation Fault (SIGSEGV)";
        break;
    case SIGABRT:
        errorMessage = "Aborted (SIGABRT)";
        break;
    case SIGFPE:
        errorMessage = "Floating Point Exception (SIGFPE)";
        break;
    case SIGILL:
        errorMessage = "Illegal Instruction (SIGILL)";
        break;
    default:
        errorMessage = "Unknown Signal";
        break;
    }

    LogCrashToFile(errorMessage);
    std::cerr << "A critical error has occurred: " << errorMessage << std::endl;

    // Ensure the program exits after handling the signal
    std::exit(EXIT_FAILURE);
}

void RegisterSignalHandlers()
{
    std::signal(SIGSEGV, SignalHandler);
    std::signal(SIGABRT, SignalHandler);
    std::signal(SIGFPE, SignalHandler);
    std::signal(SIGILL, SignalHandler);
}