#ifndef ADBI_H
#define ADBI_H

#include <QObject>
#include <string>
#include <thread>
#include <condition_variable>
#include <mutex>

class adbI : public QObject {
    Q_OBJECT

public:
    explicit adbI(QObject *parent = nullptr);
    ~adbI();

    void listen();
    bool copy(std::string filename, std::string dest);
    bool twrp(std::string cmd);
    bool flash(std::string file);
    bool isConnected();

signals:
    void deviceConnected(bool connected);

private:
    void loop();
    bool executeCommand(const std::string& command);

    std::thread listenThread;
    std::mutex mtx;
    std::condition_variable cv;
    bool result;
    bool m_loopFinished;
};

#endif // ADBI_H
