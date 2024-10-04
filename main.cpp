#include <dialog/Splash.h>
#include <QApplication>
#include <QtUiTools/QUiLoader>
#include <QFile>
#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QTimer>
#include <QDebug>
#include <QProgressBar>
#include "get/get_device.h"
#include "get/get_codename.h"
#include "dialog/devNotes.h"
#include "QMessageBox"
#include "functions/system.h"
#include <string>
#include "functions/getFiles.h"
#include "functions/parseDevices.h"
#include <iostream>
#include "functions/parseRedirects.h"
#include "functions/getFriendly.h"
#include <QStringList>
#include "functions/argumentParser.hpp"
#include "functions/log/loggingSystem.hpp"
#include "functions/DataShop/DataShop.hpp"
#include "dialog/OptionsDialog.hpp"
#include "dialog/download.hpp"
#include "functions/jsonparse.h"
#include "nlohmann/json.hpp"
#include "functions/GetSF.h"
#include "functions/interface/RecoveryInterface.hpp"
#include <QTimer>

void onExit() {
    System syst;
    QStringList ar1;
    ar1 << "-rf" << "cache";
    syst.runCmd("rm", ar1);
    Logger log;
    log.logMessage(INFO, "Exiting...");
    exit(0);
}

int onInstall() {
    Store store;
    Logger log;
    GetSF get;
    QThread thread;
    OptionsDialog odg;
    QTimer *timer = new QTimer();
    QString dig = DevNotes::showDialog();
    RI rc;
    if (dig == "Yes") {
        odg.show_dialog();
        store.save("main.oninstall", "true");
        store.save("status.text", "dr");
        std::string codename = store.get("main.codename");
        JsonParse json_io("cache/" + odg.collect().toStdString() + ".json");
        std::string result = json_io.find("download");
        log.logMessage(DEBUG, "Download link: " + result);
        QString urlString = QString::fromStdString(result);
        QString outputFilePath = "cache/droidx_" + QString::fromStdString(codename) + "_" + odg.collect() + ".zip";
        store.save("install.cleanname", "droidx_" + codename + "_" + odg.collect().toStdString() + ".zip");
        store.save("install.filename", outputFilePath.toStdString());
        get.moveToThread(&thread);
        QObject::connect(&thread, &QThread::started, [&get, &urlString, &outputFilePath]() {
            get.download(urlString.toStdString(), outputFilePath.toStdString());
        });
        QObject::connect(&get, &GetSF::downloadComplete, [&store, &rc, &timer](){
            store.save("status.text", "fos");
            QObject::connect(timer, &QTimer::timeout, [&store, &rc]() {
                rc.sideload(store.get("install.filename"), store.get("install.cleanname"));
                rc.format_data();
            });
            timer->start(80000);
            store.save("status.text", "finished");
        });
        QObject::connect(&get, &GetSF::downloadFailed, [&store]() {
            store.save("status.text", "df");
            return 1;
        });
        thread.start();
        thread.wait();
    } else {

    }
}

void errorUnofficial() {
    QMessageBox::critical(nullptr, "Error", "Your device isnt connected or its not officially supported.\nPlease restart the program if your phone was disconnected.");
}

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    Logger log;
    log.logMessage(INFO, "Initializing...");

    // Parse arguments
    ArgumentParser args(app);
    args.parseArgs();

    // Load the UI file
    QUiLoader loader;
    QFile file(":/assets/UserInterface/main.ui");
    file.open(QFile::ReadOnly);
    SplashScreen splsh;
    // Create the main window from the UI file
    QWidget *mainWindow = loader.load(&file);
    file.close();
    bool isDisconnected = false;
    GetDevice device;
    System sys;
    GetFile down;
    Store store;
    std::string installation_file;
    GetFriendly getFriendly;
    QLabel *deviceLabel = mainWindow->findChild<QLabel*>("device");
    QProgressBar *pbar = mainWindow->findChild<QProgressBar*>("pbar");
    QPushButton *install = mainWindow->findChild<QPushButton*>("install");
    QPushButton *Pro = mainWindow->findChild<QPushButton*>("offline");
    if (args.isDeveloperMode()) {
        Pro->setEnabled(true);
    } else {
        Pro->setEnabled(false);
    }
    QString productName;
    pbar->setValue(0);


    // Connect the deviceConnected signal to a slot
    QObject::connect(&device, &GetDevice::deviceConnected, [deviceLabel, &isDisconnected, &pbar, &getFriendly, &productName, &sys](bool connected) {
        if (connected) {
            if (deviceLabel) {
                QStringList ar2;
                Store store;
                ar2 << "getvar" << "product";
                QString out = sys.runCmd("fastboot", ar2);
                productName = getFriendly.extractProductName(out);
                if (store.get("status.text") == "dr") {
                    deviceLabel->setText("Downloading the Operating System...");
                    pbar->setValue(25);
                } else if (store.get("status.text") == "cf") {
                    deviceLabel->setText("Flashing the Operating System...");
                    pbar->setValue(55);
                } else if (store.get("status.text") == "df") {
                    deviceLabel->setText("Download failed.");
                    pbar->setValue(100);
                } else {
                    deviceLabel->setText("Device connected, codename: " + productName);
                    isDisconnected = true;
                    pbar->setValue(7);
                }
            }
        } else {
            Store store;
            if (deviceLabel) {
                if (isDisconnected) {
                    if (store.get("main.oninstall") == "true") {
                        deviceLabel->setText("Device is rebooted to recovery...");
                        pbar->setValue(16);
                    } else {
                        deviceLabel->setText("Device is disconnected or rebooted, or you have a bad cable. Please restart the installer");
                        pbar->setValue(0);
                    }
                } else {
                    deviceLabel->setText("No device in fastboot mode found. Connect your phone then restart the installer.");
                }
            }
        }
    });
    //QString state = splsh.showSplash();
    //if (state != "Everything is fine you can continue") {
    //    QMessageBox::critical(nullptr, "Error", "Critical Error Happened, report this to our team.\nCode: ED_nGROK");
    //    QStringList ar1;
    //    ar1 << "-rf" << "cache";
    //    sys.runCmd("rm", ar1);
    //    exit(0);
    //} else {
    //    qDebug() << "Fuck man, How the fuck this survived";
    //}
    QPushButton *exit = mainWindow->findChild<QPushButton*>("exit");
    QObject::connect(exit, &QPushButton::clicked, onExit);
    QStringList ar3;
    ar3 << "getvar" << "product";
    QString out = sys.runCmd("fastboot", ar3);
    log.logMessage(DEBUG, "fastboot getvar profucked output: " + out.toStdString());
    productName = getFriendly.extractProductName(out);
    QStringList ar4;
    ar4 << "cache";
    sys.runCmd("mkdir", ar4);
    QStringList ar5;
    ar5 << "cache/global_caches.txt";
    sys.runCmd("touch", ar5);
    log.logMessage(INFO, "Downloading device json file.");
    down.downloadFile("https://raw.githubusercontent.com/DroidX-UI-Devices/vendor_droidxOTA/14/devices.json", "cache/devices.json");
    log.logMessage(INFO, "Done.");
    ParseDevices parser("cache/devices.json");
    bool official = parser.find(productName);
    //std::cout << "Codename: " << productName << endl;
    if (official) {
        QObject::connect(install, &QPushButton::clicked, onInstall);
        log.logMessage(INFO, "Downloading device developer notes file");
        down.downloadFile("https://raw.githubusercontent.com/DroidX-UI-Devices/vendor_droidxOTA/14/changelogs/" + productName + ".txt", "cache/devnotes.txt");
        store.save("main.codename", productName.toStdString());
        log.logMessage(INFO, "Done.");
    } else {
        ParseRedirects parser_off;
        std::string redirectCodename = parser_off.findRedirect(productName.toStdString());

        bool official_check = parser.find(QString::fromStdString(redirectCodename));
        //std::cout << "Hello, World!" << redirectCodename << std::endl; Debug

        if (official_check) {
            QObject::connect(install, &QPushButton::clicked, onInstall);
            log.logMessage(INFO, "Downloading device developer notes file");
            down.downloadFile(QString::fromStdString("https://raw.githubusercontent.com/DroidX-UI-Devices/vendor_droidxOTA/14/changelogs/") + QString::fromStdString(redirectCodename) + ".txt", "cache/devnotes.txt");
            store.save("main.codename", redirectCodename);
            log.logMessage(INFO, "Done.");
        } else {
            QObject::connect(install, &QPushButton::clicked, errorUnofficial);
        }
    }
    log.logMessage(INFO, "All of downloads are done.");
    // Use a timer to periodically check the device status
    log.logMessage(INFO, "Start the device checking loop");
    QTimer *timer = new QTimer();
    QObject::connect(timer, &QTimer::timeout, [&device]() {
        device.startDetection();
    });
    timer->start(1000);  // Check every 1 second

    // Show the main window
    log.logMessage(INFO, "Initializing done, opening the user interface.");
    mainWindow->show();

    return app.exec();
}
