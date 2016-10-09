#include "festivalvoice.h"

void FestivalVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    QString tmpFile = filename + ".txt";
    writeTextToTmpFile(tmpFile, text);
    QString command = "/usr/share/omilo-qt5/festival/bin/text2wave -o " + filename + " " + tmpFile + voiceCommand();
    createWavProcess->start(command);
}
