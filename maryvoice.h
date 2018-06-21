#ifndef MARYVOICE_H
#define MARYVOICE_H

#include "speechvoice.h"
#include "downloadmanager.h"

class MaryVoice : public SpeechVoice
{
    Q_OBJECT

public:
    MaryVoice();
    ~MaryVoice();
    virtual QString voiceCommand() = 0;

protected:
    void  performSpeak(QString filename, QString text);
    void cancel();
    QString installationPath();
    virtual QString getName() = 0;

private:
    DownloadManager *maryDownloadManager;
};

class ItalianMaryVoice : public MaryVoice
{
    QString getName()
    {
        return ItalianMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::Italian);
    }
    QString voiceCommand()
    {
        return "LOCALE=it&VOICE=istc-lucia-hsmm";
    }
};

class GermanMaryVoice : public MaryVoice
{
    QString getName()
    {
        return GermanMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::German);
    }
    QString voiceCommand()
    {
        return "LOCALE=de&VOICE=dfki-pavoque-neutral-hsmm";
    }
};

class GreekHerculesVoice : public MaryVoice
{
    QString getName()
    {
        return GreekHercules;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::Greek);
    }
    QString voiceCommand()
    {
        return "LOCALE=el&VOICE=alchemist-1992-hsmm-hsmm";
    }
};

class GreekMaryVoice : public MaryVoice
{
    QString getName()
    {
        return GreekMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::Greek);
    }
    QString voiceCommand()
    {
        return "LOCALE=el&VOICE=emily-v2.0.1-hmm";
    }
};

class GreekGoogleMaryVoice : public MaryVoice
{
    QString getName()
    {
        return GreekGoogleMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::Greek);
    }
    QString voiceCommand()
    {
        return "LOCALE=el&VOICE=google-10";
    }
};

class TurkishMaryVoice : public MaryVoice
{
    QString getName()
    {
        return TurkishMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::Turkish);
    }
    QString voiceCommand()
    {
        return "LOCALE=tr&VOICE=dfki-ot-hsmm";
    }
};

class FrenchMaryVoice : public MaryVoice
{
    QString getName()
    {
        return FrenchMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::French);
    }
    QString voiceCommand()
    {
        return "LOCALE=fr&VOICE=enst-dennys-hsmm";
    }
};

class CmuBdlMaryVoice : public MaryVoice
{
    QString getName()
    {
        return CmuBdlMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=cmu-bdl-hsmm";
    }
};

class CmuRmsMaryVoice : public MaryVoice
{
    QString getName()
    {
        return CmuRmsMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=cmu-rms-hsmm";
    }
};

class CmuSltMaryVoice : public MaryVoice
{
    QString getName()
    {
        return CmuSltMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=cmu-slt-hsmm";
    }
};

class ObadiahMaryVoice : public MaryVoice
{
    QString getName()
    {
        return ObadiahMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=dfki-obadiah-hsmm";
    }
};

class PoppyMaryVoice : public MaryVoice
{
    QString getName()
    {
        return PoppyMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=dfki-poppy-hsmm";
    }
};

class PrudenceMaryVoice : public MaryVoice
{
    QString getName()
    {
        return PrudenceMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=dfki-prudence-hsmm";
    }
};

class SpikeMaryVoice : public MaryVoice
{
    QString getName()
    {
        return SpikeMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=dfki-spike-hsmm";
    }
};

class CoriMaryVoice : public MaryVoice
{
    QString getName()
    {
        return CoriMary;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "LOCALE=en_US&VOICE=cori-2740-hsmm";
    }
};

#endif // MARYVOICE_H
