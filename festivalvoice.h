#ifndef FESTIVALVOICE_H
#define FESTIVALVOICE_H

#include "festivalflitevoice.h"

class FestivalVoice : public FestivalFliteVoice
{
public:
    void performSpeak(QString filename, QString text);
};

class KalDiphoneFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
        return KalFestival;
    }    
private:
    QString voiceCommand()
    {
        return "";
    }
    QString installationPath()
    {
        return "/usr/share/festival/voices/english/kal_diphone";
    }
};

class AwbCmuFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
         return AwbCmuFestival;
    }
private:
    QString voiceCommand()
    {
        return " -eval \"(voice_cmu_us_awb_arctic_clunits)\"";
    }
    QString installationPath()
    {
        return  "/usr/share/festival/voices/english/cmu_us_awb_arctic_clunits";
    }
};

class BdlCmuFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
        return BdlCmuFestival;
    }
private:
    QString voiceCommand()
    {
          return " -eval \"(voice_cmu_us_bdl_arctic_clunits)\"";
    }
    QString installationPath()
    {
         return  "/usr/share/festival/voices/english/cmu_us_bdl_arctic_clunits";
    }
};

class ClbCmuFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
        return ClbCmuFestival;
    }
private:
    QString voiceCommand()
    {
         return " -eval \"(voice_cmu_us_clb_arctic_clunits)\"";
    }
    QString installationPath()
    {
        return  "/usr/share/festival/voices/english/cmu_us_clb_arctic_clunits";
    }
};

class JmkCmuFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
         return JmkCmuFestival;
    }
private:
    QString voiceCommand()
    {
        return " -eval \"(voice_cmu_us_jmk_arctic_clunits)\"";
    }
    QString installationPath()
    {
         return  "/usr/share/festival/voices/english/cmu_us_jmk_arctic_clunits";
    }
};

class KspCmuFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
         return KspCmuFestival;
    }
private:
    QString voiceCommand()
    {
         return " -eval \"(voice_cmu_us_ksp_arctic_clunits)\"";
    }
    QString installationPath()
    {
         return  "/usr/share/festival/voices/english/cmu_us_ksp_arctic_clunits";
    }
};

class RmsCmuFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
         return RmsCmuFestival;
    }
private:
    QString voiceCommand()
    {
        return " -eval \"(voice_cmu_us_rms_arctic_clunits)\"";
    }
    QString installationPath()
    {
         return  "/usr/share/festival/voices/english/cmu_us_rms_arctic_clunits";
    }
};

class SltCmuFestivalVoice : public FestivalVoice
{
public:
    QString getName()
    {
        return SltCmuFestival;
    }
private:
    QString voiceCommand()
    {
         return " -eval \"(voice_cmu_us_slt_arctic_clunits)\"";
    }
    QString installationPath()
    {
          return  "/usr/share/festival/voices/english/cmu_us_slt_arctic_clunits";
    }
};


#endif // FESTIVALVOICE_H
