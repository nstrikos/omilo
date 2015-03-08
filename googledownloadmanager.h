#ifndef GOOGLEDOWNLOADMANAGER_H
#define GOOGLEDOWNLOADMANAGER_H

#include <QtNetwork>
#include "definitions.h"
#include "genericdownloadmanager.h"

class GoogleDownloadManager : public GenericDownloadManager
{

public:
    void performSpeak(QString filename, QString text, QString voiceCommand);

protected:
    void finishRequest();

signals:
    void test(QString file);
};

#endif // GOOGLEDOWNLOADMANAGER_H
