#ifndef PLAYERCONTROLS_H
#define PLAYERCONTROLS_H

#include <QMediaPlayer>
#include <QWidget>
#include <QIcon>

QT_BEGIN_NAMESPACE
class QAbstractButton;
class QAbstractSlider;
class QComboBox;
QT_END_NAMESPACE

class PlayerControls : public QWidget
{
    Q_OBJECT

public:
    PlayerControls(QWidget *parent = 0);

    QMediaPlayer::State state() const;
    qreal playbackRate() const;
    void decreaseRate();
    void increaseRate();
    float getPlaybackRate();

public slots:
    void setState(QMediaPlayer::State state);
    void setPlaybackRate(float rate);
    void disablePlayButton();

signals:
    void play();
    void pause();
    void stop();
    void changeRate(qreal rate);

private slots:
    void playClicked();
    void updateRate();

private:
    QMediaPlayer::State playerState;
    QAbstractButton *playButton;
    QAbstractButton *stopButton;
    QComboBox *rateBox;
    QIcon playIcon;
    QIcon pauseIcon;
    QIcon stopIcon;
};

#endif // PLAYERCONTROLS_H
