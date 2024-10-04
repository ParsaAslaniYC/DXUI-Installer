#include "OptionsDialog.hpp"
#include "functions/getFiles.h"
#include "functions/log/loggingSystem.hpp"
#include "functions/DataShop/DataShop.hpp"
#include "functions/IO_File.h"
#include <QFile>
#include <QMessageBox>
#include <QVBoxLayout>
#include <QWidget>
#include <QString>
#include <QCheckBox>
#include <QUiLoader>
#include <QApplication>
#include <QPushButton>
#include <QDialog>


void OptionsDialog::m_exit() {
    exit_trigger = true;
    dialog->hide();
}

void OptionsDialog::m_hide() {
    dialog->hide();
}

void OptionsDialog::perfG() {
    if (g && g->isChecked()) {
        v->setChecked(false);
    }
}

void OptionsDialog::perfV() {
    if (v && v->isChecked()) {
        g->setChecked(false);
    }
}

OptionsDialog::OptionsDialog(QWidget *parent) : QDialog(parent), v(nullptr), g(nullptr), yesButton(nullptr), noButton(nullptr) {
    // Load the UI file
    Store store;
    IOFile io;
    Logger log;
    GetFile down;
    QFile file(":/assets/UserInterface/installation.ui");
    QUiLoader loader;
    file.open(QFile::ReadOnly);
    dialog = qobject_cast<QDialog*>(loader.load(&file));
    file.close();
    dialog->setWindowTitle("Installation");

    QString codename = QString::fromStdString(store.get("main.codename"));
    log.logMessage(DEBUG, "DataShop returned: " + codename.toStdString());
    down.downloadFile(QString("https://raw.githubusercontent.com/DroidX-UI-Devices/vendor_droidxOTA/refs/heads/14/builds/gapps/%1.json").arg(codename), "cache/gapps.json");
    down.downloadFile(QString("https://raw.githubusercontent.com/DroidX-UI-Devices/vendor_droidxOTA/refs/heads/14/builds/vanilla/%1.json").arg(codename), "cache/vanilla.json");
    v = dialog->findChild<QCheckBox*>("vanilla");
    g = dialog->findChild<QCheckBox*>("gapps");

    std::string ga = io.readFile("cache/gapps.json").toStdString();
    std::string va = io.readFile("cache/vanilla.json").toStdString();

    if (ga == "404: Not Found") {
        QString gap = g->text();
        g->setText(gap + " | Disabled because there is no gapps build for this device.");
        g->setEnabled(false);
    } else if (va == "404: Not Found") {
        QString van = v->text();
        v->setText(van + " | Disabled because there is no vanilla build for this device.");
        v->setEnabled(false);
    } else {
        log.logMessage(WARNING, "Thats fucking crazy theres no build available for this device bruh");
        QMessageBox::critical(nullptr, "Not Available", "There's no builds available for your device.\nPlease contact your device maintainer on Telegram.");
        store.save("check.nobuilds", "sadly");
        exit(0);
    }
    yesButton = dialog->findChild<QPushButton*>("go_ahead");
    noButton = dialog->findChild<QPushButton*>("SAD");

    if (yesButton) {
        QObject::connect(yesButton, &QPushButton::clicked, this, &OptionsDialog::m_hide);
    } else {
        qWarning() << "Could not find the 'yesButton' in the UI file.";
    }

    if (noButton) {
        QObject::connect(noButton, &QPushButton::clicked, this, &OptionsDialog::m_exit);
        noButton->hide();
    } else {
        qWarning() << "Could not find the 'noButton' in the UI file.";
    }

    connect(v, &QCheckBox::stateChanged, this, &OptionsDialog::perfV);
    connect(g, &QCheckBox::stateChanged, this, &OptionsDialog::perfG);
}

QString OptionsDialog::collect() {


    if (exit_trigger) {
        Store store;
        store.save("exit.triggered", "Yes");
    }

    if (v && v->isChecked()) {
        return "vanilla";
    } else if (g && g->isChecked()) {
        return "gapps";
    } else {
        QMessageBox::warning(nullptr, "Warning", "Please select one variant before continuing");
        return "";
    }
}

void OptionsDialog::show_dialog() {
    dialog->exec();
}

