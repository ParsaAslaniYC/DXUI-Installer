#ifndef PARSEDEVICES_H
#define PARSEDEVICES_H

#include <QString>
#include <string>

class ParseDevices {
public:
    // Constructor that takes the path to the JSON file
    ParseDevices(const std::string& jsonFilePath);

    // Function to find if a specific codename exists in the JSON file
    bool find(const QString& codename);

private:
    std::string jsonFilePath;
};

#endif // PARSEDEVICES_H
