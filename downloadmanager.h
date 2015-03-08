#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QtNetwork>
#include "definitions.h"
#include "genericdownloadmanager.h"

class DownloadManager : public GenericDownloadManager
{

public:
    void performSpeak(QString filename, QString text, QString voiceCommand);

protected:
    void finishRequest();
};

#endif // DOWNLOADMANAGER_H
