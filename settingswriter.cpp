#include "settingswriter.h"
#include <QDebug>

void SettingsWriter::write(QPoint pos, QSize size, QStringList recentFiles, QString engineVoice, double rate,
                           bool useTrayIcon, bool splitMode, QString customFestivalCommand, QString customFestivalCommandArguments)
{
    qDebug() << "Writing user settings...";
    QSettings settings("Omilo-qt5", "Omilo-qt5");
    settings.setValue("MainWindowPosition", pos);
    settings.setValue("MainWindowSize", size);
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("MainWindowVoice", engineVoice);
    settings.setValue("Rate", rate);
    settings.setValue("useTrayIcon", useTrayIcon);
    settings.setValue("SplitMode", splitMode);
    settings.setValue("customFestivalCommand", customFestivalCommand);
    settings.setValue("customFestivalCommandArguments", customFestivalCommandArguments);
    qDebug() << "Writing user settings completed.";
}

