#include "adbI.h"
#include <QDebug>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>
#include <chrono>
#include <functional>
#include <QCoreApplication>

adbI::adbI(QObject *parent) : QObject(parent), result(false), m_loopFinished(false) {
    listenThread = std::thread(&adbI::listen, this);
}

adbI::~adbI() {
    if (listenThread.joinable()) {
        listenThread.join();
    }
}

void adbI::listen() {
    while (true) {
        std::unique_lock<std::mutex> lock(mtx);
        cv.wait_for(lock, std::chrono::seconds(1), [this] { return m_loopFinished; });
        if (m_loopFinished) {
            break;
        }
        loop();
    }
}

void adbI::loop() {
    if (isConnected()) {
        qDebug() << "Connected!";
        emit deviceConnected(true);
        std::unique_lock<std::mutex> lock(mtx);
        result = true;
        m_loopFinished = true;
        cv.notify_one();
    } else {
        qDebug() << "Not connected yet...";
    }
}

bool adbI::copy(std::string filename, std::string dest) {
    std::string command = "adb push " + filename + " " + dest;
    return executeCommand(command);
}

bool adbI::twrp(std::string cmd) {
    std::string command = "adb shell twrp " + cmd;
    return executeCommand(command);
}

bool adbI::flash(std::string file) {
    std::string command = "adb shell twrp flash " + file;
    return executeCommand(command);
}

bool adbI::isConnected() {
    std::string command = "adb devices | grep -w recovery";
    return executeCommand(command);
}

bool adbI::executeCommand(const std::string& command) {
    qDebug() << "Executing command:" << QString::fromStdString(command);
    int result = std::system(command.c_str());
    if (result != 0) {
        qDebug() << "Command failed with result:" << result;
        return false;
    }
    return true;
}
