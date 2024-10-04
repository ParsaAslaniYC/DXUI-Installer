#include "getFriendly.h"
#include <QString>
#include <QDebug>
#include <QRegularExpression>
#include <iostream>
#include "functions/log/loggingSystem.hpp"

QString GetFriendly::extractProductName(const QString& input) {
    Logger log;
    // Define a regular expression to match the product name after "product:"
    QRegularExpression re("product:\\s*(\\w+)");
    //log.logMessage(DEBUG, "getFriendly.cpp:12 | Raw Input: " + input.toStdString());
    // Match the regular expression against the input string
    QRegularExpressionMatch match = re.match(input);

    // If a match is found, extract the product name
    if (match.hasMatch()) {
        QString productName = match.captured(1);
        //std::cout << productName.toStdString() << std::endl;
        return productName;
    }

    log.logMessage(INFO, "No product name found in the input.");
    return "";
}
