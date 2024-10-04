#ifndef OPTIONSDIALOG_HPP
#define OPTIONSDIALOG_HPP

#include <QDialog>
#include <QCheckBox>
#include <QString>
#include <QDialog>

class OptionsDialog : public QDialog {
    Q_OBJECT

public:
    explicit OptionsDialog(QWidget *parent = nullptr);
    QString collect();
    void show_dialog();

private:
    QCheckBox *v;
    QCheckBox *g;
    QPushButton *yesButton;
    QPushButton *noButton;
    QDialog *dialog;
    void m_hide();
    void m_exit();
    bool exit_trigger = false;
    void perfV();
    void perfG();
};

#endif // OPTIONSDIALOG_HPP
