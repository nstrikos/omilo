#include "googledownloadmanager.h"

void GoogleDownloadManager::performSpeak(QString filename, QString text, QString voiceCommand)
{
    QString finalCommand, command1, command2;
    this->filename = filename;

    //this check is necessery to prevent from multiple user requests
    //that may crash the application
    if (downloading == false)
    {
        downloading = true;
        finalCommand = "http://translate.google.com/translate_tts?ie=UTF-8&tl=" + voiceCommand + "&q=" + text;
        file = new QFile(filename);
        if (!this->file->open(QIODevice::WriteOnly))
        {
            delete this->file;
            this->file = 0;
            return;
        }
        // schedule the request
        httpRequestAborted = false;
        startRequest(finalCommand);
    }
}

void GoogleDownloadManager::finishRequest()
{
    //we are now able to handle new user requests
    downloading = false;
    emit downloadingFinished();
}

