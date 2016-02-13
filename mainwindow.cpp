#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "installvoicesdialog.h"
#include <QMessageBox>
#include <QFileDialog>
#include <QString>
#include <QTextStream>
#include <QSettings>
#include <QtGui>
#include <QMenu>
#include <QMenuBar>
#include <QScrollBar>
#include <QDesktopServices>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    //Is it necessary to set fonts?
    QFont font("Liberation Sans", 12);
    QApplication::setFont(font);

    initFunctions();
}

MainWindow::~MainWindow()
{
    qDebug() << "Closing application...";

    qDebug() << "Terminate hot keys.";
    hotKeyThread.terminate();

    settingsWriter.write(pos(), size(), recentFiles, engineVoice, useTrayIcon, splitMode, customFestivalCommand, customFestivalCommandArguments);

    if (editorVoiceOptionsDialog != NULL)
        delete editorVoiceOptionsDialog;

    if (fliteSettingsDialog != NULL)
        delete fliteSettingsDialog;

    if (installVoicesDialog != NULL)
        delete installVoicesDialog;

    if (fontSettingsDialog != NULL)
        delete fontSettingsDialog;

    if (exportProgressDialog != NULL)
        delete exportProgressDialog;

    if (displayMessageDialog != NULL)
        delete displayMessageDialog;


    if (startUpThread != NULL)
        delete startUpThread;

    if (maryStartupTimer != NULL)
        delete maryStartupTimer;
    delete engineInfo;
    delete player;
    delete playlist;
    delete engine;

    if (trayIconMenu != NULL)
    {
        qDebug() << "Deleting tray icon menu...";
        delete trayIconMenu;
    }
    if (trayIcon != NULL)
    {
        qDebug() << "Deleting tray icon...";
        delete trayIcon;
    }

    if (splashScreen != NULL)
    {
        qDebug() << "Deleting splash screen...";
        delete splashScreen;
    }

    percentStatusLabel->setText(tr("Removing temporary files..."));
    tempFilesRemover.remove();
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAction);
    menu.addAction(copyAction);
    menu.addAction(pasteAction);
    menu.addSeparator();
    menu.addAction(speakFromCurrentPositionAction);
    menu.addAction(speakSelectedTextAction);
    menu.exec(event->globalPos());
}

//ok
void MainWindow::initFunctions()
{
    qDebug() << "Initializing application...";

    //Create actions,menus and toolbars
    createActions();
    createMenus();

    //It's important to read the settings before initializing variables
    //readsettings should be called after createActions
    //otherwise recentfiles are not initialized
    readSettings();
    initVariables();

    //Create tray icon
    createTrayIcon();

    //Set up splash screen
    setupSplashScreen();

    //If splash screen takes very long a timer will show the window
    setupMaryStartupTimer();

    setupPlayer();
    setupLayout();

    //Finally connect signals to slots
    connectSignalsToSlots();

    qDebug() << "Application initialized.";
}

//ok
void MainWindow::initVariables()
{
    qDebug() << "Initializing application variables...";


    setCurrentFile("");
    rate = 1.0;

    //Initialize forms

    editorVoiceOptionsDialog = NULL;
    installVoicesDialog = NULL;
    fliteSettingsDialog = NULL;
    fontSettingsDialog = NULL;
    exportProgressDialog = NULL;
    displayMessageDialog = NULL;
    customFestivalDialog = NULL;

    selectedVoiceLabel = NULL;
    splashScreen = NULL;
    maryStartupTimer = NULL;

    engine = new SpeechEngine(engineVoice);
    engine->setSplitMode(splitMode);
    qDebug() << "Set split mode to: " << splitMode;

    startUpThread = new StartupThread(engine);

    engineInfo = new SpeechEngineInfo();
    //We have a new speechInfo class
    //So now we can set flite controls.
    //We cannot do it without initializing speechinfo class
    setFliteControls();

    //Check if current voice is installed, in case it's been deleted
    checkInstalledVoice();

    hotKeyThread.start();

    qDebug() << "Setup clipboad handler...";
    clipboard = QApplication::clipboard();

    defaultPalette = ui->textEdit->palette();
    invertedPalette = ui->textEdit->palette();
    invertedPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    invertedPalette.setColor(QPalette::Base, QColor(0, 0, 0));
    beginBlock = 0;
    endBlock = 0;
    engineIsProcessing = false;
    maxId = 0;
    currentId = 0;

    qDebug() << "Setup icons...";
    speakIcon = QIcon(":/images/speak.png");
    cancelIcon = QIcon(":images/cancel.png");

    qDebug() << "All application variables are initialized.";
}

//ok
void MainWindow::setupPlayer()
{
    qDebug() << "Setting up media player...";

    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(playlist);

    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, player->duration() / 1000);
    slider->setMaximumWidth(2000);
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    controls = new PlayerControls(this);
    controls->setState(player->state());

    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));
    connect(controls, SIGNAL(play()), player, SLOT(play()));
    connect(controls, SIGNAL(pause()), player, SLOT(pause()));
    connect(controls, SIGNAL(stop()), player, SLOT(stop()));
    connect(controls, SIGNAL(stop()), this, SLOT(stop()));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            controls, SLOT(setState(QMediaPlayer::State)));

    qDebug() << "Media player initialized.";
}

//ok
void MainWindow::setupLayout()
{
    qDebug() << "Setting Qt layout...";
    labelDuration = new QLabel(this);
    labelDuration->setText("00:00 / 00:00");
    ui->horizontalLayout->setSpacing(0);
    ui->horizontalLayout->addWidget(controls);
    ui->horizontalLayout->addItem( new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed) );
    ui->horizontalLayout->addWidget(labelDuration);
    ui->horizontalLayout->addItem( new QSpacerItem(10, 10, QSizePolicy::Fixed, QSizePolicy::Fixed) );
    ui->horizontalLayout->addWidget(slider);
    ui->horizontalLayout->setStretch(6,1);

    if (selectedVoiceLabel == NULL)
        selectedVoiceLabel = new QLabel(this);
    updateVoiceLabel();
    engineStatusLabel = new QLabel(this);
    engineStatusLabel->setText(tr("Speech engine is idle"));
    percentStatusLabel = new QLabel(this);
    percentStatusLabel->setAlignment(Qt::AlignRight);
    percentStatusLabel->setText("");
    ui->statusBar->addWidget(selectedVoiceLabel);
    ui->statusBar->addWidget(engineStatusLabel);
    ui->statusBar->addWidget(percentStatusLabel);
    ui->statusBar->addWidget(percentStatusLabel,1);
    qDebug() << "Setting Qt layout completed.";
}

//ok
void MainWindow::connectSignalsToSlots()
{
    qDebug() << "Connect all Qt signals to slots...";
    connect(ui->textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    connect(ui->speakButton, SIGNAL(clicked()), this, SLOT(speakButtonPressed()));
    connect(engine, SIGNAL(fileCreated(QString, bool, unsigned int, unsigned int)), this, SLOT(addToPlaylist(QString, bool, unsigned int, unsigned int)));
    connect(engine, SIGNAL(processingFinished()), this, SLOT(updateControlsWhenEngineIsIdle()));
    connect(engine, SIGNAL(newMaxId(int)), this, SLOT(setMaxId(int)));
    connect(engine, SIGNAL(newId(int)), this, SLOT(setCurrentId(int)));
    connect(engine, SIGNAL(exportFinished()), this, SLOT(showExportFinishedMessage()));
    connect(engine, SIGNAL(mergeId(int,int)), this, SLOT(setMergeId(int,int)));
    connect(engine, SIGNAL(mergeInfo(QString)), this, SLOT(setMergeInfo(QString)));
    connect(startUpThread, SIGNAL(maryServerIsUp()), this, SLOT(updateMaryStatus()));
    connect(&hotKeyThread, SIGNAL(playPressed()), this, SLOT(hotKeyPlayPressed()));
    connect(&hotKeyThread, SIGNAL(stopPressed()), this, SLOT(hotKeyStopPressed()));
    connect(&hotKeyThread, SIGNAL(showWindowPressed()), this, SLOT(showNormalAndRaise()));
    connect(&hotKeyThread, SIGNAL(pausePressed()), this, SLOT(play()));
    connect(playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(highlightSelection()));
    qDebug() << "All Qt signals are connected.";
}

//ok
void MainWindow::createActions()
{
    qDebug() << "Creating Qt actions...";

    newAction = new QAction(tr("&New"), this);
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setShortcut(QKeySequence::SaveAs);
    connect(saveAsAction, SIGNAL(triggered()), this, SLOT(saveAs()));

    for (int i=0; i<MaxRecentFiles; ++i)
    {
        recentFileActions[i]=new QAction(this);
        recentFileActions[i]->setVisible(false);
        connect(recentFileActions[i], SIGNAL(triggered()), this, SLOT(openRecentFile()));
    }

    voiceOptionAction = new QAction(tr("Select &voice..."), this);
    voiceOptionAction->setShortcut(tr("F4"));
    connect(voiceOptionAction, SIGNAL(triggered()), this, SLOT(selectVoice()));

    installVoicesAction = new QAction(tr("Instal&l voices..."), this);
    installVoicesAction->setShortcut(tr("F5"));
    connect(installVoicesAction, SIGNAL(triggered()), this, SLOT(installVoices()));

    exportToWavAction = new QAction(tr("&Export to wav..."), this);
    exportToWavAction->setShortcut(tr("Ctrl+E"));
    connect(exportToWavAction, SIGNAL(triggered()), this, SLOT(exportToWav()));
    exportToWavAction->setEnabled(false);

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setShortcut(QKeySequence::Undo);
    connect (undoAction, SIGNAL(triggered()), ui->textEdit, SLOT(undo()));

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setShortcut(QKeySequence::Redo);
    connect (redoAction, SIGNAL(triggered()), ui->textEdit, SLOT(redo()));

    cutAction = new QAction(tr("Cu&t"), this);
    cutAction->setShortcut(QKeySequence::Cut);
    connect (cutAction, SIGNAL(triggered()), ui->textEdit, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setShortcut(QKeySequence::Copy);
    connect (copyAction, SIGNAL(triggered()), ui->textEdit, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setShortcut(QKeySequence::Paste);
    connect (pasteAction, SIGNAL(triggered()), ui->textEdit, SLOT(paste()));

    clearAction = new QAction(tr("C&lear"), this);
    clearAction->setShortcut(tr("Alt+C"));
    connect (clearAction, SIGNAL(triggered()), ui->textEdit, SLOT(clear()));

    selectAllAction = new QAction(tr("Select &All"), this);
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, SIGNAL(triggered()), ui->textEdit, SLOT(selectAll()));

    speakAction = new QAction(tr("&Speak"), this);
    QIcon speakIcon = QIcon(":/images/speak.png");
    speakAction->setIcon(speakIcon);
    speakAction->setShortcut(tr("F2"));
    connect(speakAction, SIGNAL(triggered()), this, SLOT(speakButtonPressed()));
    ui->speakButton->setIcon(speakIcon);

    speakSelectedTextAction = new QAction(tr("&Speak (selected text)"), this);
    speakSelectedTextAction->setShortcut(tr("F7"));
    connect(speakSelectedTextAction, SIGNAL(triggered()), this, SLOT(speakSelectedText()));

    cancelAction = new QAction(tr("Cance&l"), this);
    cancelAction->setShortcut(tr("F3"));
    QIcon cancelIcon = QIcon(":/images/stop.png");
    cancelAction->setIcon(cancelIcon);
    cancelAction->setEnabled(false);
    connect(cancelAction, SIGNAL(triggered()), this, SLOT(cancel()));

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(displayAboutMessage()));

    helpAction = new QAction(tr("Help"), this);
    helpAction->setShortcut(tr("F1"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(displayHelp()));

    showFontSettingsDialogAction = new QAction(tr("Font settings..."), this);
    showFontSettingsDialogAction->setShortcut(tr("Ctrl+F"));
    connect(showFontSettingsDialogAction, SIGNAL(triggered()), this, SLOT(showFontSettingsDialog()));

    customFestivalAction = new QAction(tr("Custom festival command settings..."), this);
    customFestivalAction->setShortcut(tr("Ctrl+U"));
    connect(customFestivalAction, SIGNAL(triggered()), this, SLOT(showCustomFestivalDialog()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setShortcut(tr("Ctrl+B"));
    boldAction->setCheckable(true);
    connect(boldAction, SIGNAL(triggered()), this, SLOT(setBold()));

    invertColorsAction = new QAction(tr("Invert Colors"), this);
    invertColorsAction->setShortcut(tr("Ctrl+I"));
    invertColorsAction->setCheckable(true);
    connect(invertColorsAction, SIGNAL(triggered()), this, SLOT(invertPalette()));

    enableSplitModeAction = new QAction(tr("Enable split mode"), this);
    enableSplitModeAction->setShortcut(tr("Ctrl+T"));
    enableSplitModeAction->setCheckable(true);
    connect(enableSplitModeAction, SIGNAL(triggered()), this, SLOT(enableSplitMode()));

    toggleUseTrayIconAction = new QAction(tr("Show icon in system tray"), this);
    toggleUseTrayIconAction->setShortcut(tr("Ctrl+T"));
    toggleUseTrayIconAction->setCheckable(true);
    connect(toggleUseTrayIconAction, SIGNAL(triggered()), this, SLOT(toggleUseTrayIcon()));

    speakFromCurrentPositionAction = new QAction(tr("Speak from current position"), this);
    speakFromCurrentPositionAction->setShortcut(tr("Ctrl+F2"));
    connect(speakFromCurrentPositionAction, SIGNAL(triggered()), this, SLOT(speakFromCurrentPosition()));

    stopAction = new QAction(tr("Stop"), this);
    stopAction->setShortcut(tr("Ctrl+1"));
    stopAction->setIcon(QIcon(":/images/stop.png"));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));
    playAction = new QAction(tr("Play/Pause"), this);
    playAction->setShortcut(tr("Ctrl+3"));
    playAction->setIcon(QIcon(":/images/play.png"));
    connect(playAction, SIGNAL(triggered()), this, SLOT(play()));
    rateDownAction = new QAction(tr("Decrease rate"), this);
    rateDownAction->setShortcut(tr("Ctrl+8"));
    connect(rateDownAction, SIGNAL(triggered()), this, SLOT(rateDown()));
    rateUpAction = new QAction(tr("Increase rate"), this);
    rateUpAction->setShortcut(tr("Ctrl+9"));
    connect(rateUpAction, SIGNAL(triggered()), this, SLOT(rateUp()));
    showFliteSettingsAction = new QAction(tr("Flite settings..."), this);
    showFliteSettingsAction->setShortcut(tr("F6"));
    connect(showFliteSettingsAction, SIGNAL(triggered()), this, SLOT(showFliteDialog()));
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(tr("Ctrl+Q"));
    connect(quitAction, SIGNAL(triggered()), qApp, SLOT(quit()));
    cutAction->setEnabled(false);
    copyAction->setEnabled(false);
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), cutAction, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), copyAction, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(copyAvailable(bool)), speakSelectedTextAction, SLOT(setEnabled(bool)));
    undoAction->setEnabled(false);
    redoAction->setEnabled(false);
    connect(ui->textEdit, SIGNAL(undoAvailable(bool)), undoAction, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(redoAvailable(bool)), redoAction, SLOT(setEnabled(bool)));
    connect(ui->textEdit, SIGNAL(textChanged()), this, SLOT(documentModified()));

    clearAction->setEnabled(false);
    selectAllAction->setEnabled(false);
    speakAction->setEnabled(false);
    speakSelectedTextAction->setEnabled(false);

    qDebug() << "Qt actions created.";
}

//ok
void MainWindow::createMenus()
{
    qDebug() << "Creating Qt menus...";

    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    separatorAction = fileMenu->addSeparator();
    for (int i=0; i<MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);

    separatorAction = fileMenu->addSeparator();
    fileMenu->addAction(exportToWavAction);
    fileMenu->addAction(quitAction);

    editMenu = menuBar()->addMenu(tr("&Edit"));
    editMenu->addAction(undoAction);
    editMenu->addAction(redoAction);
    editMenu->addSeparator();
    editMenu->addAction(cutAction);
    editMenu->addAction(copyAction);
    editMenu->addAction(pasteAction);
    editMenu->addSeparator();
    editMenu->addAction(clearAction);
    editMenu->addAction(selectAllAction);

    speakMenu = menuBar()->addMenu(tr("&Speak"));
    speakMenu->addAction(speakAction);
    speakMenu->addAction(cancelAction);
    speakMenu->addAction(voiceOptionAction);
    speakMenu->addSeparator();
    speakMenu->addAction(stopAction);
    speakMenu->addAction(playAction);
    speakMenu->addSeparator();
    speakMenu->addAction(rateDownAction);
    speakMenu->addAction(rateUpAction);
    speakMenu->addSeparator();
    speakMenu->addAction(speakSelectedTextAction);
    speakMenu->addAction(speakFromCurrentPositionAction);
    speakMenu->addAction(enableSplitModeAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(installVoicesAction);
    optionsMenu->addAction(showFliteSettingsAction);
    optionsMenu->addAction(customFestivalAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(showFontSettingsDialogAction);
    viewMenu->addAction(boldAction);
    viewMenu->addAction(invertColorsAction);
    viewMenu->addAction(toggleUseTrayIconAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);

    qDebug() << "Qt menus created.";
}

//ok
void MainWindow::createTrayIcon()
{
    qDebug() << "Trying to create tray icon...";

    trayIconMenu = new QMenu(this);
    trayIconMenu->addAction(restoreAction);
    trayIconMenu->addSeparator();
    trayIconMenu->addAction(quitAction);

    trayIcon = new QSystemTrayIcon(this);
    trayIcon->setContextMenu(trayIconMenu);
    QIcon icon = QIcon(":/images/audio-input.png");
    trayIcon->setIcon(icon);
    setWindowIcon(icon);
    showHideTrayIcon();

    qDebug() << "Creating tray icon completed.";
}

//ok
void MainWindow::setupSplashScreen()
{
    qDebug() << "Trying to set up splash screen...";

    splashScreen = new QSplashScreen();
    splashScreen->setPixmap(QPixmap(":images/audio-input-splash.png"));
    splashScreen->show();
    Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
    splashScreen->showMessage(tr("Loading Mary voices..."), topRight, Qt::black);

    qDebug() << "Setting up splash screen completed.";
}

//ok
void MainWindow::setupMaryStartupTimer()
{
    qDebug() << "Set startup timer for 20 seconds...";

    //We will wait for 20 seconds for mary server
    //If timer expires we close splash screen

    maryStartupTimer = new QTimer();
    connect(maryStartupTimer, SIGNAL(timeout()), this, SLOT(splashTimerExpired()));
    maryStartupTimer->start(20000);
}

//ok
void MainWindow::documentModified()
{
    if (ui->textEdit->document()->isEmpty())
    {
        ui->speakButton->setEnabled(false);
        speakAction->setEnabled(false);
        exportToWavAction->setEnabled(false);
        clearAction->setEnabled(false);
        selectAllAction->setEnabled(false);
    }
    else
    {
        ui->speakButton->setEnabled(true);
        speakAction->setEnabled(true);
        exportToWavAction->setEnabled(true);
        clearAction->setEnabled(true);
        selectAllAction->setEnabled(true);
    }
}

//ok
void MainWindow::newFile()
{
    qDebug() << "Creating new file...";
    if (maybeSave())
    {
        ui->textEdit->clear();
        setCurrentFile("");
    }
    cancel();
}

//ok
void MainWindow::openFile()
{
    qDebug() << "Open file...";
    if (maybeSave())
    {
        QString filename = QFileDialog::getOpenFileName(this);
        qDebug() << "Filename is " << filename;
        if (!filename.isEmpty())
            loadFile(filename);
    }
}

//ok
bool MainWindow::maybeSave()
{
    if (ui->textEdit->document()->isModified())
    {
        QMessageBox::StandardButton ret;
        ret = QMessageBox::warning(this, tr("Omilo"),
                                   tr("The document has been modified.\n"
                                      "Do you want to save your changes?"),
                                   QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel);
        if (ret == QMessageBox::Save)
            return save();
        else if (ret == QMessageBox::Cancel)
            return false;
    }
    return true;
}

//ok
bool MainWindow::save()
{
    qDebug() << "Save file...";
    if (curfile.isEmpty())
        return saveAs();
    else
        return saveFile(curfile);
}

//ok
bool MainWindow::saveFile(const QString &filename)
{
    QFile file(filename);
    qDebug() << "Filename is " << filename;
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Omilo"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(filename)
                             .arg(file.errorString()));
        return false;
    }

    QTextStream out (&file);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    out << ui->textEdit->toPlainText();
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(filename);
    return true;
}

//ok
bool MainWindow::saveAs()
{
    qDebug() << "Saving as...";
    QString filename = QFileDialog::getSaveFileName(this);
    qDebug() << "Filename is " << filename;
    if (filename.isEmpty())
        return false;
    return saveFile(filename);
}

//ok
void MainWindow::setCurrentFile(const QString &filename)
{
    curfile = filename;
    setWindowModified(false);
    QString shownName = tr("Untitled");

    if (!curfile.isEmpty())
    {
        shownName = strippedName(curfile);
        recentFiles.removeAll(curfile);
        recentFiles.prepend(curfile);
        updateRecentFileActions();
    }
    setWindowTitle(tr("%1[*]").arg(shownName));
}

//ok
void MainWindow::updateRecentFileActions()
{
    QMutableStringListIterator i(recentFiles);

    while (i.hasNext())
    {
        if (!QFile::exists(i.next()))
            i.remove();
    }

    for (int j=0; j<MaxRecentFiles; ++j)
    {
        if (j<recentFiles.count())
        {
            QString text=tr("&%1 %2").arg(j+1).arg(strippedName(recentFiles[j]));
            recentFileActions[j]->setText(text);
            recentFileActions[j]->setData(recentFiles[j]);
            recentFileActions[j]->setVisible(true);
        }
        else
        {
            recentFileActions[j]->setVisible(false);
        }
    }
    separatorAction->setVisible(!recentFiles.isEmpty());
}

//ok
QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

//ok
void MainWindow::openRecentFile()
{
    if (maybeSave())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

//ok
void MainWindow::documentWasModified()
{
    setWindowModified(ui->textEdit->document()->isModified());
}

//ok
void MainWindow::loadFile(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QFile::ReadOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Omilo"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(filename)
                             .arg(file.errorString()));
        return;
    }

    QTextStream in (&file);

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif
    ui->textEdit->setPlainText(in.readAll());
#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif

    setCurrentFile(filename);
    cancel();
    qDebug() << "Loaded file..." << filename;
}

//ok
void MainWindow::readSettings()
{
    qDebug() << "Reading user settings...";

    QSettings settings("Omilo-qt5", "Omilo-qt5");
    QPoint pos = settings.value("MainWindowPosition", QPoint(200, 200)).toPoint();
    QSize size = settings.value("MainWindowSize", QSize(800, 600)).toSize();
    resize(size);
    move(pos);
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
    engineVoice = settings.value("MainWindowVoice").toString();
    if (engineVoice == "" )
        engineVoice = KalFestival;
    useTrayIcon = settings.value("useTrayIcon").toBool();
    toggleUseTrayIconAction->setChecked(useTrayIcon);
    splitMode = settings.value("SplitMode").toBool();
    enableSplitModeAction->setChecked(splitMode);
    customFestivalCommand = settings.value("customFestivalCommand").toString();
    if (customFestivalCommand == "")
        customFestivalCommand = defaultFestivalCommand;
    customFestivalCommandArguments = settings.value("customFestivalCommandArguments").toString();
    if (customFestivalCommandArguments == "")
        customFestivalCommandArguments = defaultFestivalCommandArguments;

    qDebug() << "Reading user settings completed.";
}

void MainWindow::updateControlsWhenEngineIsProcessing()
{
    qDebug() << "Engine is processing. Updating controls...";
    engineStatusLabel->setText(tr("Speech engine is processing..."));
    engineIsProcessing = true;
    ui->speakButton->setIcon(cancelIcon);
    speakAction->setEnabled(false);
    exportToWavAction->setEnabled(false);
    installVoicesAction->setEnabled(false);
    cancelAction->setEnabled(true);
}

void MainWindow::updateControlsWhenEngineIsIdle()
{
    qDebug() << "Engine is idle. Updating controls...";
    engineIsProcessing = false;
    engineStatusLabel->setText(tr("Speech engine is idle"));
    ui->speakButton->setIcon(speakIcon);
    speakAction->setEnabled(true);
    exportToWavAction->setEnabled(true);
    installVoicesAction->setEnabled(true);
    cancelAction->setEnabled(false);
}

void MainWindow::cancel()
{
    qDebug() << "Stopping all actions...";
    engine->cancel();
    player->stop();
    beginBlock = 0;
    endBlock = 0;
    beginQueue.clear();
    endQueue.clear();
    tempFilesRemover.remove();
    controls->disablePlayButton();
    playlist->clear();
    updateControlsWhenEngineIsIdle();
    percentStatusLabel->setText("");
}

void MainWindow::hotKeyStopPressed()
{
    cancel();
}

void MainWindow::speakButtonPressed()
{
    qDebug() << "Speak button pressed.";
    if (engineIsProcessing)
        cancel();
    else
    {
        QString text = ui->textEdit->document()->toPlainText();
        cursorPosition = 0;
        ui->textEdit->verticalScrollBar()->setSliderPosition(0);
        ui->textEdit->horizontalScrollBar()->setSliderPosition(0);
        speakText(text);
    }
}

void MainWindow::speakFromCurrentPosition()
{
    qDebug() << "Speak from current position.";
    QString text = ui->textEdit->document()->toPlainText();
    QTextCursor cursor = ui->textEdit->textCursor();
    cursorPosition = cursor.position();
    text = text.right(text.size() - cursorPosition);
    speakText(text);
}

void MainWindow::speakText(QString text)
{
    setVariablesBeforeSpeaking();
    highlightMode = true;
    qDebug() << "Speak text : " << text;
    engine->speak(text);
}

void MainWindow::speakSelectedText()
{
    qDebug() << "Speak selected text.";
    QTextCursor cursor(ui->textEdit->textCursor());

    //cursor.selection.toPlainText() keeps \n characters, so we are able to split sentences
    QString text = cursor.selection().toPlainText();
    speakTextWithoutHighlight(text);
}

void MainWindow::speakTextWithoutHighlight(QString text)
{
    setVariablesBeforeSpeaking();
    highlightMode = false;
    qDebug() << "Speak text : " << text;
    engine->speak(text);
}

void MainWindow::exportToWav()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (!filename.isEmpty())
    {
        if (!filename.endsWith(".wav"))
        {
            if (displayMessageDialog == NULL)
            {
                displayMessageDialog = new DisplayMessageDialog(this);
            }
            displayMessageDialog->setText(tr("Not a wav file.\n .wav will be added."));
            displayMessageDialog->show();
            displayMessageDialog->exec();
            filename += ".wav";
        }
        QString text = ui->textEdit->document()->toPlainText();

        setVariablesBeforeSpeaking();

        if (splitMode)
        {
            if (exportProgressDialog == NULL)
            {
                exportProgressDialog = new ExportProgressDialog(this);
                qDebug() << "Export progress dialog created...";
            }
            exportProgressDialog->setLabelText(tr("Exporting..."));
            exportProgressDialog->reset();
            exportProgressDialog->show();
            exportProgressDialog->setModal(true);
        }
        engine->exportWav(filename, text);
    }
}

void MainWindow::setVariablesBeforeSpeaking()
{
    updateControlsWhenEngineIsProcessing();
    tempFilesRemover.remove();
    player->stop();
    controls->disablePlayButton();
    playlist->clear();
    beginBlock = 0;
    endBlock = 0;
    beginQueue.clear();
    endQueue.clear();
    percentStatusLabel->setText("");
    checkInstalledVoice();

    engine->setSpeechVoice(engineVoice);
    qDebug() << "Set engine voice: " << engineVoice;
    splitMode = enableSplitModeAction->isChecked();
    engine->setSplitMode(splitMode);
    qDebug() << "Set split mode: " << splitMode;
    if (fliteSettingsDialog != NULL)
    {
        engine->setDurationStretch(fliteSettingsDialog->getDuration());
        engine->setTargetMean(fliteSettingsDialog->getTarget());
    }
    if (engineVoice == CustomFestival)
    {
        engine->setCustomFestivalCommand(customFestivalCommand);
        engine->setCustomFestivalCommandArguments(customFestivalCommandArguments);
    }
    rate = controls->getPlaybackRate();
    engine->setRate(rate);
    qDebug() << "Set engine rate: " << rate;
}

void MainWindow::hotKeyPlayPressed()
{
    ui->textEdit->clear();
    QString text = clipboard->text(QClipboard::Selection);
    ui->textEdit->append(text);
    cursorPosition = 0;
    speakText(text);
}

//ok
void MainWindow::selectVoice()
{
    if (!editorVoiceOptionsDialog)
        editorVoiceOptionsDialog = new EditorVoiceOptionsDialog(&engineVoice, engineInfo, this);

    editorVoiceOptionsDialog->selectVoice();
    if (editorVoiceOptionsDialog->exec())
    {
        //We dont set voice here because engine maybe processing
        //It's better to set voice voice when we speak
        //This way we dont mess up with voices and players
        //engine->setSpeechVoice(engineVoice);

        updateVoiceLabel();
        setFliteControls();
    }
}

//ok
void MainWindow::setFliteControls()
{
    if (engineInfo->getVoiceMode(engineVoice) == flite)
    {
        if (fliteSettingsDialog != NULL)
        {
            fliteSettingsDialog->update(engineVoice);
        }
        showFliteSettingsAction->setEnabled(true);
    }
    else
    {
        showFliteSettingsAction->setEnabled(false);
    }
}

//ok
void MainWindow::installVoices()
{
    if (!installVoicesDialog)
    {
        installVoicesDialog = new InstallVoicesDialog(this);
        connect(installVoicesDialog, SIGNAL(updateVoice()), this, SLOT(setDefaultVoice()));
        installVoicesDialog->setModal(true);
    }
    installVoicesDialog->showWindow(engineVoice);
}

//ok
void MainWindow::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    slider->setMaximum(duration / 1000);
}

//ok
void MainWindow::positionChanged(qint64 progress)
{
    if (!slider->isSliderDown()) {
        slider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

//ok
void MainWindow::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
    }
}

//What is the purpose of this function?
void MainWindow::playlistPositionChanged(int currentItem)
{
    //    playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
    Q_UNUSED(currentItem);
}

//ok
void MainWindow::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

//ok
void MainWindow::statusChanged(QMediaPlayer::MediaStatus status)
{
    handleCursor(status);

    // handle status message
    switch (status) {
    case QMediaPlayer::UnknownMediaStatus:
    case QMediaPlayer::NoMedia:
    case QMediaPlayer::LoadedMedia:
    case QMediaPlayer::BufferingMedia:
    case QMediaPlayer::BufferedMedia:
        setStatusInfo(QString());
        break;
    case QMediaPlayer::LoadingMedia:
        setStatusInfo(tr("Loading..."));
        break;
    case QMediaPlayer::StalledMedia:
        setStatusInfo(tr("Media Stalled"));
        break;
    case QMediaPlayer::EndOfMedia:
        QApplication::alert(this);
        break;
    case QMediaPlayer::InvalidMedia:
        displayErrorMessage();
        break;
    }
}

//ok
void MainWindow::handleCursor(QMediaPlayer::MediaStatus status)
{
#ifndef QT_NO_CURSOR
    if (status == QMediaPlayer::LoadingMedia ||
            status == QMediaPlayer::BufferingMedia ||
            status == QMediaPlayer::StalledMedia)
        setCursor(QCursor(Qt::BusyCursor));
    else
        unsetCursor();
#endif
}

//ok
void MainWindow::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}

//ok
void MainWindow::setTrackInfo(const QString &info)
{
    trackInfo = info;
    Q_UNUSED(trackInfo);
}

//ok
void MainWindow::setStatusInfo(const QString &info)
{
    statusInfo = info;
    Q_UNUSED(statusInfo)
}

//ok
void MainWindow::displayErrorMessage()
{
    setStatusInfo(player->errorString());
}

//ok
void MainWindow::updateDurationInfo(qint64 currentInfo)
{
    QString tStr;
    if (currentInfo || duration) {
        QTime currentTime((currentInfo/3600)%60, (currentInfo/60)%60, currentInfo%60, (currentInfo*1000)%1000);
        QTime totalTime((duration/3600)%60, (duration/60)%60, duration%60, (duration*1000)%1000);
        QString format = "mm:ss";
        if (duration > 3600)
            format = "hh:mm:ss";
        tStr = currentTime.toString(format) + " / " + totalTime.toString(format);
    }
    labelDuration->setText(tStr);
}

//ok
void MainWindow::updateVoiceLabel()
{
    if (selectedVoiceLabel == NULL)
        selectedVoiceLabel = new QLabel(this);
    selectedVoiceLabel->setText(tr("Voice: ") + engineVoice);
}

//ok
void MainWindow::updateMaryStatus()
{
    //startup thread connects here
    //Reaching here means mary server is up and running
    //splashTimerExpired may be called first
    //if mary is not running

    //Just show the window

    showMainWindow();
}

//ok
void MainWindow::displayHelp()
{
    qDebug() << "Display help file...";
    QDesktopServices::openUrl(QUrl("file:///usr/share/omilo-qt5/README"));
}

//ok
void MainWindow::displayAboutMessage()
{
    qDebug() << "Display about message...";
    if (displayMessageDialog == NULL)
    {
        displayMessageDialog = new DisplayMessageDialog(this);
    }
    displayMessageDialog->setText(tr("Omilo - Text To Speech\nVersion 0.3\nDeveloper : nstrikos@yahoo.gr\nWebpage : http://anoikto.webs.com/omilo\nGerman translation : Heinrich Schwietering\nheinrich.schwietering@gmx.de\nIcons: Gartoon Redux 1.11 "));
    displayMessageDialog->show();
    displayMessageDialog->setModal(true);
    displayMessageDialog->exec();
}

//ok
void MainWindow::play()
{
    if ( (player->state() == QMediaPlayer::StoppedState) ||
         (player->state() == QMediaPlayer::PausedState) )
    {
        player->play();
        qDebug() << "Media player is playing...";
    }
    else
    {
        player->pause();
        qDebug() << "Media player is paused...";
    }
}

//ok
void MainWindow::stop()
{
    qDebug() << "Stop media player.";
    player->stop();
}

//ok
void MainWindow::rateUp()
{
    controls->increaseRate();
}

//ok
void MainWindow::rateDown()
{
    controls->decreaseRate();
}

//ok
void MainWindow::showFliteDialog()
{
    if (!fliteSettingsDialog)
        fliteSettingsDialog = new FliteSettingsDialog(engineVoice, this);
    fliteSettingsDialog->setModal(true);
    fliteSettingsDialog->exec();
}

//ok
void MainWindow::splashTimerExpired()
{
    //Reaching here means mary startup time expired
    //and mary may not be running

    //Show the window and inform the user
    qDebug() << "Startup timer expired...";
    showMainWindow();

    if (displayMessageDialog == NULL)
    {
        displayMessageDialog = new DisplayMessageDialog(this);
    }
    displayMessageDialog->setText(tr("Loading takes too long\nMary voices may not be available."));
    displayMessageDialog->show();
    displayMessageDialog->exec();
}

//ok
void MainWindow::setDefaultVoice()
{
    qDebug() << "Setting default voice...";
    engineVoice = KalFestival;
    engine->setSpeechVoice(engineVoice);
    updateVoiceLabel();
}

//ok
void MainWindow::checkInstalledVoice()
{
    //This should not be here
    //But it's nice to have a clean function to check
    //if voice is installed

    qDebug() << "Checking if selected voice is installed...";
    engineInfo->update();
    bool found = false;
    int size = engineInfo->installedVoices.size();
    for (int i = 0; i < size; i++)
    {
        VoiceInfo tmp = engineInfo->installedVoices.at(i);
        if (tmp.name == engineVoice)
            found = true;
    }
    if (!found)
    {
        qDebug() << "Cannot find selected voice...";
        setDefaultVoice();
    }
    else
    {
        qDebug() << "Selected voice is installed.";
    }
}

//ok
void MainWindow::showMainWindow()
{
    //we close splash screen
    //stop mary startup timer
    //delete startup thread we dont need it any more
    //show the window

    qDebug() << "Deleting splash screen...";
    if (splashScreen != NULL)
    {
        splashScreen->finish(this);
        delete splashScreen;
        splashScreen = NULL;
    }


    qDebug() << "Deleting start up timer...";
    if (maryStartupTimer != NULL)
    {
        maryStartupTimer->stop();
        delete maryStartupTimer;
        maryStartupTimer = NULL;
    }

    if (startUpThread != NULL)
    {
        delete startUpThread;
        startUpThread = NULL;
    }

    qDebug() << "Show main window.";
    this->show();
}

//ok
void MainWindow::enableSplitMode()
{
    //We dont set split mode here because engine maybe processing
    //It's better to set when we speak
    //This way we dont mess up with voices and players
    splitMode = enableSplitModeAction->isChecked();
}

//ok
void MainWindow::toggleUseTrayIcon()
{
    if (useTrayIcon == true)
        useTrayIcon = false;
    else
        useTrayIcon = true;
    toggleUseTrayIconAction->setChecked(useTrayIcon);
    showHideTrayIcon();
}

//ok
void MainWindow::showHideTrayIcon()
{
    if (useTrayIcon)
    {
        trayIcon->show();
        qDebug() << "Tray icon is visible.";
        QApplication::setQuitOnLastWindowClosed(false);
        qDebug() << "Application will not quit after last window is closed";
    }
    else
    {
        trayIcon->hide();
        qDebug() << "Tray icon is hidden.";
        QApplication::setQuitOnLastWindowClosed(true);
        qDebug() << "Application will quit after last window is closed.";
    }
}

//ok
void MainWindow::addToPlaylist(QString filename, bool split, unsigned int begin, unsigned int end)
{
    QFileInfo fileInfo(filename);
    if (fileInfo.exists())
    {
        if (split)
        {
            //SOS append before play
            //otherwise highlight selection will point to the previous text part
            beginQueue.append(begin);
            endQueue.append(end);
        }
        else
        {
            beginQueue.append(0);
            endQueue.append(0);
        }

        {
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());

            //This is an important change for gstreamer to handle wav files
            //player->setMedia(QUrl::fromLocalFile(filename));
            playlist->addMedia(url);
            //if (player->state() == QMediaPlayer::StoppedState)
            //    player->play();
            if (playlist->currentIndex() == -1)
                player->play();
        }
    }
}

void MainWindow::highlightSelection()
{
    if (highlightMode)
    {
        if (splitMode)
        {
            int currentIndex = playlist->currentIndex();
            if ( currentIndex >= 0 )
            {
                if (!beginQueue.isEmpty() && (!endQueue.isEmpty()))
                {
                    beginBlock = this->beginQueue.dequeue();
                    endBlock = this->endQueue.dequeue();
                }
                if (endBlock > 0)
                {
                    beginBlock += cursorPosition;
                    endBlock += cursorPosition;
                    if (endBlock > ui->textEdit->document()->toPlainText().size())
                        endBlock = ui->textEdit->document()->toPlainText().size();
                    QTextCursor cursor = ui->textEdit->textCursor();
                    cursor.setPosition(beginBlock, QTextCursor::MoveAnchor);
                    cursor.setPosition(endBlock, QTextCursor::KeepAnchor);
                    ui->textEdit->setTextCursor(cursor);
                }
                if (endBlock == 0)
                {
                    beginBlock = 0;
                    endBlock = 0;
                    QTextCursor cursor = ui->textEdit->textCursor();
                    cursor.setPosition(beginBlock, QTextCursor::MoveAnchor);
                    cursor.setPosition(endBlock, QTextCursor::KeepAnchor);
                    ui->textEdit->setTextCursor(cursor);
                }
                cancelAction->setEnabled(true);
            }
        }
    }
}

//ok
void MainWindow::showFontSettingsDialog()
{
    if (fontSettingsDialog == NULL)
    {
        qDebug() << "Font settings dialog created...";
        fontSettingsDialog = new FontSettingsDialog();
    }
    fontSettingsDialog->setModal(true);

    if (fontSettingsDialog->exec())
    {
        QFont font = fontSettingsDialog->getFont();
        font.setPointSize(fontSettingsDialog->getPointSize());
        font.setBold(boldAction->isChecked());
        ui->textEdit->setFont(font);
        qDebug() << "Font family: " << font.family();
        qDebug() << "Point size: " << font.pointSize();
        qDebug() << "Bold: " << boldAction->isChecked();
    }
}

//ok
void MainWindow::setBold()
{
    QFont font = ui->textEdit->font();
    font.setBold(boldAction->isChecked());
    ui->textEdit->setFont(font);
    qDebug() << "Bold: " << boldAction->isChecked();
}

//ok
void MainWindow::invertPalette()
{
    if (invertColorsAction->isChecked())
    {
        ui->textEdit->setPalette(invertedPalette);
        qDebug() << "Using inverted palette...";
    }
    else
    {
        ui->textEdit->setPalette(defaultPalette);
        qDebug() << "Using default palette...";
    }
}

void MainWindow::setMaxId(int maxId)
{
    this->maxId = maxId;
    if (exportProgressDialog != NULL)
        exportProgressDialog->setMaxId(maxId);
}

void MainWindow::setCurrentId(int id)
{
    this->currentId = id;
    if (maxId != 0)
    {
        double percent = (double) currentId / maxId * 100;
        percentStatusLabel->setText(" " + tr("Processed ") + QString::number(currentId) + tr(" of ") + QString::number(maxId) + " " + QString::number(percent, 'f', 2) + "%");
    }
    if (exportProgressDialog != NULL)
        exportProgressDialog->setCurrentId(id);
}

void MainWindow::showExportFinishedMessage()
{
    qDebug() << "Exporting finished...";
    if (exportProgressDialog != NULL)
        exportProgressDialog->setVisible(false);
    if (displayMessageDialog == NULL)
    {
        displayMessageDialog = new DisplayMessageDialog(this);
    }
    displayMessageDialog->setText(tr("File created successfully."));
    displayMessageDialog->show();
    displayMessageDialog->exec();
    percentStatusLabel->setText("");
}

void MainWindow::setMergeId(int id, int size)
{
    double percent = (double) id / size * 100;
    percentStatusLabel->setText(" " + tr("Merged ") + QString::number(id) + tr(" of ") + QString::number(size) + " " + QString::number(percent, 'f', 2) + "%");
    exportProgressDialog->setMaxId(size);
    exportProgressDialog->setCurrentId(id);
    exportProgressDialog->setLabelText(tr("Merging..."));
}

void MainWindow::setMergeInfo(QString info)
{
    percentStatusLabel->setText(info);
}

void MainWindow::showCustomFestivalDialog()
{
    if (!customFestivalDialog)
        customFestivalDialog = new CustomFestivalDialog(&customFestivalCommand, &customFestivalCommandArguments, this);
    customFestivalDialog->setModal(true);
    customFestivalDialog->showDialog();

    //customFestivalDialog->setCustomFestivalCommand(customFestivalCommand);
    //customFestivalDialog->setCustomFestivalCommandArguments(customFestivalCommandArguments);

    if (customFestivalDialog->exec())
    {
        //We dont set voice here because engine maybe processing
        //It's better to set voice voice when we speak
        //This way we dont mess up with voices and players
        //engine->setSpeechVoice(engineVoice);

    }
}

void MainWindow::showNormalAndRaise()
{
    this->showNormal();
    this->raise();
    QApplication::setActiveWindow(this);
}
