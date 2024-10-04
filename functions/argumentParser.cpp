#include "argumentParser.hpp"

ArgumentParser::ArgumentParser(QCoreApplication &app)
    : developerOption(QStringList() << "p" << "pro-mode",
                  QCoreApplication::translate("main", "Enable professional mode.")),
    developerMode(false)
{
    parser.setApplicationDescription("DroidX-UI Installer");
    parser.addHelpOption();
    parser.addVersionOption();
    parser.addOption(developerOption);
    parser.process(app);
}

void ArgumentParser::parseArgs()
{
    Logger log;
    developerMode = parser.isSet(developerOption);

    if (developerMode) {
        log.logMessage(DEBUG, "Professional mode is enabled.");
    } else {
        log.logMessage(DEBUG, "Professional mode is disabled.");
    }
}

bool ArgumentParser::isDeveloperMode() const
{
    return developerMode;
}
