#ifndef RECOVERYINTERFACE_HPP
#define RECOVERYINTERFACE_HPP

#include <iostream>

class RI {
public:
    bool sideload(std::string filename, std::string clean_name);
    void waitForDevice();
    void format_data();
};

#endif // RECOVERYINTERFACE_HPP
