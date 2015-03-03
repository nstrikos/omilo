#ifndef SPEECHENGINE_H
#define SPEECHENGINE_H

#include "definitions.h"
#include "speechvoice.h"
#include "festivalflitevoice.h"
#include "festivalvoice.h"
#include "flitevoice.h"
#include "maryvoice.h"
#include "downloadmanager.h"
#include "textprocess.h"
#include "QTimer"

class SpeechEngine : public QObject
{
    Q_OBJECT
public:
    SpeechEngine(QString voice);
    ~SpeechEngine();
    void speak(QString text);
    void speak(QString text, bool splitMode);
    void exportWav(QString filename, QString text);
    void cancel();
    void setSpeechVoice(QString sVoice);
    void setDurationStretch(unsigned int duration);
    void setTargetMean(unsigned int target);
    SpeechVoice *getSpeechVoice();
    void testMaryServer();
    bool getIsProcessing();
    void setSplitMode(bool mode);

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
    bool isProcessing;
    TextProcess *textProcess;
    bool splitMode;
    bool currentSplitMode;
    TextContainer textContainer;
    unsigned int begin;
    unsigned int end;

private slots:
    void voiceFileCreated(QString filename);
    void processList();

signals:
    void fileCreated(QString filename, bool split, unsigned int begin, unsigned int end);
    void processingFinished();
};

#endif // SPEECHENGINE_H
