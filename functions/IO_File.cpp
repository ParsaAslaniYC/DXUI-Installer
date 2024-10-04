#include "IO_File.h"
#include <QFile>
#include <QTextStream>
#include <QDebug>
#include "functions/log/loggingSystem.hpp"

IOFile::IOFile() {
    // Initialize the file and stream
    file.setFileName("");
    stream.setDevice(&file);
}

QString IOFile::readFile(const QString& filePath) {
    Logger log;
    file.setFileName(filePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        log.logMessage(ERROR, "Failed to open file for reading:" + filePath.toStdString());
        return "";
    }

    QString contents = stream.readAll();
    file.close();
    return contents;
}

bool IOFile::writeFile(const QString& filePath, const QString& contents) {
    Logger log;
    file.setFileName(filePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        log.logMessage(ERROR, "Failed to open file for writing:" + filePath.toStdString());
        return false;
    }

    stream << contents;
    file.close();
    return true;
}

void IOFile::close() {
    if (file.isOpen()) {
        file.close();
    }
}
