#include "Splash.h"
#include <QFile>
#include <QDialog>
#include <QtUiTools/QUiLoader>
#include <QPushButton>
#include <QDebug>
#include "functions/IO_File.h"
#include <QTextEdit>
#include <QCheckBox>
#include <QObject>
#include "functions/getFiles.h"
#include <QProgressBar>
#include <QMessageBox>
#include "functions/system.h"
#include <QStringList>

const QString SplashScreen::uiFilePath = ":/assets/UserInterface/getResources_dialog.ui";



QString SplashScreen::showSplash() {
    // Load the UI file
    QFile file(uiFilePath);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Cannot open the UI file:" << uiFilePath;
        return "FUCK MAN, I COULDNT OPEN UI FILE DAD";
    }

    // Create a UI loader and load the dialog
    QUiLoader loader;
    QDialog *dialog = qobject_cast<QDialog*>(loader.load(&file));
    if (!dialog) {
        qWarning() << "Failed to load the UI file:" << uiFilePath;
        file.close();
        return "THIS IS VERY BAD, COULDNT OPEN UI FILE!!";
    }
    //file.close();

    SplashScreen::InternalJobs(dialog);
    // Execute the dialog
    //int result = dialog->exec();


}


QString SplashScreen::InternalJobs(QDialog *dialog) {
    // Find the progress bar
    QProgressBar *progressy = dialog->findChild<QProgressBar*>("pgbar");
    if (!progressy) {
        qWarning() << "Failed to find progress bar in the UI file:" << uiFilePath;
        return "HELLL THIS IS FUCKING BAD, QUIT NOW";
    }
    progressy->setValue(0);

    // Create the cache directory
    System sys;
    QStringList arg;
    arg << "cache";
    sys.runCmd(QString("mkdir"), arg);

    // Download the file
    GetFile download;
    bool code = download.downloadFile("https://raw.githubusercontent.com/DroidX-UI-Devices/vendor_droidxOTA/14/devices.json", "cache/devices.json");
    if (code) {
        progressy->setValue(100);
        //dialog->hide();
        return "Everything is fine you can continue";
    } else {
        QMessageBox::critical(nullptr, "Error", "Critical Error: can't sync latest devices.");
        return "HELLL THIS IS FUCKING BAD, QUIT NOW";
    }
}
