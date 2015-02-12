#include "downloadmanager.h"

DownloadManager::DownloadManager()
{
    downloadingFromMary = false;
    reply = NULL;
}

void DownloadManager::performMaryTTSSpeak(QString filename, QString text, QString voiceCommand)
{
    QString finalCommand, command1, command2;
    this->filename = filename;

    //this check is necessery to prevent from multiple user requests
    //that may crash the application
    if (downloadingFromMary == false)
    {
        downloadingFromMary = true;
        finalCommand = "http://localhost:59125/process?INPUT_TEXT=";
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

void DownloadManager::startRequest(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),  this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
}

void DownloadManager::httpFinished()
{
    if (httpRequestAborted) {
        if (file) {
            file->close();
            file->remove();
            delete file;
            file = 0;
        }
        reply->deleteLater();
        reply = NULL;
        return;
    }
    file->flush();
    file->close();

    reply->deleteLater();
    reply = 0;
    delete file;
    file = 0;
    finishRequest();
}

void DownloadManager::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply

    if (file)
        file->write(reply->readAll());
}

void DownloadManager::finishRequest()
{
    //we are now able to handle new user requests
    downloadingFromMary = false;
    emit downloadingFinished();
}

void DownloadManager::cancelDownload()
{
    httpRequestAborted = true;
    if (reply != NULL)
        reply->abort();
    downloadingFromMary = false;
    emit downloadingFinished();
}
