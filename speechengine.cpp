#include "speechengine.h"

SpeechEngine::SpeechEngine(QString voice)
{
    this->voice = voice;
    maryServerProcess = new QProcess();
    speechVoice = NULL;
    testVoice = new GreekGoogleMaryVoice();
    count = 1;
    producedFiles = 0;
    spokenFiles = 0;
    googleAttempts = 0;
    isProcessing = false;
    setSpeechVoice(voice);
    maryTestingDownloadManager = new DownloadManager();
    textProcess = new TextProcess();
    splitMode = false;
    googleTimer = new QTimer();
}

SpeechEngine::~SpeechEngine()
{
    if (speechVoice != NULL)
    {
        delete this->speechVoice;
        speechVoice = NULL;
    }
    if (testVoice != NULL)
    {
        delete this->testVoice;
        testVoice = NULL;
    }
    delete maryTestingDownloadManager;
    maryServerProcess->close();
    delete maryServerProcess;
    if (textProcess != NULL)
    {
        delete textProcess;
        textProcess = NULL;
    }
    delete googleTimer;
}

bool SpeechEngine::getIsProcessing()
{
    return isProcessing;
}

void SpeechEngine::speak(QString text)
{
    spokenFiles = 0;
    producedFiles = 0;
    googleAttempts = 0;

    currentSplitMode = splitMode;
    if (isProcessing == true)
        cancel();
    textContainer.clear();
    if (splitMode)
    {
        textProcess->setText(text);
        textProcess->processText();
        textContainer = textProcess->getTextContainer();
        count = 1;
        processList();
    }
    else
    {
        filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
        if ( count > maximumNumberOfFiles )
            count = 1;
        this->text = text;
        isProcessing = true;
        speechVoice->performSpeak(filename, text);
    }
}

void SpeechEngine::speak(QString text, bool splitMode)
{
    spokenFiles = 0;
    producedFiles = 0;
    googleAttempts = 0;
    currentSplitMode = splitMode;
    if (isProcessing == true)
        cancel();
    textContainer.clear();
    if (splitMode)
    {
        textProcess->setText(text);
        textProcess->processText();
        textContainer = textProcess->getTextContainer();
        count = 1;
        processList();
    }
    else
    {
        filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
        if ( count > maximumNumberOfFiles )
            count = 1;
        this->text = text;
        isProcessing = true;
        speechVoice->performSpeak(filename, text);
    }
}

void SpeechEngine::processList()
{
    //    if (!textContainer.text.isEmpty() && !textContainer.googleText.isEmpty())
    //    {
    //        filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
    //        if ( count > maximumNumberOfFiles )
    //            count = 0;
    //        if (voice == GoogleEnglish || voice == GoogleGerman || voice == GoogleGreek)
    //        {
    //            if (!textContainer.googleText.isEmpty())
    //                this->text = textContainer.googleText.takeFirst();
    //            if (!textContainer.googleBegin.isEmpty())
    //                this->begin = textContainer.googleBegin.takeFirst();
    //            if (!textContainer.googleEnd.isEmpty())
    //                this->end = textContainer.googleEnd.takeFirst();
    //        }
    //        else
    //        {
    //            if (!textContainer.text.isEmpty())
    //                this->text = textContainer.text.takeFirst();
    //            if (!textContainer.begin.isEmpty())
    //                this->begin = textContainer.begin.takeFirst();
    //            if (!textContainer.end.isEmpty())
    //                this->end = textContainer.end.takeFirst();
    //        }
    //        isProcessing = true;
    //        speechVoice->performSpeak(filename, this->text);
    //    }



        if (!textContainer.text.isEmpty())
        {

            filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
            if ( count > maximumNumberOfFiles )
                count = 0;
            if (!textContainer.text.isEmpty())
                //            this->text = textContainer.text.takeFirst();
                this->text = textContainer.text.at(0);
            if (!textContainer.begin.isEmpty())
                this->begin = textContainer.begin.at(0);
            if (!textContainer.end.isEmpty())
                this->end = textContainer.end.at(0);
            isProcessing = true;
            speechVoice->performSpeak(filename, this->text);
        }


}

void SpeechEngine::exportWav(QString filename, QString text)
{
    this->filename = filename;
    speechVoice->performSpeak(filename, text);
}

void SpeechEngine::cancel()
{
    if (isProcessing == true)
    {
        count--;
        speechVoice->cancel();
        isProcessing = false;
    }
    textContainer.clear();
    disconnect(googleTimer, SIGNAL(timeout()), this, SLOT(processList()));
}

void SpeechEngine::createVoice(SpeechVoice *sVoice)
{
    if (speechVoice != NULL)
    {
        delete this->speechVoice;
        speechVoice = NULL;
    }
    this->speechVoice = sVoice;
    connect(speechVoice, SIGNAL(fileCreated(QString)), this, SLOT(voiceFileCreated(QString)));

    if (maryServerProcess->pid()  == 0)
        startMaryProcess();
}

//We need a factory here
void SpeechEngine::setSpeechVoice(QString sVoice)
{
    if (sVoice == KalFestival)
        createVoice(new KalDiphoneFestivalVoice());
    else if (sVoice == AwbCmuFestival)
        createVoice(new AwbCmuFestivalVoice());
    else if (sVoice == BdlCmuFestival)
        createVoice(new BdlCmuFestivalVoice());
    else if (sVoice == ClbCmuFestival)
        createVoice(new ClbCmuFestivalVoice());
    else if (sVoice == JmkCmuFestival)
        createVoice(new JmkCmuFestivalVoice());
    else if (sVoice == KspCmuFestival)
        createVoice(new KspCmuFestivalVoice());
    else if (sVoice == RmsCmuFestival)
        createVoice(new RmsCmuFestivalVoice());
    else if (sVoice == SltCmuFestival)
        createVoice(new SltCmuFestivalVoice());
    else if (sVoice == RmsFlite)
        createVoice(new RmsFliteVoice());
    else if (sVoice == SltFlite)
        createVoice(new SltFliteVoice());
    else if (sVoice == AwbFlite)
        createVoice(new AwbFliteVoice());
    else if (sVoice == Kal16Flite)
        createVoice(new Kal16FliteVoice());
    else if (sVoice == FemFlite)
        createVoice(new FemFliteVoice());
    else if (sVoice == JmkFlite)
        createVoice(new JmkFliteVoice());
    else if (sVoice == RxrFlite)
        createVoice(new RxrFliteVoice());
    else if (sVoice == ItalianMary)
        createVoice(new ItalianMaryVoice());
    else if (sVoice == GermanMary)
        createVoice(new GermanMaryVoice());
    else if (sVoice == GreekHercules)
        createVoice(new GreekHerculesVoice());
    else if (sVoice == GreekMary)
        createVoice(new GreekMaryVoice());
    else if (sVoice == GreekGoogleMary)
        createVoice(new GreekGoogleMaryVoice());
    else if (sVoice == TurkishMary)
        createVoice(new TurkishMaryVoice());
    else if (sVoice == FrenchMary)
        createVoice(new FrenchMaryVoice());
    else if (sVoice == CmuRmsMary)
        createVoice(new CmuRmsMaryVoice());
    else if (sVoice == CmuSltMary)
        createVoice(new CmuSltMaryVoice());
    else if (sVoice == ObadiahMary)
        createVoice(new ObadiahMaryVoice());
    else if (sVoice == PoppyMary)
        createVoice(new PoppyMaryVoice());
    else if (sVoice == PrudenceMary)
        createVoice(new PrudenceMaryVoice());
    else if (sVoice == SpikeMary)
        createVoice(new SpikeMaryVoice());
    else
        createVoice(new KalDiphoneFestivalVoice());
    this->voice = sVoice;
}

SpeechVoice* SpeechEngine::getSpeechVoice()
{
    return speechVoice;
}

void SpeechEngine::startMaryProcess()
{
    //QString command = javaExecPath + " -showversion -ea -Xms40m -Xmx" + QString::number(maxMaryMemory) + "m -cp \"" + maryPath + "/lib/*\" -Dmary.base=\"" + maryPath + "\" marytts.server.Mary";

    //qDebug() << command;

    maryServerProcess->start("java -showversion -ea -Xms40m -Xmx" + QString::number(maxMaryMemory) + "m -cp \"/usr/share/omilo-qt5/marytts-5.0/lib/*\" -Dmary.base=\"/usr/share/omilo-qt5/marytts-5.0\" marytts.server.Mary");
    //maryServerProcess->start(command);

    //maryServerProcess->start(javaExecPath + "-showversion -ea -Xms40m -Xmx" + QString::number(maxMaryMemory) + "m -cp \"/usr/share/omilo-qt5/marytts-5.0/lib/*\" -Dmary.base=\"/usr/share/omilo-qt5/marytts-5.0\" marytts.server.Mary");
}

void SpeechEngine::voiceFileCreated(QString filename)
{
    googleTimer->disconnect();
    isProcessing = false;
    producedFiles++;
    if (filename != "/tmp/omilo.wav") // omilo.wav is used for checking mary server
    {
        int size = 0;
        QFile myFile(filename);
        if (myFile.open(QIODevice::ReadOnly)){
            size = myFile.size();
            myFile.close();
        }
        if (size < 2300)
        {

        }
        else
        {

            {
                if (!textContainer.text.isEmpty())
                    textContainer.text.dequeue();
                if (!textContainer.begin.isEmpty())
                    textContainer.begin.dequeue();
                if (!textContainer.end.isEmpty())
                    textContainer.end.dequeue();
                emit fileCreated(filename, currentSplitMode, this->begin, this->end);
            }
        }
    }

    {
        if (!textContainer.text.isEmpty())
            processList();
        else
            emit processingFinished();
    }
}

void SpeechEngine::testMaryServer()
{
    //maryTestingDownloadManager->performSpeak("/tmp/omilo-test.wav", "Welcome to omilo", "LOCALE=el&VOICE=emily-v2.0.1-hmm");
    testVoice->performSpeak("/tmp/omilo-test.wav", "Welcome to omilo");
}

void SpeechEngine::stopTestingMaryServer()
{
    //maryTestingDownloadManager->cancelDownload();
    delete testVoice;
    testVoice = NULL;
}

void SpeechEngine::setDurationStretch(unsigned int duration)
{
    durationStretch = duration;
    speechVoice->setDurationStretch(durationStretch);
}

void SpeechEngine::setTargetMean(unsigned int target)
{
    targetMean = target;
    speechVoice->setTargetMean(targetMean);
}

void SpeechEngine::setSplitMode(bool mode)
{
    //this->splitMode = mode;
    this->splitMode = false;
}

void SpeechEngine::setPlaylist(QMediaPlaylist* playlist)
{
    this->playlist = playlist;
    connect(this->playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(filePlayed()));
}

void SpeechEngine::filePlayed()
{
    spokenFiles++;
    processList();
}
