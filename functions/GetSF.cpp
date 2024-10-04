// GetSF.cpp
#include "GetSF.h"
#include <iostream>
#include <curl/curl.h>
#include <QThread>

GetSF::GetSF(QObject *parent) : QObject(parent) {}

void GetSF::download(const std::string& url, const std::string& outputFile) {
    if (performDownload(url, outputFile)) {
        emit downloadComplete();
    } else {
        emit downloadFailed("Download failed");
    }
}

bool GetSF::performDownload(const std::string& url, const std::string& outputFile) {
    CURL* curl;
    FILE* fp;
    CURLcode res;

    // Initialize libcurl
    curl = curl_easy_init();
    if (!curl) {
        std::cerr << "Failed to initialize libcurl" << std::endl;
        return false;
    }

    // Open the output file
    fp = fopen(outputFile.c_str(), "wb");
    if (!fp) {
        std::cerr << "Failed to open output file: " << outputFile << std::endl;
        curl_easy_cleanup(curl);
        return false;
    }

    // Set the URL to download
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());

    // Enable following redirects
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    // Set the output file
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);

    // Perform the download
    res = curl_easy_perform(curl);

    // Clean up
    fclose(fp);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK) {
        std::cerr << "Download failed: " << curl_easy_strerror(res) << std::endl;
        return false;
    }

    return true;
}
