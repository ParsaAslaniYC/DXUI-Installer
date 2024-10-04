#include "argumentParser.hpp"

ArgumentParser::ArgumentParser(QCoreApplication &app)
    : debugOption(QStringList() << "d" << "debug",
                  QCoreApplication::translate("main", "Enable debug mode.")),
    verboseOption(QStringList() << "v" << "verbose",
                  QCoreApplication::translate("main", "Enable verbose output.")),
    debugMode(false),
    verboseMode(false)
{
    parser.setApplicationDescription("My Qt Application");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(debugOption);
    parser.addOption(verboseOption);
    parser.process(app);
}

void ArgumentParser::parseArgs()
{
    debugMode = parser.isSet(debugOption);
    verboseMode = parser.isSet(verboseOption);

    if (debugMode) {
        qDebug() << "Debug mode is enabled.";
    } else {
        qDebug() << "Debug mode is disabled.";
    }

    if (verboseMode) {
        qDebug() << "Verbose mode is enabled.";
    } else {
        qDebug() << "Verbose mode is disabled.";
    }
}

bool ArgumentParser::isDebugMode() const
{
    return debugMode;
}

bool ArgumentParser::isVerboseMode() const
{
    return verboseMode;
}
