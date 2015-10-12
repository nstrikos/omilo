#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include <QSplashScreen>
#include <QClipboard>
#include <QSystemTrayIcon>
#include "speechengine.h"
#include "editorvoiceoptionsdialog.h"
#include "installvoicesdialog.h"
#include "flitesettingsdialog.h"
#include "fontsettingsdialog.h"
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
    void seek(int seconds);
    void jump(const QModelIndex &index);
    void playlistPositionChanged(int);
    void statusChanged(QMediaPlayer::MediaStatus status);
    void bufferingProgress(int progress);
    void addToPlaylist(QString filename, bool split, unsigned int begin, unsigned int end);
    void displayErrorMessage();
    void updateMaryStatus();
    void cancel();
    void displayHelp();
    void displayAboutMessage();
    void play();
    void stop();
    void rateUp();
    void rateDown();
    void showFliteDialog();
    void setDefaultVoice();
    void checkInstalledVoice();
    void hotKeyPlayPressed();
    void hotKeyStopPressed();
    void speakSelectedText();
    void invertPalette();
    void showFontSettingsDialog();
    void setBold();
    void updateControlsWhenEngineIsIdle();
    void enableSplitMode();
    void toggleUseTrayIcon();
    void highlightSelection();
    void speakFromCurrentPosition();
    void setMaxId(int maxId);
    void setCurrentId(int id);
    void showExportFinishedMessage();
    void setMergeId(int id, int size);
    void setMergeInfo(QString info);

private:
    Ui::MainWindow *ui;

    void initFunctions();
    void connectSignalsToSlots();
    void createActions();
    void createMenus();
    void createTrayIcon();
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
    void speakTextWithoutSplitting(QString text);
    void updateVoiceLabel();
    void updateControlsWhenEngineIsProcessing();
    void removeTempFiles();
    void showMainWindow();
    void showHideTrayIcon();
    void setFliteControls();

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
    QAction *helpAction;
    QAction *aboutAction;
    QAction *playAction;
    QAction *stopAction;
    QAction *rateUpAction;
    QAction *rateDownAction;
    QAction *showFliteSettingsAction;
    QAction *speakSelectedTextAction;
    QAction *showFontSettingsDialogAction;
    QAction *boldAction;
    QAction *invertColorsAction;    
    QAction *enableSplitModeAction;
    QAction *toggleUseTrayIconAction;
    QAction *speakFromCurrentPositionAction;
    QAction *minimizeAction;
    QAction *maximizeAction;
    QAction *restoreAction;
    QAction *quitAction;

    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *speakMenu;
    QMenu *optionsMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *speakToolBar;

    SpeechEngine *engine;
    QString engineVoice; //variable to hold engine voice after reading from settings
    QString voice; //which of two should I keep???

    EditorVoiceOptionsDialog *editorVoiceOptionsDialog;
    InstallVoicesDialog *installVoicesDialog;

    QMediaPlayer *player;
    QMediaPlaylist *playlist;
    PlaylistModel *playlistModel;
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
    QLabel *percentStatusLabel;
    int maxId;
    int currentId;
    StartupThread *startUpThread;
    QSplashScreen *splashScreen;
    PlayerControls *controls;
    FliteSettingsDialog *fliteSettingsDialog;
    QTimer *maryStartupTimer;
    SpeechEngineInfo *engineInfo;
    HotKeyThread hotKeyThread;
    QClipboard *clipboard;
    QProcess soxProcess;
    QPalette defaultPalette;
    QPalette invertedPalette;
    QQueue<unsigned int> beginQueue;
    QQueue<unsigned int> endQueue;
    int beginBlock;
    int endBlock;
    bool splitMode;
    unsigned int cursorPosition;
    int fliteDuration;
    int fliteTargetMean;

    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    bool useTrayIcon;
    bool engineIsProcessing;
    QIcon speakIcon;
    QIcon cancelIcon;

    FontSettingsDialog *fontSettingsDialog;
};

#endif // MAINWINDOW_H

