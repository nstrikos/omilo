#include "downloadmanager.h"

void DownloadManager::performSpeak(QString filename, QString text, QString voiceCommand)
{
    QString finalCommand, command1, command2;
    this->filename = filename;

    //this check is necessery to prevent from multiple user requests
    //that may crash the application
    if (downloading == false)
    {
        downloading = true;
        finalCommand = "http://localhost:59127/process?INPUT_TEXT=";
        command1 = "&INPUT_TYPE=TEXT&OUTPUT_TYPE=AUDIO&AUDIO=WAVE_FILE&";
        command2 = voiceCommand;
        finalCommand = finalCommand + text + command1 + command2;

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

void DownloadManager::finishRequest()
{
    //we are now able to handle new user requests
    downloading = false;
    emit downloadingFinished();
}
