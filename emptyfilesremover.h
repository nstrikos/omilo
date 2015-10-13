#ifndef EMPTYFILESREMOVER_H
#define EMPTYFILESREMOVER_H

#include "definitions.h"
#include <QQueue>
#include <QFile>


class EmptyFilesRemover
{
public:
    void remove(QQueue<TextProcessItem> *list);

private:
    QQueue<TextProcessItem> *list;
};

#endif // EMPTYFILESREMOVER_H
