#include "speechvoice.h"

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
    durationStretch = duration;
}

void SpeechVoice::setTargetMean(unsigned int target)
{
    targetMean = target;
}
