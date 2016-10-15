#ifndef SPEECHENGINE_H
#define SPEECHENGINE_H

#include "definitions.h"
#include "speechvoice.h"
#include "festivalflitevoice.h"
#include "festivalvoice.h"
#include "customfestivalvoice.h"
#include "flitevoice.h"
#include "maryvoice.h"
#include "downloadmanager.h"
#include "textprocess.h"
#include "soundfilesmerger.h"
#include <QMediaPlaylist>
#include <QTimer>
#include <QQueue>

#ifdef Q_OS_WIN
#include "maryserverforwindows.h"
#endif

class SpeechEngine : public QObject
{
    Q_OBJECT
public:
    SpeechEngine(QString voice);
    ~SpeechEngine();
    void speak(QString text);
    void speakWithoutSplitting(QString text);
    void exportWav(QString filename, QString text);
    void makeDialogue(QList<QString> voices, QList<QString> texts, QList<QString> filenames, bool pausesEnabled);
    void cancel();
    void setSpeechVoice(QString sVoice);
    void setDurationStretch(unsigned int duration);
    void setTargetMean(unsigned int target);
    void setUseDurationStretch(bool useDurationStretch);
    void setUseTargetMean(bool useTargetMean);
    void setCustomFestivalCommand(QString command);
    void setCustomFestivalCommandArguments(QString arguments);
    SpeechVoice *getSpeechVoice();
    void testMaryServer();
    void stopTestingMaryServer();
    bool getIsProcessing();
    void setSplitMode(bool mode);
    void setRate(double rate);

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
    bool useDurationStretch;
    bool useTargetMean;
    QString customFestivalCommand;
    QString customFestivalCommandArguments;
    bool isProcessing;
    bool splitMode;    
    int currentId;
    int maxId;
    int mergeCounter;
    QString mergeCommand;
    QProcess finalSoxProcess;
    QProcess soxProcess;
    void startMerging();
    TextProcess *textProcess;
    QQueue<QString> soxFiles;
    bool exportToWav;
    bool dialogue;
    bool pausesEnabled;
    QProcess addPausesProcess;
    void addPauses();
    int pausesCount;
    SoundFilesMerger *soundFilesMerger;
    double rate;
    QList<QString> voices;
    QList<QString> texts;
    QList<QString> filenames;
#ifdef Q_OS_WIN
    MaryServerForWindows maryServerForWindows;
#endif

private slots:
    void voiceFileCreated(QString filename);
    void processList();
    void processDialogue();
    void addPausesProcessFinished();

signals:
    void fileCreated(QString filename, bool split, unsigned int begin, unsigned int end);
    void processingFinished();
    void newMaxId(int maxId);
    void newId(int id);
    void soxFinished();
    void exportFinished();
    void mergeId(int id, int size);
    void mergeInfo(QString info);
    void dialogueFinished(QList<QString>, QList<QString>, QList<QString>);
};

#endif // SPEECHENGINE_H
