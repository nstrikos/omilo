#include "speechvoice.h"
#include <QDebug>

SpeechVoice::~SpeechVoice()
{

}

void SpeechVoice::emitFileCreatedSignal()
{
    if (!cancelled)
        emit fileCreated(filename);
}

void SpeechVoice::setDurationStretch(unsigned int duration)
{
    this->durationStretch = duration;
    qDebug() << "Set duration stretch to " << durationStretch;
}

void SpeechVoice::setTargetMean(unsigned int target)
{
    targetMean = target;
    qDebug() << "Set target mean to " << targetMean;
}

void SpeechVoice::setUseDurationStretch(bool useDurationStretch)
{
    this->useDurationStretch = useDurationStretch;
}

void SpeechVoice::setUseTargetMean(bool useTargetMean)
{
    this->useTargetMean = useTargetMean;
}

void SpeechVoice::setCustomCommand(QString command)
{
    this->customCommand = command;
    qDebug() << "Set custom command to: " << command;
}

void SpeechVoice::setCustomCommandArguments(QString arguments)
{
    this->customCommandArguments = arguments;
    qDebug() << "Set custom command arguments to: " << arguments;
}
