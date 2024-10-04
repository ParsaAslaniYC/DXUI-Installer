#include "getFiles.h"
#include <curl/curl.h>
#include <iostream>
#include <fstream>
#include <QString>
#include <QMessageBox>
#include "functions/log/loggingSystem.hpp"

// Callback function to write data to a file
size_t WriteCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t totalSize = size * nmemb;
    std::ofstream* outFile = static_cast<std::ofstream*>(userp);
    outFile->write(static_cast<char*>(contents), totalSize);
    return totalSize;
}

bool GetFile::downloadFile(const QString& url, const std::string& outputFileName) {
    std::string urlStr = url.toStdString();
    Logger log;
    CURL* curl;
    CURLcode res;
    std::ofstream outFile(outputFileName, std::ios::binary);

    if (!outFile.is_open()) {
        log.logMessage(ERROR, "Failed to open file: " + outputFileName);
        return false;
    }

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, urlStr.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &outFile);

        res = curl_easy_perform(curl);
        if (res != CURLE_OK) {
            log.logMessage(ERROR, "curl_easy_perform() failed: " + std::string(curl_easy_strerror(res)));
            QString error = QString(curl_easy_strerror(res));
            curl_easy_cleanup(curl);
            QMessageBox::critical(nullptr, "Error", "Please check your internet connected and restart the installer\ncurl_easy_perform() failed: " + error);
            outFile.close();
            return false;
        }

        curl_easy_cleanup(curl);
    } else {
        log.logMessage(ERROR, "Failed to initialize libcurl");
        QMessageBox::critical(nullptr, "Critical Fault", "Failed to initialize libcurl");
        outFile.close();
        return false;
    }

    outFile.close();
    return true;
}
