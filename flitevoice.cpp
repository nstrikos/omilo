#include "flitevoice.h"
#include <QDebug>

void FliteVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    writeTextToTmpFile(text);

    //create wav file
    QString command = "/usr/share/omilo-qt5/flite/flite " + voiceCommand() + " -f " + textFile + " -o " + filename;

    QString duration = QString::number(durationStretch/100.0);
    command += " --setf duration_stretch=";
    command += duration;
    //qDebug() << "Duration is " << duration;

    QString target = QString::number(targetMean);
    command += " --setf int_f0_target_mean=";
    command +=target;
    //qDebug() << "Target mean is " << target;

    qDebug() << "Processing command: " << command;
    createWavProcess->start(command);
}

QString FliteVoice::installationPath()
{
     return "/usr/bin/flite";
}
