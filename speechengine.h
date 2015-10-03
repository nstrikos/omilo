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
#include <QMediaPlaylist>
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
    void stopTestingMaryServer();
    bool getIsProcessing();
    void setSplitMode(bool mode);
    //    void setPlaylist(QMediaPlaylist* playerList);

private:
    QString text;
    SpeechVoice *speechVoice; //We are using strategy pattern
    SpeechVoice *testVoice;
    QProcess *maryServerProcess;
    void createVoice(SpeechVoice *sVoice); //We need a factory here
    void startMaryProcess();
    QString voice;
    unsigned int count;
    QString filename;
    unsigned int durationStretch;
    unsigned int targetMean;
    bool isProcessing;
    bool splitMode;
    //    bool currentSplitMode;
    //    TextContainer textContainer;
    //    unsigned int begin;
    //    unsigned int end;
    //    QMediaPlaylist *playlist;
    //    int producedFiles;
    //    int spokenFiles;
    //      QString currentVoice;
    //    TextProcess *textProcess;



private slots:
    void voiceFileCreated(QString filename);
//    void processList();
//    void filePlayed();

signals:
    void fileCreated(QString filename, bool split, unsigned int begin, unsigned int end);
    void processingFinished();
};

#endif // SPEECHENGINE_H
