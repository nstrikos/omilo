#include "maryserverforwindows.h"
#include <windows.h>
#include <QDir>
#include <QTime>
#include <QCoreApplication>
#include <QDebug>

MaryServerForWindows::MaryServerForWindows()
{

}

void MaryServerForWindows::startMaryServer(QProcess *maryServerProcess)
{
    this->maryServerProcess = maryServerProcess;



    availableMemory = getAvailableMemory();

    qDebug() << "Available memory" << availableMemory << "Mb...";

    if (availableMemory > MAX_MEMORY_FOR_MARY)
        StartMaryServerProcessForWindows(MAX_MEMORY_FOR_MARY);
    if  (maryServerProcess->pid() == 0)
    {
        if (availableMemory > NORMAL_MEMORY_FOR_MARY)
            StartMaryServerProcessForWindows(NORMAL_MEMORY_FOR_MARY);
    }
    if  (maryServerProcess->pid() == 0)
        StartMaryServerProcessForWindows(MIN_MEMORY_FOR_MARY);

    if (maryServerProcess->pid() == 0)
        qDebug() << "Cannot start Mary server...";

}

double MaryServerForWindows::getAvailableMemory()
{
    const int MEGABYTE =  1048576;

    MEMORYSTATUSEX memory_status;
    ZeroMemory(&memory_status, sizeof(MEMORYSTATUSEX));
    memory_status.dwLength = sizeof(MEMORYSTATUSEX);
    GlobalMemoryStatusEx(&memory_status);
    return (double) memory_status.ullAvailPhys/MEGABYTE;
}

void MaryServerForWindows::StartMaryServerProcessForWindows(int memory)
{

    if  (maryServerProcess->pid() == 0)
    {
        QString directory = QDir::currentPath();
        directory.replace("/", "\\");
        QString string1 = "\"" + directory + "\\marytts\\bin\\jre\\bin\\java\" -ea -Xms40m -Xmx" + QString::number(memory) +"m -cp ";
        QString string2 = "\"" + directory + "\\marytts\\lib\\*\" \"-Dserver=http\" marytts.server.Mary\"";
        QString finalString = string1 + " " + string2;

        qDebug() << "Set maximum memory for mary server to" << memory << "megabytes...";
        qDebug() << "Command to start mary server: " << finalString;
        maryServerProcess->start(finalString);

        //If mary server has started this delay does not affect anything
        //If mary server has not started this delay is necessary to catch the flaw in the next check
        delay(2);
    }
}

void MaryServerForWindows::delay(int n)
{
    //delays cpu for n seconds
    QTime dieTime= QTime::currentTime().addSecs(n);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
