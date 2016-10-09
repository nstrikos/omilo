#ifndef FESTIVALFLITEVOICE_H
#define FESTIVALFLITEVOICE_H

#include "speechvoice.h"
#include <QTextStream>

class FestivalFliteVoice : public SpeechVoice
{
protected:
    FestivalFliteVoice();
    ~FestivalFliteVoice();
    virtual QString voiceCommand() = 0;
    void writeTextToTmpFile(QString file, QString text);
    QString getLanguage();
    QProcess *createWavProcess;
    void cancel();
};

#endif // FESTIVALFLITEVOICE_H
