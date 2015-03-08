#ifndef GOOGLEVOICE_H
#define GOOGLEVOICE_H

#include "speechvoice.h"
#include "googledownloadmanager.h"

class GoogleVoice : public SpeechVoice
{
    Q_OBJECT

public:
    GoogleVoice();
    ~GoogleVoice();
    virtual QString voiceCommand() = 0;

protected:
    void  performSpeak(QString filename, QString text);
    void cancel();
    QString installationPath();
    virtual QString getName() = 0;

private:
    GoogleDownloadManager *googleDownloadManager;
};

class EnglishGoogleVoice : public GoogleVoice
{
    QString getName()
    {
        return GoogleEnglish;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::English);
    }
    QString voiceCommand()
    {
        return "en";
    }
};

class GermanGoogleVoice : public GoogleVoice
{
    QString getName()
    {
        return GoogleGerman;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::German);
    }
    QString voiceCommand()
    {
        return "de";
    }
};

class GreekGoogleVoice : public GoogleVoice
{
    QString getName()
    {
        return GoogleGreek;
    }
    QString getLanguage()
    {
        return QLocale::languageToString(QLocale::Greek);
    }
    QString voiceCommand()
    {
        return "el";
    }
};

#endif // GOOGLEVOICE_H
