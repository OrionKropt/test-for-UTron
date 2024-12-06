#pragma once

#include <QString>
#include <QFile>

class Logger{
public:
    void logMessage(const QString &message);
    static Logger* instance();
    ~Logger();
private:
    Logger();
    QFile logFile;
};


