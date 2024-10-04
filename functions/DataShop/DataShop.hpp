#ifndef DATASHOP_HPP
#define DATASHOP_HPP

#include <string>
#include <map>
#include <stdexcept>
#include <fstream>
#include <sstream>

class Store {
public:
    // Constructor to load data from the file
    Store();

    // Destructor to save data to the file
    ~Store();

    // Save a variable with a key
    void save(const std::string &key, const std::string &value);

    // Get a variable by key
    std::string get(const std::string &key) const;

private:
    std::map<std::string, std::string> data;
    const std::string filePath = "cache/global_caches.txt";

    // Load data from the file
    void loadFromFile();

    // Save data to the file
    void saveToFile() const;
};

#endif // DATASHOP_HPP
