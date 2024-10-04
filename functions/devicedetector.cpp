#include "devicedetector.h"
#include <QProcess>
#include <QDebug>  // Include QDebug header
#include "functions/log/loggingSystem.hpp"

void DeviceDetector::isDeviceInFastbootMode()
{
    QProcess fastbootProcess;
    Logger log;
    fastbootProcess.setProcessChannelMode(QProcess::MergedChannels);  // Merge stdout and stderr
    fastbootProcess.start("fastboot", QStringList() << "devices");
    fastbootProcess.waitForFinished();

    QString output = fastbootProcess.readAll();

    //log.logMessage(DEBUG, "Fastboot devices output: " + output.toStdString());
    // This bruh shit just spams the log file, it can make over 1gb file in 1 hours

    bool connected = !output.isEmpty();
    emit deviceConnected(connected);
}
