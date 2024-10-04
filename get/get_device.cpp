#include "get_device.h"
#include <QProcess>
#include <QDebug>  // Include QDebug header
#include <functions/devicedetector.h>

GetDevice::GetDevice(QObject *parent)
    : QObject(parent), workerThread(new QThread), detector(new DeviceDetector)
{
    detector->moveToThread(&workerThread);

    connect(&workerThread, &QThread::finished, detector, &QObject::deleteLater);
    connect(this, &GetDevice::startDetection, detector, &DeviceDetector::isDeviceInFastbootMode);
    connect(detector, &DeviceDetector::deviceConnected, this, &GetDevice::deviceConnected);

    workerThread.start();
}

GetDevice::~GetDevice()
{
    workerThread.quit();
    workerThread.wait();
}

//void GetDevice::startDetection()
//{
//    emit startDetection();
//}

QString GetDevice::getDeviceCodename()
{
    if (!isDeviceInFastbootMode()) {
        qDebug() << "No device in fastboot mode found.";
        return QString();
    }

    QProcess fastbootProcess;
    fastbootProcess.setProcessChannelMode(QProcess::MergedChannels);  // Merge stdout and stderr
    fastbootProcess.start("fastboot", QStringList() << "getvar" << "product");
    fastbootProcess.waitForFinished();

    QString output = fastbootProcess.readAll();

    // Print the raw output for debugging
    qDebug() << "Raw output:" << output;

    // Parse the output to extract the codename
    QStringList lines = output.split("\n");
    QString codename;

    for (const QString &line : lines) {
        qDebug() << "Processing line:" << line;
        if (line.startsWith("product:")) {
            codename = line.mid(line.indexOf(":") + 1).trimmed();
            qDebug() << "Extracted codename:" << codename;
            break;
        }
    }

    if (codename.isEmpty()) {
        qDebug() << "Failed to retrieve device codename.";
    } else {
        qDebug() << "Device Codename:" << codename;
    }

    return codename;
}

bool GetDevice::isDeviceInFastbootMode()
{
    QProcess fastbootProcess;
    fastbootProcess.setProcessChannelMode(QProcess::MergedChannels);  // Merge stdout and stderr
    fastbootProcess.start("fastboot", QStringList() << "devices");
    fastbootProcess.waitForFinished();

    QString output = fastbootProcess.readAll();

    qDebug() << "Fastboot devices output:" << output;

    return !output.isEmpty();
}
