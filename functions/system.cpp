#include "system.h"
#include <QProcess>
#include <QDebug>
#include "log/loggingSystem.hpp"

QString System::runCmd(const QString& command, const QStringList& arguments) {
    QProcess process;
    Logger log;
    process.start(command, arguments);

    if (!process.waitForStarted()) {
        log.logMessage(ERROR, "Failed to start command: " + command.toStdString());
        return "";
    }

    if (!process.waitForFinished()) {
        log.logMessage(WARNING, "Timeout reached for command: " + command.toStdString());
        return "";
    }

    QByteArray output = process.readAllStandardOutput();
    QByteArray errorOutput = process.readAllStandardError();

    if (!errorOutput.isEmpty()) {
        return QString::fromUtf8(errorOutput);
    }

    return QString::fromUtf8(output);
}
