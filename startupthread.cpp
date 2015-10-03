#include "startupthread.h"

StartupThread::StartupThread(SpeechEngine *speechEngine)
{
    qDebug() << "Starting mary server testing tread...";
    this->engine = speechEngine;
    connect(&timer, SIGNAL(timeout()), this, SLOT(checkMaryServer()));
    timer.start(50);
    checkMaryServer();
}

StartupThread::~StartupThread()
{
    qDebug() << "Deleting start up thread...";
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
        qDebug() << "Mary server is up...";
        timer.stop();
        QFile::remove(filename);
        engine->stopTestingMaryServer();
        emit maryServerIsUp();
    }
}
