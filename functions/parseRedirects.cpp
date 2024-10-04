#include "parseRedirects.h"
#include <QFile>
#include <QByteArray>
#include <QDebug>
#include <nlohmann/json.hpp>
#include <iostream>
#include "functions/log/loggingSystem.hpp"

using json = nlohmann::json;

ParseRedirects::ParseRedirects() {
    Logger log;
    // Load the JSON file from the QRC resource
    QFile file(":/assets/codename_redirects.json");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        log.logMessage(ERROR, "Failed to open JSON file from QRC resource");
        //std::cout << "1" << std::endl; Debug
        return;
    }

    QByteArray jsonContent = file.readAll();
    file.close();

    try {
        jsonData = json::parse(jsonContent.toStdString());
    } catch (json::parse_error& e) {
        log.logMessage(ERROR, "JSON parse error: " + std::string(e.what()));
        //std::cout << "2" << std::endl; Debug
    }
}

std::string ParseRedirects::findRedirect(const std::string& codename) {
    Logger log;
    if (jsonData.empty()) {
        log.logMessage(INFO, "JSON data is empty");
        //std::cout << "3" << std::endl; Debug
        return "";
    }
    for (const auto& entry : jsonData) {
        if (entry.contains(codename)) {
            // std::cout << "Device" << entry[codename].get<std::string>() << std::endl; Debug
            return entry[codename].get<std::string>();
        }
    }

    log.logMessage(INFO, "No redirect found for codename: " + codename);
    // std::cout << "4" << codename << "4" << std::endl; Debug
    return "";
}
