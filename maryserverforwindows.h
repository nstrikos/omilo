#ifndef MARYSERVERFORWINDOWS_H
#define MARYSERVERFORWINDOWS_H

#include <QProcess>

class MaryServerForWindows
{
public:
    MaryServerForWindows();
    void startMaryServer(QProcess *maryServerProcess);

private:
    double getAvailableMemory();
    void StartMaryServerProcessForWindows(int memory);
    void delay(int n);
    QProcess *maryServerProcess;

    double availableMemory;
    const int MAX_MEMORY_FOR_MARY =  2000; // 2000 megabytes
    const int NORMAL_MEMORY_FOR_MARY = 1000; //1000 megabytes
    const int MIN_MEMORY_FOR_MARY = 500; //500 megabytes
};

#endif // MARYSERVERFORWINDOWS_H
