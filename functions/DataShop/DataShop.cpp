#include "DataShop.hpp"
#include "functions/log/loggingSystem.hpp"

// Constructor to load data from the file
Store::Store() {
    loadFromFile();
}

// Destructor to save data to the file
Store::~Store() {
    saveToFile();
}

// Save a variable with a key
void Store::save(const std::string &key, const std::string &value) {
    data[key] = value;
    saveToFile();
}

// Get a variable by key
std::string Store::get(const std::string &key) const {
    Logger log;
    auto it = data.find(key);
    if (it != data.end()) {
        return it->second;
    }
    log.logMessage(ERROR, "Key not found: " + key);
    return "Not found";
}

// Load data from the file
void Store::loadFromFile() {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        return; // File does not exist or cannot be opened
    }

    std::string line;
    while (std::getline(file, line)) {
        std::istringstream iss(line);
        std::string key, value;
        if (std::getline(iss, key, '=') && std::getline(iss, value)) {
            data[key] = value;
        }
    }
}

// Save data to the file
void Store::saveToFile() const {
    std::ofstream file(filePath);
    if (!file.is_open()) {
        return; // File cannot be opened
    }

    for (const auto &pair : data) {
        file << pair.first << "=" << pair.second << std::endl;
    }
}
