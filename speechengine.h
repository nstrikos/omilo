#ifndef SPEECHENGINE_H
#define SPEECHENGINE_H

#include "definitions.h"
#include "speechvoice.h"
#include "festivalflitevoice.h"
#include "festivalvoice.h"
#include "flitevoice.h"
#include "maryvoice.h"
#include "downloadmanager.h"
#include "QTimer"

class SpeechEngine : public QObject
{
    Q_OBJECT
public:
    SpeechEngine(QString voice);
    ~SpeechEngine();
    void speak(QString text);
    void exportWav(QString filename, QString text);
    void cancel();
    void setSpeechVoice(QString sVoice);
    void setDurationStretch(unsigned int duration);
    void setTargetMean(unsigned int target);
    SpeechVoice *getSpeechVoice();
    void testMaryServer();

private:
    QString text;
    SpeechVoice *speechVoice; //We are using strategy pattern
    QProcess *maryServerProcess;
    void createVoice(SpeechVoice *sVoice); //We need a factory here
    void startMaryProcess();
    QString voice;
    unsigned int count;
    QString filename;
    DownloadManager *maryTestingDownloadManager;
    unsigned int durationStretch;
    unsigned int targetMean;

private slots:
    void voiceFileCreated(QString filename);

signals:
    void fileCreated(QString filename);
};

#endif // SPEECHENGINE_H
