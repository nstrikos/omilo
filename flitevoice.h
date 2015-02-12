#ifndef FLITEVOICE_H
#define FLITEVOICE_H

#include "festivalflitevoice.h"

class FliteVoice : public FestivalFliteVoice
{
public:
    void performSpeak(QString filename, QString text);
    QString installationPath();
};

class RmsFliteVoice : public FliteVoice
{
public:
    RmsFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean =  defTargetMean;
    }
    QString getName()
    {
        return RmsFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice rms";
    }
};

class SltFliteVoice : public FliteVoice
{
public:
    SltFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defSltTargetMean;
    }
    QString getName()
    {
        return SltFlite;
   }
private:
    QString voiceCommand()
    {
        return "-voice slt";
    }
};

class AwbFliteVoice : public FliteVoice
{
public:
    AwbFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return AwbFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice awb";
    }
};

class Kal16FliteVoice : public FliteVoice
{
public:
    Kal16FliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return Kal16Flite;
    }
private:
    QString voiceCommand()
    {
        return "-voice kal16";
    }
};

#endif // FLITEVOICE_H
