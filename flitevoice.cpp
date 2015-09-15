#include "flitevoice.h"

void FliteVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    writeTextToTmpFile(text);

    //create wav file
    //QString command="flite " + voiceCommand() + " -f " + textFile + " -o " + filename;
    QString command = "/usr/share/omilo-qt5/flite/flite " + voiceCommand() + " -f " + textFile + " -o " + filename;

    QString duration = QString::number(durationStretch/100.0);
    command += " --setf duration_stretch=";
    command += duration;

    QString target = QString::number(targetMean);
    command += " --setf int_f0_target_mean=";
    command +=target;

    createWavProcess->start(command);
}

QString FliteVoice::installationPath()
{
     return "/usr/bin/flite";
}
