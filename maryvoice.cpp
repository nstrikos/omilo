#include "maryvoice.h"

MaryVoice::MaryVoice()
{
    maryDownloadManager = new DownloadManager();
    connect(maryDownloadManager, SIGNAL(downloadingFinished()), this, SLOT(emitFileCreatedSignal()));
}

MaryVoice::~MaryVoice()
{
    delete maryDownloadManager;
}

void MaryVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    maryDownloadManager->performMaryTTSSpeak(filename, text, voiceCommand());
}

void MaryVoice::cancel()
{
    cancelled = true;
    maryDownloadManager->cancelDownload();
}

QString MaryVoice::installationPath()
{
    return "/usr/share/marytts-5.0";
}
