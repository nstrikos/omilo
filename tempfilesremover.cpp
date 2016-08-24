#include "tempfilesremover.h"
#include <QFile>
#include <QDir>
#include <QDebug>
#include "definitions.h"

void TempFilesRemover::remove()
{
    qDebug() << "Removing all temp files...";
    QStringList nameFilter("omilo-*");
    QDir directory(tempPath);
    QStringList txtFilesAndDirectories = directory.entryList(nameFilter);
    for (int i = 0; i < txtFilesAndDirectories.size(); i++)
    {
        QString filename = txtFilesAndDirectories.at(i);
        QString command = tempPath + filename;
        QFile::remove(command);
    }
    if(QFile::exists(textFile))
        QFile::remove(textFile);
    if(QFile::exists(wavFile))
        QFile::remove(wavFile);
    if(QFile::exists(voiceTextEditorFile))
        QFile::remove(voiceTextEditorFile);

    //Festival temporary files
    QStringList nameFilter2("est_*");
    QDir directory2("/tmp/");
    QStringList txtFilesAndDirectories2 = directory2.entryList(nameFilter2);
    for (int i = 0; i < txtFilesAndDirectories2.size(); i++)
    {
        QString filename = txtFilesAndDirectories2.at(i);
        QString command = "/tmp/" + filename;
        QFile::remove(command);
    }
    qDebug() << "Temp files removed.";
}

