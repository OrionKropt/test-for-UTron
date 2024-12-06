#include "logger.h"
#include<QDateTime>
#include <QTextBlock>

Logger::Logger() {
    logFile.setFileName("modbus_log.txt");
    logFile.open(QIODevice::Append | QIODevice::Text);
}

Logger* Logger::instance() {
    static Logger inst;
    return &inst;
}

void Logger::logMessage(const QString &message) {
    QTextStream logStream(&logFile);
    logStream << QDateTime::currentDateTime().toString("yyyy-MM-dd HH:mm:ss") << " - " << message << "\n";
    logFile.flush();
}

Logger::~Logger(){
    logFile.close();
}
