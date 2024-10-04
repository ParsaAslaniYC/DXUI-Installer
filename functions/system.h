#ifndef SYSTEM_H
#define SYSTEM_H

#include <QString>

class System {
public:
    // Function to run a command and capture its output
    static QString runCmd(const QString& command, const QStringList& arguments);
};

#endif // SYSTEM_H
