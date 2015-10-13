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
#include "soundfilesmerger.h"
#include <QMediaPlaylist>
#include <QTimer>
#include <QQueue>

class SpeechEngine : public QObject
{
    Q_OBJECT
public:
    SpeechEngine(QString voice);
    ~SpeechEngine();
    void speak(QString text);
    void speakWithoutSplitting(QString text);
    void exportWav(QString filename, QString text);
    void cancel();
    void setSpeechVoice(QString sVoice);
    void setDurationStretch(unsigned int duration);
    void setTargetMean(unsigned int target);
    SpeechVoice *getSpeechVoice();
    void testMaryServer();
    void stopTestingMaryServer();
    bool getIsProcessing();
    void setSplitMode(bool mode);
    void setExportToWav(bool value);

private:
    QString text;
    SpeechVoice *speechVoice; //We are using strategy pattern
    SpeechVoice *testVoice;
    QProcess *maryServerProcess;
    void createVoice(SpeechVoice *sVoice); //We need a factory here
    void startMaryProcess();
    QString voice;
    //unsigned int count;
    QString filename;
    unsigned int durationStretch;
    unsigned int targetMean;
    bool isProcessing;
    bool splitMode;    
    int currentId;
    int maxId;
    int mergeCounter;
    QString mergeCommand;
    QProcess finalSoxProcess;
    void startMerging();
    TextProcess *textProcess;
    bool overlap;
    int limit;
    QQueue<QString> soxFiles;
    bool exportToWav;
    SoundFilesMerger *soundFilesMerger;

private slots:
    void voiceFileCreated(QString filename);
    void processList();    

signals:
    void fileCreated(QString filename, bool split, unsigned int begin, unsigned int end);
    void processingFinished();
    void newMaxId(int maxId);
    void newId(int id);
    void soxFinished();
    void exportFinished();
    void mergeId(int id, int size);
    void mergeInfo(QString info);
};

#endif // SPEECHENGINE_H
