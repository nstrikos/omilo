#ifndef SPEECHENGINEINFO_H
#define SPEECHENGINEINFO_H

#include <vector>
using namespace std;

#include <QDir>
#include "speechvoice.h"
#include "definitions.h"

class SpeechEngineInfo
{
public:
    SpeechEngineInfo();
    vector<VoiceInfo> installedVoices;
    vector<VoiceInfo> availableVoices;
    //If we need to add a new voice we should re-code this function
    void update();
};

#endif // SPEECHENGINEINFO_H
