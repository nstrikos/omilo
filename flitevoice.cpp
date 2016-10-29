#include "flitevoice.h"
#include <QDebug>

void FliteVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    QString tmpFile = filename+".txt";
    writeTextToTmpFile(tmpFile, text);

    //create wav file
#ifdef Q_OS_WIN
    QString command = fliteCommand + " " + voiceCommand() + " -f " + tmpFile + " -o " + filename;
#else
    QString command = "/usr/share/omilo-qt5/flite/flite " + voiceCommand() + " -f " + tmpFile + " -o " + filename;
#endif
    if ( useDurationStretch )
    {
        QString duration = QString::number(durationStretch/100.0);
        command += " --setf duration_stretch=";
        command += duration;
        qDebug() << "Duration is " << duration;
    }

    if ( useTargetMean )
    {
        QString target = QString::number(targetMean);
        command += " --setf int_f0_target_mean=";
        command +=target;
        qDebug() << "Target mean is " << target;
    }

    qDebug() << "Processing command: " << command;
    createWavProcess->start(command);
}

QString FliteVoice::installationPath()
{
     return "/usr/bin/flite";
}
