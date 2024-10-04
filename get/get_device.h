#ifndef GET_DEVICE_H
#define GET_DEVICE_H

#include <QString>
#include <QObject>
#include <QThread>
#include "functions/devicedetector.h"

class GetDevice : public QObject
{
    Q_OBJECT

public:
    GetDevice(QObject *parent = nullptr);
    ~GetDevice();
    QString getDeviceCodename();  // Ensure this method is public

signals:
    void startDetection();  // Declare startDetection as a signal
    void fastbootModeChecked(bool isInFastbootMode);
    void deviceConnected(bool connected);

private:
    QThread workerThread;
    DeviceDetector *detector;

private slots:
    bool isDeviceInFastbootMode();
};

#endif // GET_DEVICE_H
