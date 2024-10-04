#include "Phone.hpp"
#include "functions/system.h"
#include <QStringList>
#include <QString>

void Phone::rebootRecovery() {
    System sys;
    QStringList arg;
    arg << "reboot" << "recovery";
    sys.runCmd("fastboot", arg);
}
