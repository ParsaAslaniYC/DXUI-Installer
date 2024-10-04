#ifndef DOWNLOAD_HPP
#define DOWNLOAD_HPP

#include <QObject>
#include <QFile>
#include <QProgressBar>
#include <QLabel>
#include <QPushButton>
#include <QUrl>
#include <QWidget>
#include <QVBoxLayout>
#include <QApplication>
#include <QDialog>
#include <QFile>
#include <QDir>
#include <QDebug>
#include <QUiLoader>
#include <QFile>
#include <string>
#include <curl/curl.h>
#include "functions/log/loggingSystem.hpp"

class Download : public QObject {
    Q_OBJECT

public:
    Download(QWidget* parent = nullptr);
    ~Download();

    void download(const QString& url, const QString& outputFilePath);

private slots:
    void onCancelClicked();
    void onDownloadFinished();
    void updateProgress();

private:
    QProgressBar* m_progressBar;
    QLabel* m_statusLabel;
    QPushButton* m_cancelButton;
    QDialog* m_dialog;
    QString m_outputFilePath;
    bool m_isCancelled;
    Logger log;
    CURL* m_curl;
    FILE* m_file;
    qint64 m_bytesReceived;
    qint64 m_bytesTotal;
    QTimer* m_progressTimer;

    static size_t writeCallback(void* contents, size_t size, size_t nmemb, void* userp);
    static int progressCallback(void* clientp, double dltotal, double dlnow, double ultotal, double ulnow);

signals:
    void downloadComplete();
};

#endif // DOWNLOAD_HPP
