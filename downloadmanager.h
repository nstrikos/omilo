#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QTextStream>
#include <QtNetwork>
#include "definitions.h"

class DownloadManager : public QObject
{
    Q_OBJECT

public:
    DownloadManager();
    void performMaryTTSSpeak(QString filename, QString text, QString voiceCommand);

private:
    void startRequest(QUrl url);
    void finishRequest();
    bool downloadingFromMary;
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
