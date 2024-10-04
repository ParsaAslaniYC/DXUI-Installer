#ifndef LOGGINGSYSTEM_HPP
#define LOGGINGSYSTEM_HPP

#include <string>

enum LogLevel {
    DEBUG,
    INFO,
    ERROR,
    WARNING
};

class Logger {
public:
    Logger();
    void setVariables(bool verbose);
    bool logMessage(LogLevel level, const std::string& message);

private:
    std::string getCurrentDate() const;
    std::string getCurrentOnlyDate() const;
    bool isSpam(const std::string& message);

    std::string logFileName;
    bool log_v;
    std::string previousMsg; // Changed to non-const std::string
};

#endif // LOGGINGSYSTEM_HPP
