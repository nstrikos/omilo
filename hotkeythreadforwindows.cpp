#include "hotkeythreadforwindows.h"

#include "windows.h"
#include <QApplication>
#define MOD_NOREPEAT    0x4000
#define MOD_ALT         0x0001
#define MOD_CONTROL     0x0002

HotKeyThread::HotKeyThread()
{
    clipBoard = QApplication::clipboard();
}

HotKeyThread::~HotKeyThread()
{
    if (clipBoard != NULL)
    {
        clipBoard = NULL;
    }
}

void HotKeyThread::run()
{
    RegisterHotKey(NULL,1, MOD_NOREPEAT,0x79); //F10
    RegisterHotKey(NULL,2, MOD_NOREPEAT,0x7A); //F11
    //RegisterHotKey(NULL,3, MOD_NOREPEAT,0x7B); //F12
    RegisterHotKey(NULL,3, MOD_ALT | MOD_NOREPEAT,0x20); //Alt + Space
    RegisterHotKey(NULL,4, MOD_NOREPEAT,0x78); //F9

    QApplication::processEvents();

    MSG msg;
    while(GetMessage(&msg,NULL,0,0)){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
        if (msg.message == WM_HOTKEY){
            if (msg.wParam == 1)
            {
                qDebug() << "F10 pressed...";
                emit playPressed();
            }
            else if (msg.wParam == 2)
            {
                qDebug() << "F11 pressed...";
                emit stopPressed();
            }
            else if (msg.wParam == 3)
            {
                qDebug() << "Alt + Space pressed...";
                emit showWindowPressed();
            }
            else if (msg.wParam == 4)
            {
                qDebug() << "F9 pressed...";
                emit pausePressed();
            }
        }
    }
}

