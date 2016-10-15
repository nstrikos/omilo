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
    dialogue = false;
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
        filename = wavPrefix + QString::number(0) + ".wav";
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
        emit newMaxId(maxId);
        processList();
    }
}

void SpeechEngine::speakWithoutSplitting(QString text)
{

    splitMode = false;
    if (isProcessing == true)
        cancel();

    filename = wavPrefix + QString::number(0) + ".wav";
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

void SpeechEngine::processDialogue()
{
    if (dialogue == true) {
        if ( currentId <= maxId)
        {
            QString filename = filenames.at(currentId);
            QString text = texts.at(currentId);
            setSpeechVoice(voices.at(currentId));
            //speechVoice->setUseDurationStretch(false);
            //speechVoice->setUseTargetMean(false);
            setUseDurationStretch(false);
            setUseTargetMean(false);
            qDebug() << currentId << " a";
            speechVoice->performSpeak(filename, text);
        }
        else
        {
            dialogue = false;
            currentId = 0;
            emit processingFinished();
            emit dialogueFinished(voices, texts, filenames);
        }
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
        emit newMaxId(maxId + 1);
        processList();
    }
}

void SpeechEngine::makeDialogue(QList<QString> voices, QList<QString> texts, QList<QString> filenames)
{
    dialogue = true;
    if (isProcessing == true)
        cancel();

    this->voices.clear();
    this->texts.clear();
    this->filenames.clear();
    for (int i = 0; i < voices.size(); i++)
        this->voices.append(voices.at(i));
    for (int i = 0; i < texts.size(); i++)
        this->texts.append(texts.at(i));

    for (int i = 0; i < filenames.size(); i++)
        this->filenames.append(filenames.at(i));

    currentId = 0;
    maxId = texts.size() - 1;
    emit newMaxId(maxId + 1);
    processDialogue();
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
    filenames.clear();
    texts.clear();
    voices.clear();
    dialogue = false;
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
    qDebug() << currentId << " b";
    isProcessing = false;
    if (filename != "/tmp/omilo.wav") // omilo.wav is used for checking mary server
    {

        if (rate != 1)
        {
            QFile::remove(tempFile);
            QFile::rename(filename, tempFile);
            //QString command = "sox /tmp/omilo-tmp.wav " + filename + " tempo " + QString::number(rate);
            QString command = soxCommand +  " " + tempFile + " " + filename + " tempo " + QString::number(rate);
            qDebug() << command;
            soxProcess.start(command);
            soxProcess.waitForFinished();
        }
        if (!dialogue)
        {
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
        else
        {
            qDebug() << currentId << " b+";
            //emit fileCreated(filenames.at(currentId), splitMode, 0, 0);
            emit newId(currentId + 1);
            currentId++;
            processDialogue();
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
    else if (sVoice == CustomFestival)
        createVoice(new CustomFestivalVoice());
    else if (sVoice == RmsFlite)
        createVoice(new RmsFliteVoice());
    else if (sVoice == SltFlite)
        createVoice(new SltFliteVoice());
    else if (sVoice == AwbFlite)
        createVoice(new AwbFliteVoice());
    else if (sVoice == Kal16Flite)
        createVoice(new Kal16FliteVoice());
    else if (sVoice == IndicAupMrFlite)
        createVoice(new IndicAupMrFliteVoice());
    else if (sVoice == IndicAxbGuFlite)
        createVoice(new IndicAxbGuFliteVoice());
    else if (sVoice == IndicAxbHiFlite)
        createVoice(new IndicAxbHiFliteVoice());
    else if (sVoice == IndicKnrTeFlite)
        createVoice(new IndicKnrTeFliteVoice());
    else if (sVoice == IndicSksTaFlite)
        createVoice(new IndicSksTaFliteVoice());
    else if (sVoice == IndicSlpMrFlite)
        createVoice(new IndicSlpMrFliteVoice());
    else if (sVoice == IndicSxsHiFlite)
        createVoice(new IndicSxsHiFliteVoice());
    else if (sVoice == IndicSxvTaFlite)
        createVoice(new IndicSxvTaFliteVoice());
    else if (sVoice == AewFlite)
        createVoice(new AewFliteVoice());
    else if (sVoice == AhwFlite)
        createVoice(new AhwFliteVoice());
    else if (sVoice == AupFlite)
        createVoice(new AupFliteVoice());
    else if (sVoice == Awb2Flite)
        createVoice(new Awb2FliteVoice());
    else if (sVoice == AxbFlite)
        createVoice(new AxbFliteVoice());
    else if (sVoice == BdlFlite)
        createVoice(new BdlFliteVoice());
    else if (sVoice == ClbFlite)
        createVoice(new ClbFliteVoice());
    else if (sVoice == EeyFlite)
        createVoice(new EeyFliteVoice());
    else if (sVoice == FemFlite)
        createVoice(new FemFliteVoice());
    else if (sVoice == GkaFlite)
        createVoice(new GkaFliteVoice());
    else if (sVoice == JmkFlite)
        createVoice(new JmkFliteVoice());
    else if (sVoice == KspFlite)
        createVoice(new KspFliteVoice());
    else if (sVoice == LjmFlite)
        createVoice(new LjmFliteVoice());
    else if (sVoice == Rms2Flite)
        createVoice(new Rms2FliteVoice());
    else if (sVoice == RxrFlite)
        createVoice(new RxrFliteVoice());
    else if (sVoice == Slt2Flite)
        createVoice(new Slt2FliteVoice());
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
#ifndef Q_OS_WIN
    qDebug() << "Starting maryserver for Linux...";
    QString command = "java -showversion -ea -Xms40m -Xmx" + QString::number(maxMaryMemory) + "m -cp \"/usr/share/omilo-qt5/marytts-5.0/lib/*\" -Dmary.base=\"/usr/share/omilo-qt5/marytts-5.0\" marytts.server.Mary" ;
    maryServerProcess->start(command);
    qDebug() << "Start mary server : " << command;
#else
    qDebug() << "Starting Mary server for Windows...";
    maryServerForWindows.startMaryServer(maryServerProcess);
#endif
}

void SpeechEngine::testMaryServer()
{
    testVoice->performSpeak(testFile, "Welcome to omilo");
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

void SpeechEngine::setUseDurationStretch(bool useDurationStretch)
{
    this->useDurationStretch = useDurationStretch;
    speechVoice->setUseDurationStretch(useDurationStretch);
}

void SpeechEngine::setUseTargetMean(bool useTargetMean)
{
    this->useTargetMean = useTargetMean;
    speechVoice->setUseTargetMean(useTargetMean);
}

void SpeechEngine::setCustomFestivalCommand(QString command)
{
    this->customFestivalCommand = command;
    speechVoice->setCustomCommand(command);
}

void SpeechEngine::setCustomFestivalCommandArguments(QString arguments)
{
    this->customFestivalCommandArguments = arguments;
    speechVoice->setCustomCommandArguments(arguments);
}

void SpeechEngine::setSplitMode(bool mode)
{
    this->splitMode = mode;
}

void SpeechEngine::setRate(double rate)
{
    this->rate = rate;
}
