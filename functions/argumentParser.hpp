#ifndef ARGUMENTPARSER_HPP
#define ARGUMENTPARSER_HPP

#include <QCoreApplication>
#include <QCommandLineParser>
#include <QDebug>
#include "functions/log/loggingSystem.hpp"

class ArgumentParser
{
public:
    ArgumentParser(QCoreApplication &app);
    void parseArgs();
    bool isDeveloperMode() const;

private:
    QCommandLineParser parser;
    QCommandLineOption developerOption;
    bool developerMode;
};

#endif // ARGUMENTPARSER_HPP
