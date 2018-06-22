#ifndef SETTINGSWRITER_H
#define SETTINGSWRITER_H

#include <QPoint>
#include <QSize>
#include <QStringList>
#include <QSettings>

class SettingsWriter
{
public:
    void write(QPoint pos, QSize size, QStringList recentFiles, QString engineVoice, double rate,
                               bool useTrayIcon, bool splitMode, QString customFestivalCommand, QString customFestivalCommandArguments);

};

#endif // SETTINGSWRITER_H
