#include "jsonparse.h"
#include <fstream>
#include <stdexcept>

JsonParse::JsonParse(const std::string& filepath) {
    std::ifstream file(filepath);
    if (!file.is_open()) {
        throw std::runtime_error("Could not open file: " + filepath);
    }
    file >> jsonData;
}

std::string JsonParse::find(const std::string& key) {
    // Check if the top-level "response" key exists and is an array
    if (jsonData.contains("response") && jsonData["response"].is_array() && !jsonData["response"].empty()) {
        // Access the first element of the array
        auto& responseArray = jsonData["response"];
        if (responseArray[0].contains(key)) {
            return responseArray[0][key].get<std::string>();
        }
    }
    return "";
}
