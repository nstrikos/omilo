#ifndef FLITEVOICE_H
#define FLITEVOICE_H

#include "festivalflitevoice.h"
#include "definitions.h"

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
        this->durationStretch = defDurationStretch;
        this->targetMean = defSltTargetMean;
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

class FemFliteVoice : public FliteVoice
{
public:
    FemFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return FemFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/cmu_us_fem.flitevox";
    }
};

class JmkFliteVoice : public FliteVoice
{
public:
    JmkFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return JmkFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/cmu_us_jmk.flitevox";
    }
};

class RxrFliteVoice : public FliteVoice
{
public:
    RxrFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return RxrFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/cmu_us_rxr.flitevox";
    }
};

#endif // FLITEVOICE_H
