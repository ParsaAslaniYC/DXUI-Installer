#ifndef PARSEREDIRECTS_H
#define PARSEREDIRECTS_H

#include <string>
#include <nlohmann/json.hpp>

class ParseRedirects {
public:
    // Constructor
    ParseRedirects();

    // Function to find the value associated with a specific codename
    std::string findRedirect(const std::string& codename);

private:
    nlohmann::json jsonData;
};

#endif // PARSEREDIRECTS_H
