#include "festivalflitevoice.h"

FestivalFliteVoice::FestivalFliteVoice()
{
    createWavProcess = new QProcess();
    connect(createWavProcess, SIGNAL(finished(int)), this, SLOT(emitFileCreatedSignal()));
}

FestivalFliteVoice::~FestivalFliteVoice()
{
    createWavProcess->close();
    delete createWavProcess;
}

QString FestivalFliteVoice::getLanguage()
{
    return QLocale::languageToString(QLocale::English);
}

void FestivalFliteVoice::writeTextToTmpFile(QString text)
{
    //save text to /tmp/omilo.txt
    QFile outputFile(textFile);
    outputFile.open(QIODevice::WriteOnly);
    QTextStream outStream(&outputFile);
    outStream << text;
    outputFile.close();
}

void FestivalFliteVoice::cancel()
{
    cancelled = true;
    createWavProcess->close();
}
