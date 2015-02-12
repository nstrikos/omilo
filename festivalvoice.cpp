#include "festivalvoice.h"

void FestivalVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    writeTextToTmpFile(text);
    QString command = "/usr/share/omilo/festival/bin/text2wave -o " + filename + " " + textFile + voiceCommand();
    createWavProcess->start(command);
}
