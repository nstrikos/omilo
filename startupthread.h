#ifndef STARTUPTHREAD_H
#define STARTUPTHREAD_H

#include "speechengine.h"

class StartupThread : public QObject
{
    Q_OBJECT

public:
    StartupThread(SpeechEngine *speechEngine);

private slots:
    void checkMaryServer();


private:
    SpeechEngine *engine;
    QTimer timer;

signals:
    void maryServerIsUp();
};

#endif // STARTUPTHREAD_H
