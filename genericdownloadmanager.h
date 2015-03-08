#ifndef GENERICDOWNLOADMANAGER_H
#define GENERICDOWNLOADMANAGER_H


#include <QtNetwork>
#include "definitions.h"

class GenericDownloadManager : public QObject
{
    Q_OBJECT

public:
    GenericDownloadManager();
    virtual void performSpeak(QString filename, QString text, QString voiceCommand) = 0;

protected:
    void startRequest(QUrl url);
    virtual void finishRequest() = 0;
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

#endif // GENERICDOWNLOADMANAGER_H
