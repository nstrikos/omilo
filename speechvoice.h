#ifndef SPEECHVOICE_H
#define SPEECHVOICE_H

#include <QProcess>
#include <QFile>

#include "definitions.h"

class SpeechVoice : public QObject
{
    Q_OBJECT
private:
    //QString name;
    QLocale::Language language;

protected:
    QString filename;
    bool cancelled;
    unsigned int durationStretch;
    unsigned int targetMean;

protected slots:
    void emitFileCreatedSignal();

public:
    virtual ~SpeechVoice();
    virtual void performSpeak(QString filename, QString text) = 0;
    virtual void cancel() = 0;
    virtual QString installationPath() = 0;
    virtual QString getName() = 0;
    virtual QString getLanguage() = 0;
    virtual QString voiceCommand() = 0;
    void setDurationStretch(unsigned int duration);
    void setTargetMean(unsigned int target);

signals:
    void fileCreated(QString filename);

};


#endif // SPEECHVOICE_H
