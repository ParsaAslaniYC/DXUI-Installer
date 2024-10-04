#include "get_codename.h"
#include <QProcess>
#include <QDebug>
#include <QStringList>

QString GetCodename::getDeviceCodename() {
    QProcess process;
    QStringList arguments;
    arguments << "getvar" << "product";

    process.start("fastboot", arguments);
    if (!process.waitForStarted()) {
        qDebug() << "Failed to start fastboot command.";
        return "Failed to start fastboot command.";
    }

    if (!process.waitForFinished()) {
        qDebug() << "Fastboot command failed to finish.";
        return "Fastboot command failed to finish.";
    }

    QByteArray output = process.readAllStandardOutput();
    QString outputString = QString::fromUtf8(output);

    // Parse the output to extract the product name
    QStringList lines = outputString.split('\n');
    for (const QString& line : lines) {
        if (line.contains("product:")) {
            QString productName = line.split(':')[1].trimmed();
            qDebug() << "Device Codename:" << productName;
            return productName;
        }
    }

    qDebug() << "Failed to retrieve device codename.";
    return "Failed to retrieve device codename.";
}
