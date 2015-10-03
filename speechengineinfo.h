#ifndef SPEECHENGINEINFO_H
#define SPEECHENGINEINFO_H

#include <vector>
using namespace std;

#include <QDir>
#include <QDebug>
#include "speechvoice.h"
#include "definitions.h"

class SpeechEngineInfo
{
public:
    SpeechEngineInfo();
    ~SpeechEngineInfo();
    vector<VoiceInfo> installedVoices;
    vector<VoiceInfo> availableVoices;
    //If we need to add a new voice we should re-code this function
    void update();
    engineMode getVoiceMode(QString voice);
};

#endif // SPEECHENGINEINFO_H
