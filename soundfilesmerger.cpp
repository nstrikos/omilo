#include "soundfilesmerger.h"
#include <QDebug>
#include <QFile>

SoundFilesMerger::SoundFilesMerger()
{
    qDebug() << "Creating new soundfilemerger class...";
    mergeCounter = 0;
    mergeCommand = "";
    mergeProcess = new QProcess;
    emptyFilesRemover = new EmptyFilesRemover();
    exportToWav = false;
    connect(mergeProcess, SIGNAL(finished(int)), this, SLOT(continueMerging()));
    connect(this, SIGNAL(soxFinished()), this, SLOT(finalMerge()));
    connect(&finalSoxProcess, SIGNAL(finished(int)), this, SIGNAL(exportFinished()));
    qDebug() << "Creating new soundfilemerger class completed.";
}

SoundFilesMerger::~SoundFilesMerger()
{
    mergeProcess->disconnect();
    mergeProcess->close();
    delete mergeProcess;
    delete emptyFilesRemover;
    finalSoxProcess.close();
}

void SoundFilesMerger::mergeSoundFiles(QQueue<TextProcessItem> *list, QString filename)
{
    this->list = list;
    this->filename = filename;
    removeEmptyFiles();
    startMerging();
}

void SoundFilesMerger::removeEmptyFiles()
{
    emptyFilesRemover->remove(list);
}

void SoundFilesMerger::startMerging()
{
    mergeCounter = 0;
    mergeCommand = "";
    soxFiles.clear();
    limit = 1000;
    QString filename = list->at(0).filename;
    overlap = false;
    QString command = "sox " + filename + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav";
    mergeProcess->start(command);
    qDebug() << "Merging started...";
    qDebug() << command;
}

void SoundFilesMerger::continueMerging()
{
    if (!overlap)
    {
        mergeCounter++;
        if (mergeCounter < limit && mergeCounter < list->size() )
        {
            if (QFile::exists(list->at(mergeCounter).filename))
            {
                mergeCommand += " " + list->at(mergeCounter).filename;
            }
            if (mergeCounter % 100 == 0)
            {
                QFile::remove("/tmp/omilo-exp-tmp.wav");
                QFile::rename("/tmp/omilo-exp-" + QString::number(limit)  + ".wav", "/tmp/omilo-exp-tmp.wav");
                QString command = "sox /tmp/omilo-exp-tmp.wav " + mergeCommand + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav";
                mergeProcess->start(command);
                emit mergeId(mergeCounter, list->size());
                mergeCommand = "";
                qDebug() << command;
            }
            else
                //Dummy process just to return to this function again
                //Because mergeProcess finished signal is connect to continueMerging function
                mergeProcess->start("echo foo");
        }
        else if (mergeCounter == limit || mergeCounter == list->size()  )
        {
            QFile::remove("/tmp/omilo-exp-tmp.wav");
            QFile::rename("/tmp/omilo-exp-" + QString::number(limit)  + ".wav", "/tmp/omilo-exp-tmp.wav");
            QString command = "sox /tmp/omilo-exp-tmp.wav " + mergeCommand + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav";
            soxFiles.enqueue("/tmp/omilo-exp-" + QString::number(limit)  + ".wav");
            overlap = true;
            mergeProcess->start(command);
            emit mergeId(mergeCounter, list->size());
            qDebug() << command;
        }
    }
    else
    {
        if (mergeCounter >= list->size())
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
                if (QFile::exists(list->at(mergeCounter).filename))
                    done = true;
                else
                    mergeCounter++;
            }
            mergeProcess->start("sox " + list->at(mergeCounter).filename + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav");
        }
    }
}

void SoundFilesMerger::finalMerge()
{
    mergeCommand = "";
    for (int i = 0; i < soxFiles.size(); i++)
    {
        mergeCommand += " " + soxFiles.at(i);
    }
    QString command = "sox " + mergeCommand + " " + filename;
    finalSoxProcess.start(command);
    emit mergeInfo(tr("Creating file..."));
    qDebug() << command;
}

