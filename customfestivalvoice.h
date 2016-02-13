#ifndef CUSTOMFESTIVALVOICE_H
#define CUSTOMFESTIVALVOICE_H

#include "festivalvoice.h"


class CustomFestivalVoice : public FestivalVoice
{
public:
    void performSpeak(QString filename, QString text);
    QString getName()
    {
        return CustomFestival;
    }
    void setVoiceCommand(QString command)
    {
        this->command = command;
    }

private:
    QString command;
    QString voiceCommand()
    {
        return command;
    }

    QString installationPath()
    {
          return  "";
    }
};

#endif // CUSTOMFESTIVALVOICE_H
