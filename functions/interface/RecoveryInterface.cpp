#include <iostream>
#include "RecoveryInterface.hpp"
#include "adbI.h"
#include <QDebug>
#include "functions/log/loggingSystem.hpp"

void onConnected() {
    Logger log;
    log.logMessage(INFO, "Device Connected");
}

bool RI::sideload(std::string filename, std::string clean_name) {
    adbI adb;
    Logger log;
    bool state = adb.copy(filename, "/sdcard");
    if (!state) {
        log.logMessage(ERROR, "Copy function failed");
    }
    bool status = adb.twrp("install /sdcard/" + clean_name);
    if (!status) {
        log.logMessage(ERROR, "Flash command failed, code is in ./functions/interface/RecoveryInterface.cpp:14. if you want to get deeper its in ./functions/interface/adbI.cpp function twrp.");
    }
    return true;
}

void RI::format_data() {
    adbI adb;
    adb.twrp("format data");
}

void RI::waitForDevice() {
    adbI adb;
    Logger log;
    log.logMessage(INFO, "Listening for device connections... ");
    adb.listen();
}
