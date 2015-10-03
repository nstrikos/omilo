#include "hotkeythread.h"
#include <X11/Xlib.h>
#include <X11/Xutil.h>

HotKeyThread::HotKeyThread()
{
    qDebug() << "Registering hot keys...";
    stopped = false;
}

void HotKeyThread::stop()
{
    stopped = true;
}

void HotKeyThread::setStopped(bool stopped)
{
    this->stopped = stopped;
}

void HotKeyThread::run()
{
    Display    *dpy     =  XOpenDisplay(0);
    Window      root    = DefaultRootWindow(dpy);
    XEvent      ev;
    KeySym key;
    char text[255];

    //unsigned int    modifiers       = Mod2Mask;
    unsigned int    modifiers = 0;
    int             keycode         = XKeysymToKeycode(dpy, XK_F10);
    int             keycode2        = XKeysymToKeycode(dpy, XK_F11);
    int             keycode3        = XKeysymToKeycode(dpy, XK_F12);
    Window          grab_window     = root;
    Bool            owner_events    = False;
    int             pointer_mode    = GrabModeAsync;
    int             keyboard_mode   = GrabModeAsync;

    XGrabKey(dpy, keycode, modifiers, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);

    XGrabKey(dpy, keycode2, modifiers, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);
    XGrabKey(dpy, keycode3, modifiers, grab_window, owner_events,
             pointer_mode,
             keyboard_mode);

    XSelectInput(dpy, root, KeyPressMask);
    while(!stopped)
    {
        XNextEvent(dpy, &ev);
        switch(ev.type)
        {
        case KeyPress:
            XLookupString(&ev.xkey,text,255,&key,0);
            if (key == 65479)
            {
                //F10 pressed
                qDebug() << "F10 pressed...";
                emit playPressed();
            }
            else if (key == 65480)
            {
                //F11 pressed
                qDebug() << "F11 pressed...";
                emit stopPressed();
            }
            else if (key == 65481)
            {
                //F12 pressed
                qDebug() << "F12 pressed";
                emit showWindowPressed();
            }
        }
    }

    if (stopped)
    {
        XUngrabKey(dpy,keycode,modifiers,grab_window);
        XUngrabKey(dpy,keycode2,modifiers,grab_window);
    }
    stopped = true;
    XCloseDisplay(dpy);
}
