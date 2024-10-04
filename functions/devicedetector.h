#ifndef DEVICEDETECTOR_H
#define DEVICEDETECTOR_H

#include <QObject>

class DeviceDetector : public QObject
{
    Q_OBJECT

public slots:
    void isDeviceInFastbootMode();

signals:
    void deviceConnected(bool connected);
};

#endif // DEVICEDETECTOR_H
