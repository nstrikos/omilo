#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include <QListView>
#include <QSplashScreen>
#include <QClipboard>
#include <QFontComboBox>
#include <QSpinBox>
#include <QCheckBox>
#include "speechengine.h"
#include "editorvoiceoptionsdialog.h"
#include "installvoicesdialog.h"
#include "flitesettingsdialog.h"
#include "playercontrols.h"
#include "playlistmodel.h"
#include "startupthread.h"
#include "hotkeythread.h"

class QAction;
class QMenu;

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void contextMenuEvent(QContextMenuEvent *event);
    void closeEvent(QCloseEvent *event);

private slots:
    void newFile();
    void openFile();
    bool save();
    bool saveAs();
    void documentWasModified();
    void documentModified();
    void openRecentFile();
    void speakButtonPressed();
    void exportToWav();
    void selectVoice();
    void installVoices();
    void splashTimerExpired();
    void durationChanged(qint64 duration);
    void positionChanged(qint64 progress);
    void previous();
    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);
    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int progress);
    void addToPlaylist(QString filename, unsigned int begin, unsigned int end);
    void displayErrorMessage();
    void updateMaryStatus();
    void on_cancelButton_clicked();
    void displayAboutMessage();
    void play();
    void stop();
    void next();
    void volumeMute();
    void volumeUp();
    void volumeDown();
    void rateUp();
    void rateDown();
    void toggleSplash();
    void showFliteDialog();
    void setDefaultVoice();
    void checkInstalledVoice();
    void hotKeyPlayPressed();
    void hotKeyStopPressed();
    void speakSelectedText();
    void hotKeyShowWindowPressed();
    void fontChanged(const QString &arg1);
    void spinBoxValueChanged(int arg1);
    void invertPalette();
    void updateFont();
    void increasePointSize();
    void decreasePointSize();
    void showFontList();
    void updateControlsWhenEngineIsIdle();
    void test();

private:
    Ui::MainWindow *ui;

    void initFunctions();
    void connectSignalsToSlots();
    void createActions();
    void createMenus();
    void createToolBars();
    void readSettings();
    void setupSplashScreen();
    void setupMaryStartupTimer();
    void setupPlayer();
    void setupLayout();
    void initVariables();
    void writeSettings();
    bool maybeSave();
    bool saveFile(const QString &filename);
    void loadFile(const QString &filename);
    void setCurrentFile(const QString &filename);
    void updateRecentFileActions();
    QString strippedName(const QString &fileName);
    void speakText(QString text);
    void updateVoiceLabel();
    void updateControlsWhenEngineIsProcessing();
    void removeTempFiles();
    void writeVoiceToFile(QString string);
    void showWindowWithoutCloseButton();

    QString curfile;
    enum {MaxRecentFiles = 5};
    QStringList recentFiles;
    QAction *recentFileActions[MaxRecentFiles];
    QAction *separatorAction;
    QAction *newAction;
    QAction *openAction;
    QAction *saveAction;
    QAction *saveAsAction;
    QAction *exportToWavAction;
    QAction *exitAction;
    QAction *undoAction;
    QAction *redoAction;
    QAction *cutAction;
    QAction *copyAction;
    QAction *pasteAction;
    QAction *clearAction;
    QAction *selectAllAction;
    QAction *speakAction;
    QAction *cancelAction;
    QAction *voiceOptionAction;
    QAction *installVoicesAction;
    QAction *aboutAction;
    QAction *playAction;
    QAction *stopAction;
    QAction *previousAction;
    QAction *nextAction;
    QAction *volumeMuteAction;
    QAction *volumeUpAction;
    QAction *volumeDownAction;
    QAction *rateUpAction;
    QAction *rateDownAction;
    QAction *toggleSplashAction;
    QAction *showFliteSettingsAction;
    QAction *speakSelectedTextAction;
    QAction *hideWindowAction;
    QAction *boldAction;
    QAction *invertColorsAction;
    QAction *increasePointSizeAction;
    QAction *decreasePointSizeAction;
    QAction *showFontListAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *speakMenu;
    QMenu *optionsMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *fileToolBar;
    QToolBar *editToolBar;
    QToolBar *speakToolBar;
    QToolBar *optionsToolBar;
    QToolBar *viewToolBar;
    QToolBar *helpToolBar;    

    SpeechEngine *engine;
    QString engineVoice; //variable to hold engine voice after reading from settings
    EditorVoiceOptionsDialog *editorVoiceOptionsDialog;
    InstallVoicesDialog *installVoicesDialog;

    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    PlaylistModel *playlistModel;
    QAbstractItemView *playlistView;
    QSlider *slider;
    qint64 duration;
    void updateDurationInfo(qint64 currentInfo);
    void setTrackInfo(const QString &info);
    void setStatusInfo(const QString &info);
    void handleCursor(QMediaPlayer::MediaStatus status);
    QString trackInfo;
    QString statusInfo;
    QLabel *labelDuration;
    QLabel *historyLabel;
    QLabel *selectedVoiceLabel;
    QLabel *engineStatusLabel;
    StartupThread *startUpThread;
    QSplashScreen *splashScreen;
    PlayerControls *controls;
    bool splashScreenIsDisabled;
    FliteSettingsDialog *fliteSettingsDialog;
    QTimer *maryStartupTimer;
    QTimer *checkInstalledVoiceTimer;
    SpeechEngineInfo *engineInfo;
    QString voice;
    HotKeyThread hotKeyThread;
    QClipboard *clipboard;
    QProcess soxProcess;
    QFontComboBox *fontComboBox;
    QSpinBox *spinBox;
    QPalette defaultPalette;
    QPalette invertedPalette;
    QQueue<unsigned int> beginQueue;
    QQueue<unsigned int> endQueue;
    unsigned int beginBlock;
    unsigned int endBlock;
};

#endif // MAINWINDOW_H

