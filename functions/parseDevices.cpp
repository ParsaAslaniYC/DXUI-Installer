#include "parseDevices.h"
#include <fstream>
#include <iostream>
#include <nlohmann/json.hpp>
#include <functions/log/loggingSystem.hpp>

using json = nlohmann::json;

ParseDevices::ParseDevices(const std::string& jsonFilePath) : jsonFilePath(jsonFilePath) {}

bool ParseDevices::find(const QString& codename) {
    std::string codenameStr = codename.toStdString();
    std::cout << codenameStr << std::endl;
    std::ifstream jsonFile(jsonFilePath);
    Logger log;
    if (!jsonFile.is_open()) {
        //std::cerr << "Failed to open JSON file: " << jsonFilePath << std::endl; Debug
        return false;
    }

    json j;
    jsonFile >> j;
    jsonFile.close();

    //std::cout << "JSON content: " << j.dump(4) << std::endl; Debug statement to print JSON content

    for (const auto& device : j) {
        if (device.contains("codename") && device["codename"] == codenameStr) {
            //std::cout << "Found codename: " << device["codename"] << std::endl; Debug statement to print found codename
            return true;
        }
    }

    log.logMessage(INFO, "No official support for this device: " + codenameStr); //Debug
    return false;
}
