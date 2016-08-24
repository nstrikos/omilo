#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include "definitions.h"
#include <QtNetwork>

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager();
    ~DownloadManager();
    void performSpeak(QString filename, QString text, QString voiceCommand);

private:
    void startRequest(QUrl url);
    void finishRequest();
    bool downloading;
    QFile *file;
    QNetworkAccessManager qnam;
    QNetworkReply *reply;
    bool httpRequestAborted;
    QString filename;

private slots:
    void httpFinished();
    void httpReadyRead();

public slots:
    void cancelDownload();

signals:
    void downloadingFinished();
};

#endif // DOWNLOADMANAGER_H
