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

    double availableMemory;
    int memoryForMaryServer = 0;
    const int MAX_MEMORY_FOR_MARY =  2000; // 2000 megabytes
    const int NORMAL_MEMORY_FOR_MARY = 1000; //1000 megabytes
    const int MIN_MEMORY_FOR_MARY = 500; //500 megabytes

    availableMemory = getAvailableMemory();

    //if there is enough memory available we allocate it to mary server
    //otherwise we choose the default allocation
    if (availableMemory > MAX_MEMORY_FOR_MARY)
        //try to allocate 2Gb for mary server
        StartMaryServerProcessForWindows(MAX_MEMORY_FOR_MARY);

    //if process has not started try with 1gb
    if  (maryServerProcess->pid() == 0)
    {
        if (availableMemory > NORMAL_MEMORY_FOR_MARY)
            //try to allocate 1Gb for mary server
            StartMaryServerProcessForWindows(NORMAL_MEMORY_FOR_MARY);

    }

    if  (maryServerProcess->pid() == 0)
        //try to allocate 500mb for mary server
        StartMaryServerProcessForWindows(MIN_MEMORY_FOR_MARY);

    //Not necessary anymore
    //Openmary.py now waits enough to connect with server
    //startNVDA();

    //finally if process fails to start we display information
    if (maryServerProcess->pid() == 0)
    {
        //            memoryForMaryServer = 0;
        //            QMessageBox msgBox;
        //            msgBox.setText( QObject::tr("Η Έμιλυ δεν μπορεί να ξεκινήσει.") );
        //            msgBox.setIcon( QMessageBox::Critical );
        //            msgBox.exec();
        qDebug() << "Cannot start Mary server";
    }
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
        //try to allocate memory for mary server
        QString directory = QDir::currentPath();
        directory.replace("/", "\\");
        QString string1 = "\"" + directory + "\\marytts\\bin\\jre\\bin\\java\" -ea -Xms40m -Xmx" + QString::number(memory) +"m -cp ";
        QString string2 = "\"" + directory + "\\marytts\\lib\\*\" \"-Dserver=http\" marytts.server.Mary\"";
        QString string3 = string1 + " " + string2;
        qDebug() << string3;

        maryServerProcess->start(string3);

        //Why do we have to keep track of memory???
        //This should be a class variable
        //memoryForMaryServer = memory;

        //If mary server has started this delay does not affect anything
        //If mary server has not started this delay is necessary to catch the flaw in the next check
        delay(2);
    }
}

void MaryServerForWindows::delay(int n)
{
    QTime dieTime= QTime::currentTime().addSecs(n);
    while( QTime::currentTime() < dieTime )
        QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}
