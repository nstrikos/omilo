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
    mergeCounter = 0;
    mergeCommand = "";
    mergeProcess = new QProcess;
    connect(mergeProcess, SIGNAL(finished(int)), this, SLOT(continueMerging()));
    connect(this, SIGNAL(soxFinished()), this, SLOT(finalMerge()));
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
    mergeProcess->disconnect();
    mergeProcess->close();
    delete mergeProcess;
}

bool SpeechEngine::getIsProcessing()
{
    return isProcessing;
}

void SpeechEngine::speak(QString text)
{

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
        mergeProcess->close();
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
        //??????????
        emit processingFinished();

        //Remove silenced files
        QQueue<QString> filelist;
        for (int i = 0; i < textProcess->list.size() - 1; i++)
        {
            QString filename = textProcess->list.at(i).filename;
            int size = 0;
            QFile myFile(filename);
            if (myFile.open(QIODevice::ReadOnly)){
                size = myFile.size();
                myFile.close();
            }
            if (size < 4000)
            {
                qDebug() << "Removed " << filename << " with size " << size << "and text " << textProcess->list.at(i).text;
                QFile::remove(filename);
            }
            else
            {
                filelist.enqueue(textProcess->list.at(i).filename);
            }
        }
        startMerging();
    }

}

//This code works perfect
//But can we improve it?
void SpeechEngine::startMerging()
{
    mergeCounter = 0;
    mergeCommand = "";
    soxFiles.clear();
    limit = 1000;
    QString filename = textProcess->list.at(0).filename;
    overlap = false;
    mergeProcess->start("sox " + filename + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav");
    qDebug() << "Merging started...";
}

void SpeechEngine::continueMerging()
{
    qDebug() << "continue merging";
    if (!overlap)
    {
        mergeCounter++;
        if (mergeCounter < limit && mergeCounter < textProcess->list.size() )
        {
            if (QFile::exists(textProcess->list.at(mergeCounter).filename))
            {
                mergeCommand += " " + textProcess->list.at(mergeCounter).filename;
            }
            //qDebug() << mergeCommand;
            if (mergeCounter % 100 == 0)
            {
                QFile::remove("/tmp/omilo-exp-tmp.wav");
                QFile::rename("/tmp/omilo-exp-" + QString::number(limit)  + ".wav", "/tmp/omilo-exp-tmp.wav");
                QString command = "sox /tmp/omilo-exp-tmp.wav " + mergeCommand + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav";
                mergeProcess->start(command);
                mergeCommand = "";
                qDebug() << command;
            }
            else
                mergeProcess->start("echo foo");
        }
        else if (mergeCounter == limit || mergeCounter == textProcess->list.size()  )
        {
            QFile::remove("/tmp/omilo-exp-tmp.wav");
            QFile::rename("/tmp/omilo-exp-" + QString::number(limit)  + ".wav", "/tmp/omilo-exp-tmp.wav");
            QString command = "sox /tmp/omilo-exp-tmp.wav " + mergeCommand + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav";
            soxFiles.enqueue("/tmp/omilo-exp-" + QString::number(limit)  + ".wav");
            overlap = true;
            mergeProcess->start(command);
            qDebug() << command;
            qDebug() << "Merging finished";
        }
    }
    else
    {
        if (mergeCounter >= textProcess->list.size())
        {
            QFile::remove("/tmp/omilo-exp-tmp.wav");
            emit soxFinished();
        }
        else
        {
            limit += 1000;
            overlap = false;
            mergeCommand = "";
            bool done = false;
            while (!done)
            {
                if (QFile::exists(textProcess->list.at(mergeCounter).filename))
                    done = true;
                else
                    mergeCounter++;
            }
            mergeProcess->start("sox " + textProcess->list.at(mergeCounter).filename + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav");
        }
    }
}

void SpeechEngine::finalMerge()
{
    mergeCommand = "";
    for (int i = 0; i < soxFiles.size(); i++)
    {
        mergeCommand += " " + soxFiles.at(i);
    }
    QString command = "sox " + mergeCommand + " /home/nick/omilo-exp.wav";
    finalSoxProcess.start(command);
    qDebug() << command;
}

//void SpeechEngine::continueMerging()
//{
//    qDebug() << "continue merging";
//    if (!overlap)
//    {
//        mergeCounter++;
//        if (mergeCounter < textProcess->list.size())
//        {
//            if (QFile::exists(textProcess->list.at(mergeCounter).filename))
//            {
//                mergeCommand += " " + textProcess->list.at(mergeCounter).filename;
//            }
//            qDebug() << mergeCommand;
//            if (mergeCounter % 100 == 0)
//            {
//                QFile::remove("/home/nick/omilo-tmp.wav");
//                QFile::rename("/home/nick/omilo-exp.wav", "/home/nick/omilo-tmp.wav");
//                QString command = "sox /home/nick/omilo-tmp.wav " + mergeCommand + " /home/nick/omilo-exp.wav";
//                mergeProcess->start(command);
//                mergeCommand = "";
//                qDebug() << command;
//            }
//            else
//                mergeProcess->start("echo foo");
//        }
//        else if (mergeCounter == textProcess->list.size() )
//        {
//            QFile::remove("/home/nick/omilo-tmp.wav");
//            QFile::rename("/home/nick/omilo-exp.wav", "/home/nick/omilo-tmp.wav");
//            QString command = "sox /home/nick/omilo-tmp.wav " + mergeCommand + " /home/nick/omilo-exp.wav";
//            overlap = true;
//            mergeProcess->start(command);
//            qDebug() << command;
//            qDebug() << "Merging finished";
//        }
//    }
//    else
//    {
//        QFile::remove("/home/nick/omilo-tmp.wav");
//    }
//}

void SpeechEngine::exportWav(QString filename, QString text)
{
    this->filename = filename;
    speechVoice->performSpeak(filename, text);
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
        if (splitMode)
        {
            emit fileCreated(filename, splitMode, textProcess->list.at(currentId).begin, textProcess->list.at(currentId).end);
            emit newId(currentId);
            currentId++;
            processList();
        }
        else
        {
            emit fileCreated(filename, splitMode, 0, 0);
            emit processingFinished();
        }

    }
    //emit processingFinished();

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

//void SpeechEngine::setPlaylist(QMediaPlaylist* playlist)
//{
//    this->playlist = playlist;
//    connect(this->playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(filePlayed()));
//}

//void SpeechEngine::filePlayed()
//{
//    //    spokenFiles++;
//    processList();
//}

//void SpeechEngine::voiceFileCreated(QString filename)
//{
//    isProcessing = false;
//    producedFiles++;
//    if (filename != "/tmp/omilo.wav") // omilo.wav is used for checking mary server
//    {
//        int size = 0;
//        QFile myFile(filename);
//        if (myFile.open(QIODevice::ReadOnly)){
//            size = myFile.size();
//            myFile.close();
//        }
//        if (size < 2300)
//        {

//        }
//        else
//        {

//        {
//                if (!textContainer.text.isEmpty())
//                    textContainer.text.dequeue();
//                if (!textContainer.begin.isEmpty())
//                    textContainer.begin.dequeue();
//                if (!textContainer.end.isEmpty())
//                    textContainer.end.dequeue();
//                emit fileCreated(filename, currentSplitMode, this->begin, this->end);
//            emit fileCreated(filename, false, 0, 0);

//        }
//}
//    }

//    {
//        if (!textContainer.text.isEmpty())
//            processList();
//        else
//        emit processingFinished();
//    }
//}

//void SpeechEngine::processList()
//{
//    //    if (!textContainer.text.isEmpty())
//    {
//        filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
//        if ( count > maximumNumberOfFiles )
//            count = 0;
//        //        if (voice == GoogleEnglish || voice == GoogleGerman || voice == GoogleGreek)
//        //        {
//        //            if (!textContainer.googleText.isEmpty())
//        //                this->text = textContainer.googleText.takeFirst();
//        //            if (!textContainer.googleBegin.isEmpty())
//        //                this->begin = textContainer.googleBegin.takeFirst();
//        //            if (!textContainer.googleEnd.isEmpty())
//        //                this->end = textContainer.googleEnd.takeFirst();
//        //        }
//        //        else
//        {
//            //            if (!textContainer.text.isEmpty())
//            //                this->text = textContainer.text.takeFirst();
//            //            if (!textContainer.begin.isEmpty())
//            //                this->begin = textContainer.begin.takeFirst();
//            //            if (!textContainer.end.isEmpty())
//            //                this->end = textContainer.end.takeFirst();
//        }
//        isProcessing = true;
//        speechVoice->performSpeak(filename, this->text);
//    }



//    //    if (!textContainer.text.isEmpty())
//    {

//        filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
//        if ( count > maximumNumberOfFiles )
//            count = 0;
//        //        if (!textContainer.text.isEmpty())
//        //            this->text = textContainer.text.takeFirst();
//        //            this->text = textContainer.text.at(0);
//        //        if (!textContainer.begin.isEmpty())
//        //            this->begin = textContainer.begin.at(0);
//        //        if (!textContainer.end.isEmpty())
//        //            this->end = textContainer.end.at(0);
//        isProcessing = true;
//        speechVoice->performSpeak(filename, this->text);
//    }


//}

//void SpeechEngine::speak(QString text)
//{
//    spokenFiles = 0;
//    producedFiles = 0;

//    currentSplitMode = splitMode;
//    if (isProcessing == true)
//        cancel();
//    textContainer.clear();
//    if (splitMode)
//    {
//        textProcess->setText(text);
//        textProcess->processText();
//        textContainer = textProcess->getTextContainer();
//        count = 1;
//        processList();
//    }
//    else
//    {
//        filename = "/tmp/omilo-" + QString::number(count++) + ".wav";
//        if ( count > maximumNumberOfFiles )
//            count = 1;
//        this->text = text;
//        isProcessing = true;
//        speechVoice->performSpeak(filename, text);
//    }
//}
