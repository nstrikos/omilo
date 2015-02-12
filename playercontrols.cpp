#include "playercontrols.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
    , playerState(QMediaPlayer::StoppedState)
    , playerMuted(false)
    , playButton(0)
    , stopButton(0)
    , nextButton(0)
    , previousButton(0)
    , muteButton(0)
    , volumeSlider(0)
    , rateBox(0)
{
    playButton = new QToolButton(this);
    playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));

    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    stopButton = new QToolButton(this);
    stopButton->setIcon(style()->standardIcon(QStyle::SP_MediaStop));
    stopButton->setEnabled(false);

    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));

    nextButton = new QToolButton(this);
    nextButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipForward));

    connect(nextButton, SIGNAL(clicked()), this, SIGNAL(next()));

    previousButton = new QToolButton(this);
    previousButton->setIcon(style()->standardIcon(QStyle::SP_MediaSkipBackward));

    connect(previousButton, SIGNAL(clicked()), this, SIGNAL(previous()));

    muteButton = new QToolButton(this);
    muteButton->setIcon(style()->standardIcon(QStyle::SP_MediaVolume));

    connect(muteButton, SIGNAL(clicked()), this, SLOT(muteClicked()));

    volumeSlider = new QSlider(Qt::Horizontal, this);
    volumeSlider->setRange(0, 100);

    connect(volumeSlider, SIGNAL(sliderMoved(int)), this, SIGNAL(changeVolume(int)));

    rateBox = new QComboBox(this);
    rateBox->addItem("0.1x", QVariant(0.1));
    rateBox->addItem("0.2x", QVariant(0.2));
    rateBox->addItem("0.3x", QVariant(0.4));
    rateBox->addItem("0.4x", QVariant(0.4));
    rateBox->addItem("0.5x", QVariant(0.5));
    rateBox->addItem("0.6x", QVariant(0.6));
    rateBox->addItem("0.7x", QVariant(0.7));
    rateBox->addItem("0.8x", QVariant(0.8));
    rateBox->addItem("0.9x", QVariant(0.9));
    rateBox->addItem("1.0x", QVariant(1.0));
    rateBox->addItem("1.1x", QVariant(1.1));
    rateBox->addItem("1.2x", QVariant(1.2));
    rateBox->addItem("1.3x", QVariant(1.3));
    rateBox->addItem("1.4x", QVariant(1.4));
    rateBox->addItem("1.5x", QVariant(1.5));
    rateBox->addItem("1.6x", QVariant(1.6));
    rateBox->addItem("1.7x", QVariant(1.7));
    rateBox->addItem("1.8x", QVariant(1.8));
    rateBox->addItem("1.9x", QVariant(1.9));
    rateBox->addItem("2.0x", QVariant(2.0));
    rateBox->setCurrentIndex(9);

    connect(rateBox, SIGNAL(activated(int)), SLOT(updateRate()));
    connect(rateBox, SIGNAL(currentIndexChanged(int)), SLOT(updateRate()));

    QBoxLayout *layout = new QHBoxLayout;
    layout->setMargin(0);
    layout->addWidget(stopButton);
    layout->addWidget(previousButton);
    layout->addWidget(playButton);
    layout->addWidget(nextButton);
    layout->addWidget(muteButton);
    layout->addWidget(volumeSlider);
    layout->addWidget(rateBox);
    setLayout(layout);
}

QMediaPlayer::State PlayerControls::state() const
{
    return playerState;
}

void PlayerControls::setState(QMediaPlayer::State state)
{
    if (state != playerState) {
        playerState = state;

        switch (state) {
        case QMediaPlayer::StoppedState:
            stopButton->setEnabled(false);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        case QMediaPlayer::PlayingState:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPause));
            break;
        case QMediaPlayer::PausedState:
            stopButton->setEnabled(true);
            playButton->setIcon(style()->standardIcon(QStyle::SP_MediaPlay));
            break;
        }
    }
}

int PlayerControls::volume() const
{
    return volumeSlider ? volumeSlider->value() : 0;
}

void PlayerControls::setVolume(int volume)
{
    if (volumeSlider)
        volumeSlider->setValue(volume);
}

bool PlayerControls::isMuted() const
{
    return playerMuted;
}

void PlayerControls::setMuted(bool muted)
{
    if (muted != playerMuted) {
        playerMuted = muted;

        muteButton->setIcon(style()->standardIcon(muted
                ? QStyle::SP_MediaVolumeMuted
                : QStyle::SP_MediaVolume));
    }
}

void PlayerControls::playClicked()
{
    switch (playerState) {
    case QMediaPlayer::StoppedState:
    case QMediaPlayer::PausedState:
        emit play();
        break;
    case QMediaPlayer::PlayingState:
        emit pause();
        break;
    }
}

void PlayerControls::muteClicked()
{
    emit changeMuting(!playerMuted);
}

qreal PlayerControls::playbackRate() const
{
    return rateBox->itemData(rateBox->currentIndex()).toDouble();
}

void PlayerControls::setPlaybackRate(float rate)
{
    for (int i = 0; i < rateBox->count(); ++i) {
        if (qFuzzyCompare(rate, float(rateBox->itemData(i).toDouble()))) {
            rateBox->setCurrentIndex(i);
            return;
        }
    }

    rateBox->addItem(QString("%1x").arg(rate), QVariant(rate));
    rateBox->setCurrentIndex(rateBox->count() - 1);
}

void PlayerControls::updateRate()
{
    emit changeRate(playbackRate());
}

void PlayerControls::decreaseRate()
{
    if (rateBox->currentIndex() > 0)
        rateBox->setCurrentIndex(rateBox->currentIndex() - 1);
}

void PlayerControls::increaseRate()
{
    if (rateBox->currentIndex() < rateBox->count() - 1)
        rateBox->setCurrentIndex(rateBox->currentIndex() + 1);
}

float PlayerControls::getPlaybackRate()
{
    return rateBox->itemData(rateBox->currentIndex()).toDouble();
}
