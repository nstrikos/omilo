#include "startupthread.h"

StartupThread::StartupThread(SpeechEngine *speechEngine)
{
    this->engine = speechEngine;
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkMaryServer()));
    timer.start(50);
    checkMaryServer();
}

void StartupThread::checkMaryServer()
{
    //It has to be omilo-test.wav
    //So orca does not replace this file
    QString filename = "/tmp/omilo-test.wav";
    int size = 0;
    QFile myFile(filename);
    if (myFile.open(QIODevice::ReadOnly)){
        size = myFile.size();
        myFile.close();
    }
    if (size == 0)
    {
        engine->testMaryServer();       
    }
    else
    {
        timer.stop();
        engine->stopTestingMaryServer();
        QFile::remove(filename);
        emit maryServerIsUp();
    }
}
