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
};

#endif // MARYSERVERFORWINDOWS_H
