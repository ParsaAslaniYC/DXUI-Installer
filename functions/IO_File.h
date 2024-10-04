#ifndef IO_FILE_H
#define IO_FILE_H

#include <QString>
#include <QFile>
#include <QTextStream>

class IOFile {
public:
    // Constructor
    IOFile();

    // Method to read a file and return its contents as a QString
    QString readFile(const QString& filePath);

    // Method to write contents to a file
    bool writeFile(const QString& filePath, const QString& contents);

    // Method to close the file
    void close();

private:
    QFile file;
    QTextStream stream;
};

#endif // IO_FILE_H
