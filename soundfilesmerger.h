#ifndef SOUNDFILESMERGER_H
#define SOUNDFILESMERGER_H

#include <QObject>
#include <QProcess>
#include <textprocess.h>
#include "emptyfilesremover.h"


class SoundFilesMerger : public QObject
{
    Q_OBJECT

public:
    SoundFilesMerger();
    ~SoundFilesMerger();
    void mergeSoundFiles(QQueue<TextProcessItem> *list, QString filename);

private:
    EmptyFilesRemover *emptyFilesRemover;
    QString filename;
    int mergeCounter;
    QString mergeCommand;
    QProcess *mergeProcess;
    QProcess finalSoxProcess;
    TextProcess *textProcess;
    bool overlap;
    int limit;
    QQueue<QString> soxFiles;
    bool exportToWav;
    QQueue<TextProcessItem> *list;

    void removeEmptyFiles();
    void startMerging();

private slots:
    void continueMerging();
    void finalMerge();

signals:
    void soxFinished();
    void exportFinished();
    void mergeId(int id, int size);
    void mergeInfo(QString info);

public slots:
};

#endif // SOUNDFILESMERGER_H
