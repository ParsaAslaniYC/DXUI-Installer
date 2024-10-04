#include "loggingSystem.hpp"
#include "functions/system.h"
#include <fstream>
#include <chrono>
#include <iomanip>
#include <sstream>
#include <iostream>
#include <QStringList>
#include <filesystem>
#include <mutex>

Logger::Logger() : logFileName("system-" + Logger::getCurrentOnlyDate() + ".log"), log_v(false), previousMsg("") {}

std::string Logger::getCurrentDate() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %X");
    return ss.str();
}

std::string Logger::getCurrentOnlyDate() const {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

void Logger::setVariables(bool verbose) {
    log_v = true;
}

bool Logger::isSpam(const std::string& message) {
    if (Logger::previousMsg == message) {
        return true;
    } else {
        return false;
    }
}

bool Logger::logMessage(LogLevel level, const std::string& message) {
    std::string logLevelStr;
    switch (level) {
    case DEBUG: logLevelStr = "DEBUG"; break;
    case INFO: logLevelStr = "INFO"; break;
    case WARNING: logLevelStr = "WARNING"; break;
    case ERROR: logLevelStr = "ERROR"; break;
    }

    // Check if the message is empty
    if (message.empty()) {
        std::cerr << "Error: Empty message cannot be logged." << std::endl;
        return false;
    }

    std::cout << getCurrentDate() << " | " << logLevelStr << " : " << message << std::endl;
    std::ofstream logFile;
    std::mutex fileMutex;

    // Ensure the log file exists
    if (!std::filesystem::exists(logFileName)) {
        std::ofstream tempFile(logFileName);
        if (!tempFile) {
            std::cerr << "Unable to create log file: " << logFileName << std::endl;
            return false;
        }
        tempFile.close();
    }

    // Open the log file in append mode
    logFile.open(logFileName, std::ios_base::app);
    if (!logFile.is_open()) {
        std::cerr << "Unable to open log file: " << logFileName << std::endl;
        return false;
    }

    // Write to log file
    std::lock_guard<std::mutex> lock(fileMutex);
    logFile << getCurrentDate() << " | " << logLevelStr << " : " << message << std::endl;
    logFile.close();
    return true;
}
