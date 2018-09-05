#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QLabel>
#include <QClipboard>
#include <QSystemTrayIcon>
#include <QClipboard>
#include "speechengine.h"
#include "editorvoiceoptionsdialog.h"
#include "installvoicesdialog.h"
#include "dialoguewindow.h"
#include "flitesettingsdialog.h"
#include "fontsettingsdialog.h"
#include "playercontrols.h"
#include "playlistmodel.h"
#include "startupthread.h"

#include "textprocess.h"
#include "soundfilesmerger.h"

#ifdef Q_OS_WIN
#include "hotkeythreadforwindows.h"
#else
#include "hotkeythread.h"
#endif

#include "tempfilesremover.h"
#include "settingswriter.h"
#include "exportprogressdialog.h"
#include "displaymessagedialog.h"
#include "customfestivaldialog.h"

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
    void showCustomFestivalDialog();
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
    void showNormalAndRaise();
    void showDialogueWindow();
    void dialogueFinished(QList<QString> voices, QList<QString> texts, QList<QString> filenames);
    void clearPlaylist();
    void handleClipboard();
    void setUseClipboard();

private:
    Ui::MainWindow *ui;

    //Private functions
    void initFunctions();
    void connectSignalsToSlots();
    void createActions();
    void createMenus();
    void createTrayIcon();
    void readSettings();
    void setupPlayer();
    void setupLayout();
    void initVariables();
    bool maybeSave();
    bool saveFile(const QString &filename);
    void loadFile(const QString &filename);
    void setCurrentFile(const QString &filename);
    void updateRecentFileActions();
    QString strippedName(const QString &fileName);
    void speakText(QString text);
    void speakTextWithoutHighlight(QString text);
    void setVariablesBeforeSpeaking();
    void updateVoiceLabel();
    void updateControlsWhenEngineIsProcessing();
    void showMainWindow();
    void showHideTrayIcon();
    void setFliteControls();

    //GUI Actions
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
    QAction *trayCancelAction;
    QAction *voiceOptionAction;
    QAction *showDialogueAction;
    QAction *installVoicesAction;
    QAction *customFestivalAction;
    QAction *helpAction;
    QAction *aboutAction;
    QAction *playAction;
    QAction *trayPlayAction;
    QAction *hotKeyPlayAction;
    QAction *handleClipboardAction;
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
    QAction *showWindowAction;
    QAction *quitAction;

    //GUI Menus
    QMenu *fileMenu;
    QMenu *editMenu;
    QMenu *speakMenu;
    QMenu *optionsMenu;
    QMenu *viewMenu;
    QMenu *helpMenu;

    QToolBar *speakToolBar;
    QSystemTrayIcon *trayIcon;
    QMenu *trayIconMenu;
    QIcon speakIcon;
    QIcon cancelIcon;

    //Form variables
    EditorVoiceOptionsDialog *editorVoiceOptionsDialog;
    InstallVoicesDialog *installVoicesDialog;
    FontSettingsDialog *fontSettingsDialog;
    ExportProgressDialog *exportProgressDialog;
    DisplayMessageDialog *displayMessageDialog;
    FliteSettingsDialog *fliteSettingsDialog;
    CustomFestivalDialog *customFestivalDialog;
    DialogueWindow *dialogueWindow;

    //Startup thread and splash screen variables
    StartupThread *startUpThread;

    //Various classes
    TempFilesRemover tempFilesRemover;
    SettingsWriter settingsWriter;

    HotKeyThread hotKeyThread;
    QClipboard *clipboard;
    bool useClipboard;

    //Player related variables
    PlayerControls *controls;
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

    //Status bar labels
    QLabel *maryVoicesStatusLabel;
    QLabel *selectedVoiceLabel;
    QLabel *engineStatusLabel;
    QLabel *percentStatusLabel;

    //Textedit variables
    QPalette defaultPalette;
    QPalette invertedPalette;
    QQueue<unsigned int> beginQueue;
    QQueue<unsigned int> endQueue;
    int beginBlock;
    int endBlock;
    unsigned int cursorPosition;

    //Engine variables
    SpeechEngineInfo *engineInfo;
    SpeechEngine *engine;
    QString engineVoice;
    double rate;
    bool useTrayIcon;
    bool engineIsProcessing;
    bool splitMode;
    bool highlightMode;
    int fliteDuration;
    int fliteTargetMean;
    QString customFestivalCommand;
    QString customFestivalCommandArguments;
    int maxId;
    int currentId;

    SoundFilesMerger *soundFilesMerger;
    QQueue<TextProcessItem> *exportDialogueList;
};

#endif // MAINWINDOW_H
