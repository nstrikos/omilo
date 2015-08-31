#include "playercontrols.h"

#include <QBoxLayout>
#include <QSlider>
#include <QStyle>
#include <QToolButton>
#include <QComboBox>

PlayerControls::PlayerControls(QWidget *parent)
    : QWidget(parent)
    , playerState(QMediaPlayer::StoppedState)
    , playButton(0)
    , rateBox(0)
{
    playButton = new QToolButton(this);
    playIcon = QIcon(":/images/play.png");
    pauseIcon = QIcon(":images/pause.png");
    stopIcon= QIcon(":images/stop.png");

    QSize size;
    size.setHeight(40);
    size.setWidth(40);

    playButton->setIconSize(size);
    playButton->setIcon(playIcon);
    playButton->setMinimumWidth(50);
    playButton->setMinimumHeight(50);
    playButton->setEnabled(false);

    connect(playButton, SIGNAL(clicked()), this, SLOT(playClicked()));

    stopButton = new QToolButton(this);
    stopButton->setIcon(stopIcon);
    stopButton->setEnabled(false);
    stopButton->setMinimumWidth(50);
    stopButton->setMinimumHeight(50);
    stopButton->setIconSize(size);

    connect(stopButton, SIGNAL(clicked()), this, SIGNAL(stop()));

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
    rateBox->setMaximumWidth(100);
    rateBox->setMinimumWidth(50);
    rateBox->setMinimumHeight(50);

    connect(rateBox, SIGNAL(activated(int)), SLOT(updateRate()));
    connect(rateBox, SIGNAL(currentIndexChanged(int)), SLOT(updateRate()));

    QBoxLayout *layout = new QHBoxLayout;
    layout->setSpacing(0);
    layout->setMargin(0);
    layout->addWidget(stopButton);
    layout->addWidget(playButton);
    layout->addItem( new QSpacerItem(20, 20, QSizePolicy::Fixed, QSizePolicy::Fixed) );
    layout->addWidget(rateBox);
    layout->setAlignment(Qt::AlignLeft);
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
        playButton->setEnabled(true);

        switch (state) {
        case QMediaPlayer::StoppedState:
            stopButton->setEnabled(false);
            playButton->setIcon(playIcon);
            break;
        case QMediaPlayer::PlayingState:
            stopButton->setEnabled(true);
            playButton->setIcon(pauseIcon);
            break;
        case QMediaPlayer::PausedState:
            stopButton->setEnabled(true);
            playButton->setIcon(playIcon);
            break;
        }
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

void PlayerControls::disablePlayButton()
{
    playButton->setDisabled(true);
}
