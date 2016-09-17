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
        this->targetMean = defTargetMean;
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

class IndicAupMrFliteVoice : public FliteVoice
{
public:
    IndicAupMrFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicAupMrFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_aup_mr.flitevox";
    }
};

class IndicAxbGuFliteVoice : public FliteVoice
{
public:
    IndicAxbGuFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicAxbGuFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_axb_gu.flitevox";
    }
};

class IndicAxbHiFliteVoice : public FliteVoice
{
public:
    IndicAxbHiFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicAxbHiFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_axb_hi.flitevox";
    }
};

class IndicKnrTeFliteVoice : public FliteVoice
{
public:
    IndicKnrTeFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicKnrTeFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_knr_te.flitevox";
    }
};

class IndicSksTaFliteVoice : public FliteVoice
{
public:
    IndicSksTaFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicSksTaFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_sks_ta.flitevox";
    }
};

class IndicSlpMrFliteVoice : public FliteVoice
{
public:
    IndicSlpMrFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicSlpMrFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_slp_mr.flitevox";
    }
};

class IndicSxsHiFliteVoice : public FliteVoice
{
public:
    IndicSxsHiFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicSxsHiFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_sxs_hi.flitevox";
    }
};

class IndicSxvTaFliteVoice : public FliteVoice
{
public:
    IndicSxvTaFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return IndicSxvTaFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_indic_sxv_ta.flitevox";
    }
};

class AewFliteVoice : public FliteVoice
{
public:
    AewFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return AewFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_aew.flitevox";
    }
};

class AhwFliteVoice : public FliteVoice
{
public:
    AhwFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return AhwFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_ahw.flitevox";
    }
};

class AupFliteVoice : public FliteVoice
{
public:
    AupFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return AupFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_aup.flitevox";
    }
};

class Awb2FliteVoice : public FliteVoice
{
public:
    Awb2FliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return Awb2Flite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_awb.flitevox";
    }
};

class AxbFliteVoice : public FliteVoice
{
public:
    AxbFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return AxbFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_axb.flitevox";
    }
};

class BdlFliteVoice : public FliteVoice
{
public:
    BdlFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return BdlFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_bdl.flitevox";
    }
};

class ClbFliteVoice : public FliteVoice
{
public:
    ClbFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return ClbFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_clb.flitevox";
    }
};

class EeyFliteVoice : public FliteVoice
{
public:
    EeyFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return EeyFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_eey.flitevox";
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
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_fem.flitevox";
    }
};

class GkaFliteVoice : public FliteVoice
{
public:
    GkaFliteVoice()
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
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_gka.flitevox";
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
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_jmk.flitevox";
    }
};

class KspFliteVoice : public FliteVoice
{
public:
    KspFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return KspFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_ksp.flitevox";
    }
};

class LjmFliteVoice : public FliteVoice
{
public:
    LjmFliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return LjmFlite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_ljm.flitevox";
    }
};

class Rms2FliteVoice : public FliteVoice
{
public:
    Rms2FliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return Rms2Flite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_rms.flitevox";
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
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_rxr.flitevox";
    }
};

class Slt2FliteVoice : public FliteVoice
{
public:
    Slt2FliteVoice()
    {
        durationStretch = defDurationStretch;
        targetMean = defTargetMean;
    }
    QString getName()
    {
        return Slt2Flite;
    }
private:
    QString voiceCommand()
    {
        return "-voice /usr/share/omilo-qt5/flite/voices/cmu_us_slt.flitevox";
    }
};

#endif // FLITEVOICE_H
