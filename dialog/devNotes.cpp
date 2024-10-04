#include "devNotes.h"
#include <QFile>
#include <QDialog>
#include <QtUiTools/QUiLoader>
#include <QPushButton>
#include <QDebug>
#include "functions/IO_File.h"
#include <QTextEdit>
#include <QCheckBox>
#include <QObject>

const QString DevNotes::uiFilePath = ":/assets/UserInterface/devNotes.ui";

void ChickyEvent(QPushButton *const niggabutton) {

    if (!niggabutton->isEnabled()) {
        niggabutton->setDisabled(false);
    } else {
        niggabutton->setDisabled(true);
    }
}

QString DevNotes::showDialog() {
    // Load the UI file
    QFile file(uiFilePath);
    if (!file.open(QFile::ReadOnly)) {
        qWarning() << "Cannot open the UI file:" << uiFilePath;
        return "Error";
    }

    // Create a UI loader and load the dialog
    QUiLoader loader;
    QDialog *dialog = qobject_cast<QDialog*>(loader.load(&file));
    if (!dialog) {
        qWarning() << "Failed to load the UI file:" << uiFilePath;
        file.close();
        return "Error";
    }
    file.close();
    IOFile io;
    QString notes = io.readFile("cache/devnotes.txt");

    // Connect the buttons to the dialog's accept/reject slots
    QPushButton *yesButton = dialog->findChild<QPushButton*>("yes");
    QCheckBox *chicky = dialog->findChild<QCheckBox*>("chicky");
    yesButton->setDisabled(true);
    QPushButton *noButton = dialog->findChild<QPushButton*>("no");
    QTextEdit *note = dialog->findChild<QTextEdit*>("note");
    note->setReadOnly(true);
    note->setText(notes);

    QObject::connect(chicky, &QCheckBox::stateChanged, [yesButton]() {
        ChickyEvent(yesButton);
    });

    if (yesButton) {
        QObject::connect(yesButton, &QPushButton::clicked, dialog, &QDialog::accept);
    } else {
        qWarning() << "Could not find the 'yesButton' in the UI file:" << uiFilePath;
    }

    if (noButton) {
        QObject::connect(noButton, &QPushButton::clicked, dialog, &QDialog::reject);
    } else {
        qWarning() << "Could not find the 'noButton' in the UI file:" << uiFilePath;
    }



    // Execute the dialog and get the result
    int result = dialog->exec();

    // Check the result and return the corresponding message
    if (result == QDialog::Accepted) {
        return "Yes";
    } else if (result == QDialog::Rejected) {
        return "No";
    }

    return "Error";
}

void ChickyEvent(int state, QPushButton niggabutton) {
    if (state != 1) {
        niggabutton.setDisabled(true);
    } else {
        niggabutton.setDisabled(false);
    }
}
