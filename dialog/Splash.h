#ifndef SPLASH_H
#define SPLASH_H

#include <QDialog>
#include <QString>

class SplashScreen {
public:
    static QString showSplash();

private:
    static const QString uiFilePath;
    static QString InternalJobs(QDialog *dialog);
};

#endif // SPLASH_H
