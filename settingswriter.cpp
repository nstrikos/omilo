#include "settingswriter.h"
#include <QFont>
#include <QDebug>

void SettingsWriter::write(QPoint pos, QSize size, QStringList recentFiles, QString engineVoice, double rate,
                           bool useTrayIcon, bool splitMode, bool useClipboard, QString customFestivalCommand, QString customFestivalCommandArguments,
                           QFont appFont, QFont docFont)
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
    settings.setValue("useClipboard", useClipboard);
    settings.setValue("customFestivalCommand", customFestivalCommand);
    settings.setValue("customFestivalCommandArguments", customFestivalCommandArguments);
#ifndef Q_OS_WIN
    settings.setValue("fontFamily", appFont.family());
    settings.setValue("fontSize", appFont.pointSize());
#endif
    settings.setValue("docfontFamily", docFont.family());
    settings.setValue("docfontSize", docFont.pointSize());
    settings.setValue("docBold", docFont.bold());
    qDebug() << "Writing user settings completed.";
}

