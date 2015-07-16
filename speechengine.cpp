#include "speechengine.h"

SpeechEngine::SpeechEngine(QString voice)
{
    this->voice = voice;
    maryServerProcess = new QProcess();
    speechVoice = NULL;
    count = 1;
    producedFiles = 0;
    spokenFiles = 0;
    googleAttempts = 0;
    isProcessing = false;
    setSpeechVoice(voice);
    maryTestingDownloadManager = new DownloadManager();
    textProcess = new TextProcess();
    splitMode = true;
    googleTimer = new QTimer();
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
    if (this->voice == GoogleEnglish || this->voice == GoogleGerman || this->voice == GoogleGreek)
        speak(text, true);
    else
    {

        currentSplitMode = splitMode;
        if (isProcessing == true)
            cancel();
        textContainer.clear();
        if (splitMode)
        {
            textProcess->setText(text);
            if (this->voice == GoogleEnglish || this->voice == GoogleGerman || this->voice == GoogleGreek)
                textProcess->processGoogleText();
            else
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
        if (this->voice == GoogleEnglish || this->voice == GoogleGerman || this->voice == GoogleGreek)
            textProcess->processGoogleText();
        else
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

    if (voice == GoogleEnglish || voice == GoogleGerman || voice == GoogleGreek)
    {
        if (!textContainer.googleText.isEmpty())
        {
            filename = "/tmp/omilo-" + QString::number(count++) + ".mp3";
            if ( count > maximumNumberOfFiles )
                count = 0;
            if (!textContainer.googleText.isEmpty())
                this->text = textContainer.googleText.at(0);
            if (!textContainer.googleBegin.isEmpty())
                this->begin = textContainer.googleBegin.at(0);
            if (!textContainer.googleEnd.isEmpty())
                this->end = textContainer.googleEnd.at(0);
            isProcessing = true;
            speechVoice->performSpeak(filename, this->text);
        }
    }
    else
    {
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
    else if (sVoice == GoogleEnglish)
        createVoice(new EnglishGoogleVoice());
    else if (sVoice == GoogleGerman)
        createVoice(new GermanGoogleVoice());
    else if (sVoice == GoogleGreek)
        createVoice(new GreekGoogleVoice());
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
    maryServerProcess->start("java -showversion -ea -Xms40m -Xmx" + QString::number(maxMaryMemory) + "m -cp \"/usr/share/omilo-qt5/marytts-5.0/lib/*\" -Dmary.base=\"/usr/share/omilo-qt5/marytts-5.0\" marytts.server.Mary");
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
            if (voice == GoogleEnglish || voice == GoogleGerman || voice == GoogleGreek)
            {
                googleAttempts++;
                qDebug() << "Maybe empty  file: " << filename;
                qDebug() << "Google attempt:" << googleAttempts;
                if (googleAttempts < 5)
                {
                    //                    speechVoice->performSpeak(this->filename, this->text);
                    //                    textContainer.googleText.prepend(this->text);
                    //                    textContainer.googleBegin.prepend(this->begin);
                    //                    textContainer.googleEnd.prepend(this->end);
                    processList();
                    return;
                }
                else
                {
                    googleAttempts = 0;
                    textContainer.googleText.dequeue();
                    textContainer.googleBegin.dequeue();
                    textContainer.googleEnd.dequeue();
                    emit fileCreated(filename, currentSplitMode, this->begin, this->end);
                }
            }
        }
        else
        {
            if (voice == GoogleEnglish || voice == GoogleGerman || voice == GoogleGreek)
            {
                textContainer.googleText.dequeue();
                textContainer.googleBegin.dequeue();
                textContainer.googleEnd.dequeue();
                emit fileCreated(filename, currentSplitMode, this->begin, this->end);
                googleAttempts = 0;
            }
            else
            {
                textContainer.text.dequeue();
                textContainer.begin.dequeue();
                textContainer.end.dequeue();
                emit fileCreated(filename, currentSplitMode, this->begin, this->end);
            }
        }
    }
    if (voice == GoogleEnglish || voice == GoogleGerman || voice == GoogleGreek)
    {
        if (!textContainer.googleText.isEmpty())
            //processList();
            //googleTimer->start(5000);
            if (producedFiles - spokenFiles < 12)
                //processList();
            {
                connect(googleTimer, SIGNAL(timeout()), this, SLOT(processList()));
                googleTimer->start(2000);
            }
            else
            {
                disconnect(googleTimer, SIGNAL(timeout()), this, SLOT(processList()));
            }

        else
            emit processingFinished();
    }
    else
    {
        if (!textContainer.text.isEmpty())
            processList();
        else
            emit processingFinished();
    }
}

void SpeechEngine::testMaryServer()
{
    maryTestingDownloadManager->performSpeak("/tmp/omilo-test.wav", "Welcome to omilo", "LOCALE=el&VOICE=emily-v2.0.1-hmm");
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
