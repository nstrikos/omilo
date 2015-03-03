#include "speechengine.h"

SpeechEngine::SpeechEngine(QString voice)
{
    this->voice = voice;
    maryServerProcess = new QProcess();
    speechVoice = NULL;
    count = 1;
    isProcessing = false;
    setSpeechVoice(voice);
    maryTestingDownloadManager = new DownloadManager();
    textProcess = new TextProcess();
    splitMode = true;
}

SpeechEngine::~SpeechEngine()
{
    if (speechVoice != NULL)
    {
        delete this->speechVoice;
        speechVoice = NULL;
    }
    delete maryTestingDownloadManager;
    maryServerProcess->close();
    delete maryServerProcess;
    if (textProcess != NULL)
    {
        delete textProcess;
        textProcess = NULL;
    }
}

bool SpeechEngine::getIsProcessing()
{
    return isProcessing;
}

void SpeechEngine::speak(QString text)
{
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
    //if (!textList.isEmpty())
    if (!textContainer.text.isEmpty())
    {
        filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
        if ( count > maximumNumberOfFiles )
            count = 0;
        //this->text = textList.takeFirst();
        if (!textContainer.googleText.isEmpty())
            this->text = textContainer.googleText.takeFirst();
        if (!textContainer.googleBegin.isEmpty())
            this->begin = textContainer.googleBegin.takeFirst();
        if (!textContainer.googleEnd.isEmpty())
            this->end = textContainer.googleEnd.takeFirst();
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
    else if (sVoice == ItalianMary)
        createVoice(new ItalianMaryVoice());
    else if (sVoice == GermanMary)
        createVoice(new GermanMaryVoice());
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
}

SpeechVoice* SpeechEngine::getSpeechVoice()
{
    return speechVoice;
}

void SpeechEngine::startMaryProcess()
{
    maryServerProcess->start("java -showversion -ea -Xms40m -Xmx" + QString::number(maxMaryMemory) + "m -cp \"/usr/share/omilo-qt5/marytts-5.0/lib/*\" -Dmary.base=\"/usr/share/omilo-qt5/marytts-5.0\" marytts.server.Mary");
}

void SpeechEngine::voiceFileCreated(QString filename)
{
    isProcessing = false;
    if (filename != "/tmp/omilo.wav") // omilo.wav is used for checking mary server
        emit fileCreated(filename, currentSplitMode, this->begin, this->end);
    if (!textContainer.googleText.isEmpty())
        processList();
    else
        emit processingFinished();
}

void SpeechEngine::testMaryServer()
{
    maryTestingDownloadManager->performMaryTTSSpeak("/tmp/omilo-test.wav", "Welcome to omilo", "LOCALE=el&VOICE=emily-v2.0.1-hmm");
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

