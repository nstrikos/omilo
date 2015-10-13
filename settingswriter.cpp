#include "settingswriter.h"
#include <QDebug>

void SettingsWriter::write(QPoint pos, QSize size, QStringList recentFiles, QString engineVoice,
                           bool useTrayIcon, bool splitMode)
{
    qDebug() << "Writing user settings...";
    QSettings settings("Omilo-qt5", "Omilo-qt5");
    settings.setValue("MainWindowPosition", pos);
    settings.setValue("MainWindowSize", size);
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("MainWindowVoice", engineVoice);
    settings.setValue("useTrayIcon", useTrayIcon);
    settings.setValue("SplitMode", splitMode);
    qDebug() << "Writing user settings completed.";
}

