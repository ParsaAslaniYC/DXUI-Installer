#ifndef DEVNOTES_H
#define DEVNOTES_H

#include <QString>

class DevNotes {
public:
    static QString showDialog();

private:
    static const QString uiFilePath;
};

#endif // DEVNOTES_H
