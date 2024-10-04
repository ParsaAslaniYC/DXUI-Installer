#ifndef JSONPARSE_H
#define JSONPARSE_H

#include <string>
#include "nlohmann/json.hpp"

class JsonParse {
public:
    JsonParse(const std::string& filepath);
    std::string find(const std::string& key);

private:
    nlohmann::json jsonData;
};

#endif // JSONPARSE_H
