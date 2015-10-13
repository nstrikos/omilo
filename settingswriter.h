#ifndef SETTINGSWRITER_H
#define SETTINGSWRITER_H

#include <QPoint>
#include <QSize>
#include <QStringList>
#include <QSettings>

class SettingsWriter
{
public:
    void write(QPoint pos, QSize size, QStringList recentFiles, QString engineVoice,
                               bool useTrayIcon, bool splitMode);

};

#endif // SETTINGSWRITER_H
