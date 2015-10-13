#include "emptyfilesremover.h"
#include <QDebug>

void EmptyFilesRemover::remove(QQueue<TextProcessItem> *list)
{
    qDebug() << "Removing empty sound files...";

    this->list = list;

    for (int i = 0; i < list->size() - 1; i++)
    {
        QString filename = list->at(i).filename;
        int size = 0;
        QFile myFile(filename);
        if (myFile.open(QIODevice::ReadOnly)){
            size = myFile.size();
            myFile.close();
        }
        if (size < 4000)
        {
            qDebug() << "Removed " << filename << " with size " << size << "and text " << list->at(i).text;
            QFile::remove(filename);
        }
    }
    qDebug() << "Removing empty sound files finished.";
}

