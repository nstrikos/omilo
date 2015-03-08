#ifndef GENERICPLAYER_H
#define GENERICPLAYER_H

#include <QObject>
#include <QQueue>
#include <QProcess>

class GenericPlayer : public QObject
{
    Q_OBJECT

public:
    GenericPlayer();
    ~GenericPlayer();
    void addToPlayList(QString item);

public slots:
    void play();
    void stop();

private:
    void clearPlaylist();
    QQueue<QString> playlist;
    QProcess *playProcess;

signals:
    void currentIndexChanged();
    void playing();
    void playingFinished();
};

#endif // GENERICPLAYER_H
