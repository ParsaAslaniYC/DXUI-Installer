#ifndef GETFILES_H
#define GETFILES_H

#include <string>
#include <QString>

class GetFile {
public:
    // Function to download a file from a URL
    bool downloadFile(const QString& url, const std::string& outputFileName);
};

#endif // GETFILES_H
