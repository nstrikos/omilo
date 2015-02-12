#include <QtGui>
#include "mainwindow.h"
#include "definitions.h"
#include <signal.h>
#include <QMessageBox>


static QSharedMemory sharedMemory;

void signalhandler(int sig)
{
    //When the application receives a segmentation fault
    //terminate or kill
    //it will detach the sharedMemory
    //This is needed mainly in ubuntu unity
    //Without this loging out prevents detaching sharedMemory
    Q_UNUSED(sig);
    sharedMemory.detach();
}

int main(int argc, char *argv[])
{
    Q_INIT_RESOURCE(omilo);

    QApplication a(argc, argv);
    QTranslator appTranslator;
    appTranslator.load("omilo_" + QLocale::system().name(), ":/translations");
    a.installTranslator(&appTranslator);

    //---- Check for another instance code snippet ----

    //GUID : Generated once for your application
    //you could get one GUID here: http://www.guidgenerator.com/online-guid-generator.aspx
    sharedMemory.setKey("4c8ff818-1a54-4861-8ab5-1233e76735d2");
    if(sharedMemory.attach())
    {
        //Do nothing;
        ;
    }

    if (!sharedMemory.create(1))
    {
        QMessageBox msgBox;
        msgBox.setText( QObject::tr("Application is already running") );
        msgBox.setIcon( QMessageBox::Critical );
        msgBox.exec();
        qWarning() << "Can't start more than one instance of the application.";
        exit(0);
    }

    signal(SIGSEGV, signalhandler);
    signal(SIGKILL, signalhandler);
    signal(SIGTERM, signalhandler);
    MainWindow w;
    //w.show();
    return a.exec();
}
