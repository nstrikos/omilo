#include "genericdownloadmanager.h"

GenericDownloadManager::GenericDownloadManager()
{
    downloading = false;
    reply = NULL;
}

void GenericDownloadManager::startRequest(QUrl url)
{
    reply = qnam.get(QNetworkRequest(url));
    connect(reply, SIGNAL(finished()),  this, SLOT(httpFinished()));
    connect(reply, SIGNAL(readyRead()), this, SLOT(httpReadyRead()));
}

void GenericDownloadManager::httpFinished()
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

void GenericDownloadManager::httpReadyRead()
{
    // this slot gets called every time the QNetworkReply has new data.
    // We read all of its new data and write it into the file.
    // That way we use less RAM than when reading it at the finished()
    // signal of the QNetworkReply

    if (file)
        file->write(reply->readAll());
}

void GenericDownloadManager::cancelDownload()
{
    httpRequestAborted = true;
    if (reply != NULL)
        reply->abort();
    downloading = false;
    emit downloadingFinished();
}

