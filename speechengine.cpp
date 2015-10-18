#include "speechengine.h"

SpeechEngine::SpeechEngine(QString voice)
{
    qDebug() << "Creating new speech engine...";
    this->voice = voice;
    maryServerProcess = new QProcess();
    speechVoice = NULL;
    testVoice = new GreekGoogleMaryVoice();
    splitMode = false;
    isProcessing = false;
    setSpeechVoice(voice);
    textProcess = new TextProcess();
    currentId = 0;
    maxId = 0;
    exportToWav = false;
    soundFilesMerger = new SoundFilesMerger();
    connect(soundFilesMerger, SIGNAL(mergeId(int,int)), this, SIGNAL(mergeId(int,int)));
    connect(soundFilesMerger, SIGNAL(exportFinished()), this, SIGNAL(exportFinished()));
    connect(soundFilesMerger, SIGNAL(mergeInfo(QString)), this, SIGNAL(mergeInfo(QString)));
    qDebug() << "Creating new speech engine completed.";
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
    maryServerProcess->close();
    delete maryServerProcess;

    if (textProcess != NULL)
    {
        delete textProcess;
        textProcess = NULL;
    }
    if (soundFilesMerger != NULL)
        delete soundFilesMerger;
}

bool SpeechEngine::getIsProcessing()
{
    return isProcessing;
}

void SpeechEngine::speak(QString text)
{

    exportToWav = false;
    if (isProcessing == true)
        cancel();

    if (splitMode == false)
    {
        filename = "/tmp/omilo-" + QString::number(0) + ".wav";
        //if ( count > maximumNumberOfFiles )
        //    count = 1;
        this->text = text;
        isProcessing = true;
        speechVoice->performSpeak(filename, text);
    }
    else
    {
        textProcess->setText(text);
        textProcess->processText();
        currentId = 0;
        maxId = textProcess->list.size() - 1;
        qDebug() << "Maxid:" << maxId;
        emit newMaxId(maxId);
        processList();
    }
}

void SpeechEngine::speakWithoutSplitting(QString text)
{

    splitMode = false;
    if (isProcessing == true)
        cancel();

    filename = "/tmp/omilo-" + QString::number(0) + ".wav";
    this->text = text;
    isProcessing = true;
    speechVoice->performSpeak(filename, text);
}

void SpeechEngine::processList()
{

    if ( currentId <= maxId)
    {
        QString filename = textProcess->list.at(currentId).filename;
        QString text = textProcess->list.at(currentId).text;
        speechVoice->performSpeak(filename, text);
    }
    else
    {
        emit processingFinished();
        if (exportToWav)
            soundFilesMerger->mergeSoundFiles(&textProcess->list, filename);
    }

}

void SpeechEngine::exportWav(QString filename, QString text)
{
    //    this->filename = filename;
    //    exportToWav = true;
    //    speechVoice->performSpeak(filename, text);
    exportToWav = true;
    if (isProcessing == true)
        cancel();

    if (splitMode == false)
    {
        this->filename = filename;
        //if ( count > maximumNumberOfFiles )
        //    count = 1;
        this->text = text;
        isProcessing = true;
        speechVoice->performSpeak(filename, text);
    }
    else
    {

        this->filename = filename;
        textProcess->setText(text);
        textProcess->processText();
        currentId = 0;
        maxId = textProcess->list.size() - 1;
        //mergeProcess->close();
        qDebug() << "Maxid:" << maxId;
        emit newMaxId(maxId);
        processList();
    }
}

void SpeechEngine::cancel()
{
    //    if (isProcessing == true)
    //    {
    qDebug() << "Engine is processing... Trying to cancel processing";
    //count--;
    speechVoice->cancel();
    isProcessing = false;
    textProcess->list.clear();
    currentId = 0;
    maxId = 0;
    //    }
    //    textContainer.clear();
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

void SpeechEngine::voiceFileCreated(QString filename)
{
    isProcessing = false;
    if (filename != "/tmp/omilo.wav") // omilo.wav is used for checking mary server
    {

        if (rate != 1)
        {
            QFile::remove("/tmp/omilo-tmp.wav");
            QFile::rename(filename, "/tmp/omilo-tmp.wav");
            QString command = "sox /tmp/omilo-tmp.wav " + filename + " tempo " + QString::number(rate);
            soxProcess.start(command);
            soxProcess.waitForFinished();
        }

        if (splitMode)
        {
            if (!exportToWav)
                emit fileCreated(filename, splitMode, textProcess->list.at(currentId).begin, textProcess->list.at(currentId).end);
            emit newId(currentId);
            currentId++;
            processList();
        }
        else
        {
            if (!exportToWav)
                emit fileCreated(filename, splitMode, 0, 0);
            if (exportToWav)
                emit exportFinished();
            exportToWav = false;
            emit processingFinished();
        }

    }
}

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
    else if (sVoice == CmuBdlMary)
        createVoice(new CmuBdlMaryVoice());
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
    QString command = "java -showversion -ea -Xms40m -Xmx" + QString::number(maxMaryMemory) + "m -cp \"/usr/share/omilo-qt5/marytts-5.0/lib/*\" -Dmary.base=\"/usr/share/omilo-qt5/marytts-5.0\" marytts.server.Mary" ;
    maryServerProcess->start(command);
    qDebug() << "Start mary server : " << command;
}

void SpeechEngine::testMaryServer()
{
    testVoice->performSpeak("/tmp/omilo-test.wav", "Welcome to omilo");
}

void SpeechEngine::stopTestingMaryServer()
{
    delete testVoice;
    testVoice = NULL;
    qDebug() << "Stop testing mary server...";
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
    this->splitMode = mode;
}

void SpeechEngine::setExportToWav(bool value)
{
    exportToWav = value;
}

void SpeechEngine::setRate(double rate)
{
    this->rate = rate;
}
