#ifndef SPEECHVOICE_H
#define SPEECHVOICE_H

#include <QProcess>
#include <QFile>
#include <QLocale>

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
    bool useDurationStretch;
    bool useTargetMean;
    QString customCommand;
    QString customCommandArguments;

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
    void setUseDurationStretch(bool useDurationStretch);
    void setUseTargetMean(bool useTargetMean);
    void setCustomCommand(QString command);
    void setCustomCommandArguments(QString arguments);

signals:
    void fileCreated(QString filename);

};


#endif // SPEECHVOICE_H
