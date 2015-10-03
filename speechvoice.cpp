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
