#ifndef ERROR_HANDLING_H
#define ERROR_HANDLING_H

void LogCrashToFile(const std::string &errorMessage);
void SignalHandler(int signal);

void RegisterSignalHandlers();

#endif // ERROR_HANDLING_H
