#include "genericplayer.h"
#include <QDebug>

GenericPlayer::GenericPlayer()
{
    playProcess = new QProcess();
    connect(playProcess, SIGNAL(finished(int)), this, SLOT(play()));
}

GenericPlayer::~GenericPlayer()
{
    playlist.clear();
    playProcess->close();
    delete playProcess;
}

void GenericPlayer::play()
{
    if (!playlist.isEmpty() && playProcess->pid() == 0)
    {
        QString file = playlist.dequeue();
//        QString command = "mpg321 " + file;
        QString command = "mplayer " + file;
        playProcess->start(command.toLocal8Bit());
        emit currentIndexChanged();
        emit playing();
    }
    else if (playlist.isEmpty() && playProcess->pid() == 0)
        emit playingFinished();
}

void GenericPlayer::addToPlayList(QString item)
{
    qDebug() << "File added to generic player playlist:" << item;
    playlist.append(item);
}

void GenericPlayer::clearPlaylist()
{
    playlist.clear();
}

void GenericPlayer::stop()
{
    clearPlaylist();
    playProcess->close();
}
