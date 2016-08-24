#ifndef HOTKEYTHREADFORWINDOWS_H
#define HOTKEYTHREADFORWINDOWS_H

#include <QThread>
#include <QtWidgets>
#include <QClipboard>

class HotkeyThread : public QThread
 {
     Q_OBJECT
public:
    HotkeyThread();
    ~HotkeyThread();


protected:
    void run();

private:
    QClipboard *clipBoard;

signals:
     void setGreekVoice();
     void setEnglishVoice();
     void speakHighlightedText(QString);
     void stop();
     void pause();

signals:
    void playPressed();
    void stopPressed();
    void showWindowPressed();
    void pausePressed();
 };

#endif // HOTKEYTHREADFORWINDOWS_H
