#ifndef HOTKEYTHREAD_H
#define HOTKEYTHREAD_H

#include <QThread>
#include <QDebug>

class HotKeyThread : public QThread
{
    Q_OBJECT
public:
    HotKeyThread();
    void stop();
    void setStopped(bool stopped);

protected:
    void run();

private:
    volatile bool stopped;

signals:
    void playPressed();
    void stopPressed();
    void showWindowPressed();
};

#endif // HOTKEYTHREAD_H
