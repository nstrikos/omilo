#include "googlevoice.h"

GoogleVoice::GoogleVoice()
{
    googleDownloadManager = new GoogleDownloadManager();
    connect(googleDownloadManager, SIGNAL(downloadingFinished()), this, SLOT(emitFileCreatedSignal()));
}

GoogleVoice::~GoogleVoice()
{
    delete googleDownloadManager;
}

void GoogleVoice::performSpeak(QString filename, QString text)
{
    cancelled = false;
    this->filename = filename;
    googleDownloadManager->performSpeak(filename, text, voiceCommand());
}

void GoogleVoice::cancel()
{
    cancelled = true;
    googleDownloadManager->cancelDownload();
}

QString GoogleVoice::installationPath()
{
    return "/usr/share/marytts-5.0";
}
