// GetSF.h
#ifndef GETSF_H
#define GETSF_H

#include <string>
#include <QObject>

class GetSF : public QObject {
    Q_OBJECT

public:
    explicit GetSF(QObject *parent = nullptr);
    void download(const std::string& url, const std::string& outputFile);

signals:
    void downloadComplete();
    void downloadFailed(const std::string& errorMessage);

private:
    bool performDownload(const std::string& url, const std::string& outputFile);
};

#endif // GETSF_H
