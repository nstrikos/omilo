#include "customfestivalvoice.h"

void CustomFestivalVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    writeTextToTmpFile("",text);
    customCommandArguments.replace("\'", "\"");
    QString command = customCommand + " -o " + filename + " " + textFile + " " + customCommandArguments;
    createWavProcess->start(command);
}
