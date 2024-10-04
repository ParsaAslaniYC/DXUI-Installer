#include "download.hpp"
#include <QDebug>
#include <QThread>
#include <QTimer>

Download::Download(QWidget* parent)
    : QObject(parent), m_isCancelled(false), m_curl(nullptr), m_file(nullptr), m_bytesReceived(0), m_bytesTotal(0) {

    // Load the UI file
    QUiLoader loader;
    QFile file(":/assets/UserInterface/getResources_dialog.ui");
    file.open(QFile::ReadOnly);
    m_dialog = qobject_cast<QDialog*>(loader.load(&file, parent));
    file.close();

    // Get the widgets from the UI
    m_progressBar = m_dialog->findChild<QProgressBar*>("pgbar");
    m_statusLabel = m_dialog->findChild<QLabel*>("label");
    m_cancelButton = m_dialog->findChild<QPushButton*>("cancelButton");

    // Connect the cancel button to the onCancelClicked slot
    connect(m_cancelButton, &QPushButton::clicked, this, &Download::onCancelClicked);

    // Initialize logger
    log.logMessage(DEBUG, "Download object created");

    // Initialize progress timer
    m_progressTimer = new QTimer(this);
    connect(m_progressTimer, &QTimer::timeout, this, &Download::updateProgress);
}

Download::~Download() {
    if (m_curl) {
        curl_easy_cleanup(m_curl);
    }
    if (m_file) {
        fclose(m_file);
    }
    log.logMessage(DEBUG, "Download object destroyed");
}

void Download::download(const QString& url, const QString& outputFilePath) {
    m_isCancelled = false;
    m_progressBar->setValue(0);
    m_statusLabel->setText("Downloading...");
    m_outputFilePath = outputFilePath;

    QThread* thread = QThread::create([this, url, outputFilePath]() {
        m_curl = curl_easy_init();
        if (!m_curl) {
            log.logMessage(ERROR, "Failed to initialize libcurl");
            return;
        }

        m_file = fopen(outputFilePath.toStdString().c_str(), "wb");
        if (!m_file) {
            log.logMessage(ERROR, "Failed to open file for writing: " + outputFilePath.toStdString());
            curl_easy_cleanup(m_curl);
            m_curl = nullptr;
            return;
        }

        curl_easy_setopt(m_curl, CURLOPT_URL, url.toStdString().c_str());
        curl_easy_setopt(m_curl, CURLOPT_WRITEFUNCTION, writeCallback);
        curl_easy_setopt(m_curl, CURLOPT_WRITEDATA, m_file);
        curl_easy_setopt(m_curl, CURLOPT_NOPROGRESS, 0L);
        curl_easy_setopt(m_curl, CURLOPT_PROGRESSFUNCTION, progressCallback);
        curl_easy_setopt(m_curl, CURLOPT_PROGRESSDATA, this);
        curl_easy_setopt(m_curl, CURLOPT_FOLLOWLOCATION, 1L); // Follow redirects

        // Start the progress timer
        m_progressTimer->start(100); // Update progress every 100ms

        CURLcode res = curl_easy_perform(m_curl);
        if (res != CURLE_OK) {
            log.logMessage(ERROR, "Download failed: " + std::string(curl_easy_strerror(res)));
            m_statusLabel->setText("Download failed");
        } else {
            log.logMessage(DEBUG, "Download completed");
            m_statusLabel->setText("Download completed");
        }

        fclose(m_file);
        curl_easy_cleanup(m_curl);
        m_curl = nullptr;
        m_file = nullptr;

        // Stop the progress timer
        m_progressTimer->stop();

        emit downloadComplete();
    });

    connect(thread, &QThread::finished, thread, &QThread::deleteLater);
    connect(this, &Download::downloadComplete, this, &Download::onDownloadFinished);

    thread->start();

    // Show the dialog
    m_dialog->show();
}

void Download::onCancelClicked() {
    m_isCancelled = true;
    if (m_curl) {
        curl_easy_cleanup(m_curl);
        m_curl = nullptr;
    }
    if (m_file) {
        fclose(m_file);
        m_file = nullptr;
    }
    m_statusLabel->setText("Download cancelled");
    m_progressTimer->stop();
}

void Download::onDownloadFinished() {
    log.logMessage(DEBUG, "Download finished");
}

void Download::updateProgress() {
    if (m_bytesTotal > 0) {
        m_progressBar->setMaximum(m_bytesTotal);
        m_progressBar->setValue(m_bytesReceived);
    }
}

size_t Download::writeCallback(void* contents, size_t size, size_t nmemb, void* userp) {
    size_t realsize = size * nmemb;
    FILE* file = (FILE*)userp;
    return fwrite(contents, size, nmemb, file);
}

int Download::progressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow) {
    Download* download = static_cast<Download*>(clientp);
    if (download->m_isCancelled) {
        return 1; // Cancel the download
    }

    download->m_bytesReceived = dlnow;
    download->m_bytesTotal = dltotal;

    return 0; // Continue the download
}
