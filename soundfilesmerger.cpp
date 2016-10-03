#include "soundfilesmerger.h"
#include "definitions.h"
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
    connect(mergeProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleMergeProcessError()));
    connect(this, SIGNAL(soxFinished()), this, SLOT(finalMerge()));
    connect(&finalSoxProcess, SIGNAL(finished(int)), this, SIGNAL(exportFinished()));
    connect(&finalSoxProcess, SIGNAL(error(QProcess::ProcessError)), this, SLOT(handleFinalSoxProcessError()));
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
    QString command = soxCommand + " " + filename + " " + expPrefix + QString::number(limit)  + ".wav";
    mergeProcess->start(command);
    qDebug() << "Merging started...";
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
                QFile::remove(expTempFile);
                QFile::rename(expPrefix + QString::number(limit)  + ".wav", expTempFile);
                //QString command = "sox /tmp/omilo-exp-tmp.wav " + mergeCommand + " " + "/tmp/omilo-exp-" + QString::number(limit)  + ".wav";
                QString command = soxCommand + " " + expTempFile + " " + mergeCommand + " " + expPrefix + QString::number(limit) + ".wav";
                mergeProcess->start(command);
                emit mergeId(mergeCounter, list->size());
                mergeCommand = "";
            }
            else
                //Dummy process just to return to this function again
                //Because mergeProcess finished signal is connect to continueMerging function
                mergeProcess->start(dummyCommand);
        }
        else if (mergeCounter == limit || mergeCounter == list->size()  )
        {
            QFile::remove(expTempFile);
            QFile::rename(expPrefix + QString::number(limit)  + ".wav", expTempFile);
            QString command = soxCommand + " " + expTempFile + " " + mergeCommand + " " + expPrefix + QString::number(limit)  + ".wav";
            soxFiles.enqueue(expPrefix + QString::number(limit)  + ".wav");
            overlap = true;
            mergeProcess->start(command);
            emit mergeId(mergeCounter, list->size());
        }
    }
    else
    {
        if (mergeCounter >= list->size())
        {
            QFile::remove(expTempFile);
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
            mergeProcess->start(soxCommand + " " + list->at(mergeCounter).filename + " " + expPrefix + QString::number(limit)  + ".wav");
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
    QString command = soxCommand + " " + mergeCommand + " " + filename;
    finalSoxProcess.start(command);
    qDebug() << command;
    emit mergeInfo(tr("Creating file..."));
}

void SoundFilesMerger::handleMergeProcessError()
{
    qDebug() << "Error in merging: " << mergeProcess->exitStatus();
}

void SoundFilesMerger::handleFinalSoxProcessError()
{
    qDebug() << "Error in final merging: " << finalSoxProcess.exitStatus();
}
