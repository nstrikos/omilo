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
    QFont font("Liberation Sans", 12);
    QApplication::setFont(font);
    initFunctions();
}

//ok
MainWindow::~MainWindow()
{
    hotKeyThread.terminate();
    writeSettings();
    if (editorVoiceOptionsDialog != NULL)
        delete editorVoiceOptionsDialog;
    if (fliteSettingsDialog != NULL)
        delete fliteSettingsDialog;
    if (installVoicesAction != NULL)
        delete installVoicesDialog;
    if (startUpThread != NULL)
        delete startUpThread;

    if (maryStartupTimer != NULL)
        delete maryStartupTimer;
    delete engineInfo;
    delete player;
    delete playlist;
    delete engine;
    delete fontComboBox;
    if (trayIconMenu != NULL)
        delete trayIconMenu;
    if (trayIcon != NULL)
        delete trayIcon;
    delete splashScreen;
    removeTempFiles();
    delete ui;
}

void MainWindow::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    menu.addAction(cutAction);
    menu.addAction(copyAction);
    menu.addAction(pasteAction);
    menu.addSeparator();
    menu.addAction(speakSelectedTextAction);
    menu.exec(event->globalPos());
}

//ok
void MainWindow::initFunctions()
{
    setCurrentFile("");

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
}

void MainWindow::initVariables()
{
    //Initialize forms
    editorVoiceOptionsDialog = NULL;
    installVoicesDialog = NULL;
    fliteSettingsDialog = NULL;

    selectedVoiceLabel = NULL;
    splashScreen = NULL;
    maryStartupTimer = NULL;

    engine = new SpeechEngine(engineVoice);
    engine->setSplitMode(splitMode);
    startUpThread = new StartupThread(engine);
    engineInfo = new SpeechEngineInfo();
    //Check if current voice is installed, in case it's been deleted
    checkInstalledVoice();

    hotKeyThread.start();
    clipboard = QApplication::clipboard();
    fontComboBox = new QFontComboBox;
    //spinBox = new QSpinBox;
    //spinBox->setMinimum(8);
    //spinBox->setMaximum(72);
    //spinBox->setValue(qApp->font().pointSize());
    defaultPalette = ui->textEdit->palette();
    invertedPalette = ui->textEdit->palette();
    invertedPalette.setColor(QPalette::Text, QColor(255, 255, 255));
    invertedPalette.setColor(QPalette::Base, QColor(0, 0, 0));
    //beginBlock = 0;
    //endBlock = 0;
    engineIsProcessing = false;
    speakIcon = QIcon(":/images/speak.png");
    cancelIcon = QIcon(":images/cancel.png");
    splitMode = false;
}

//ok
void MainWindow::setupPlayer()
{
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Sequential);
    player->setPlaylist(playlist);
    engine->setPlaylist(playlist);
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
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            controls, SLOT(setState(QMediaPlayer::State)));
}

//ok
void MainWindow::setupLayout()
{
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
    ui->statusBar->addWidget(selectedVoiceLabel);
    updateVoiceLabel();
    engineStatusLabel = new QLabel(this);
    engineStatusLabel->setText(tr("Speech engine is idle"));
    ui->statusBar->addWidget(engineStatusLabel);
}

//ok
void MainWindow::connectSignalsToSlots()
{
    connect(ui->textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    connect(ui->speakButton, SIGNAL(clicked()), this, SLOT(speakButtonPressed()));
    connect(engine, SIGNAL(fileCreated(QString, bool, unsigned int, unsigned int)), this, SLOT(addToPlaylist(QString, bool, unsigned int, unsigned int)));
    connect(engine, SIGNAL(processingFinished()), this, SLOT(updateControlsWhenEngineIsIdle()));
    connect(startUpThread, SIGNAL(maryServerIsUp()), this, SLOT(updateMaryStatus()));
    connect(&hotKeyThread, SIGNAL(playPressed()), this, SLOT(hotKeyPlayPressed()));
    connect(&hotKeyThread, SIGNAL(stopPressed()), this, SLOT(hotKeyStopPressed()));
    connect(fontComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontChanged(QString)));
    connect(playlist, SIGNAL(currentIndexChanged(int)), this, SLOT(highlightSelection()));
}

//ok
void MainWindow::createActions()
{
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
    voiceOptionAction->setShortcut(tr("F3"));
    connect(voiceOptionAction, SIGNAL(triggered()), this, SLOT(selectVoice()));

    installVoicesAction = new QAction(tr("Instal&l voices..."), this);
    installVoicesAction->setShortcut(tr("F4"));
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
    cancelAction->setShortcut(tr("F12"));
    QIcon cancelIcon = QIcon(":/images/stop.png");
    cancelAction->setIcon(cancelIcon);
    cancelAction->setEnabled(false);

    aboutAction = new QAction(tr("About"), this);
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(displayAboutMessage()));

    helpAction = new QAction(tr("Help"), this);
    helpAction->setShortcut(tr("F1"));
    connect(helpAction, SIGNAL(triggered()), this, SLOT(displayHelp()));

    boldAction = new QAction(tr("Bold"), this);
    boldAction->setShortcut(tr("Ctrl+B"));
    boldAction->setCheckable(true);
    connect(boldAction, SIGNAL(triggered()), this, SLOT(updateFont()));

    invertColorsAction = new QAction(tr("Invert Colors"), this);
    invertColorsAction->setShortcut(tr("Ctrl+I"));
    invertColorsAction->setCheckable(true);
    connect(invertColorsAction, SIGNAL(triggered()), this, SLOT(invertPalette()));

    increasePointSizeAction = new QAction(tr("Increase font size"), this);
    increasePointSizeAction->setShortcut(tr("Ctrl+L"));
    connect(increasePointSizeAction, SIGNAL(triggered()), this, SLOT(increasePointSize()));

    decreasePointSizeAction = new QAction(tr("Decrease font size"), this);
    decreasePointSizeAction->setShortcut(tr("Ctrl+K"));
    connect(decreasePointSizeAction, SIGNAL(triggered()), this, SLOT(decreasePointSize()));

    showFontListAction = new QAction(tr("Fonts"), this);
    showFontListAction->setShortcut(tr("Ctrl+F"));
    connect(showFontListAction, SIGNAL(triggered()), this, SLOT(showFontList()));

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
    showFliteSettingsAction->setShortcut(tr("F5"));
    connect(showFliteSettingsAction, SIGNAL(triggered()), this, SLOT(showFliteDialog()));
    restoreAction = new QAction(tr("&Restore"), this);
    connect(restoreAction, SIGNAL(triggered()), this, SLOT(showNormal()));
    quitAction = new QAction(tr("&Quit"), this);
    quitAction->setShortcut(QKeySequence::Quit);
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
}

//ok
void MainWindow::createMenus()
{
    fileMenu = menuBar()->addMenu(tr("&File"));
    fileMenu->addAction(newAction);
    fileMenu->addAction(openAction);
    fileMenu->addAction(saveAction);
    fileMenu->addAction(saveAsAction);

    separatorAction = fileMenu->addSeparator();
    for (int i=0; i<MaxRecentFiles; ++i)
        fileMenu->addAction(recentFileActions[i]);

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
    speakMenu->addAction(speakFromCurrentPositionAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(installVoicesAction);
    optionsMenu->addAction(showFliteSettingsAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(boldAction);
    viewMenu->addAction(invertColorsAction);
    viewMenu->addAction(toggleUseTrayIconAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(helpAction);
    helpMenu->addAction(aboutAction);
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
    if (maybeSave())
    {
        ui->textEdit->clear();
        setCurrentFile("");
    }
    cancelButton_clicked();
}

//ok
void MainWindow::openFile()
{
    if (maybeSave())
    {
        QString filename = QFileDialog::getOpenFileName(this);
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
    if (curfile.isEmpty())
        return saveAs();
    else
        return saveFile(curfile);
}

//ok
bool MainWindow::saveFile(const QString &filename)
{
    QFile file(filename);
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
    QString filename = QFileDialog::getSaveFileName(this);
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
    cancelButton_clicked();
}

//ok
void MainWindow::readSettings()
{
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
    voice = engineVoice;
    useTrayIcon = settings.value("useTrayIcon").toBool();
    toggleUseTrayIconAction->setChecked(useTrayIcon);
    //enableSplitModeAction->setChecked(splitMode);
}

//ok
void MainWindow::writeSettings()
{
    QSettings settings("Omilo-qt5", "Omilo-qt5");
    settings.setValue("MainWindowPosition", pos());
    settings.setValue("MainWindowSize", size());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("MainWindowVoice", this->engineVoice);
    settings.setValue("useTrayIcon", useTrayIcon);
    //settings.setValue("SplitMode", splitMode);
}

void MainWindow::exportToWav()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (!filename.isEmpty())
    {
        //filename += ".wav";
        QString text = ui->textEdit->document()->toPlainText();
        engine->exportWav(filename, text);
        updateControlsWhenEngineIsProcessing();
    }
}

void MainWindow::speakButtonPressed()
{
    QString text = ui->textEdit->document()->toPlainText();
    if (engineIsProcessing)
        cancelButton_clicked();
    else
        speakText(text);
}

void MainWindow::updateControlsWhenEngineIsProcessing()
{
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
    engineIsProcessing = false;
    engineStatusLabel->setText(tr("Speech engine is idle"));
    //ui->speakButton->setEnabled(true);
    ui->speakButton->setIcon(speakIcon);
    speakAction->setEnabled(true);
    exportToWavAction->setEnabled(true);
    installVoicesAction->setEnabled(true);
    //ui->cancelButton->setEnabled(false);
    cancelAction->setEnabled(false);
}

void MainWindow::speakText(QString text)
{
    updateControlsWhenEngineIsProcessing();
    removeTempFiles();
    player->stop();
    controls->disablePlayButton();
    playlist->clear();
    //    genericPlayer->stop();
    //beginQueue.clear();
    //endQueue.clear();
    cursorPosition = 0;
    checkInstalledVoice();

    engine->speak(text);
    ui->textEdit->verticalScrollBar()->setSliderPosition(0);
}

void MainWindow::selectVoice()
{
    if (!editorVoiceOptionsDialog)
        editorVoiceOptionsDialog = new EditorVoiceOptionsDialog(&engineVoice,this);
    editorVoiceOptionsDialog->selectVoice();
    if (editorVoiceOptionsDialog->exec())
    {
        //We dont set voice here because engine maybe processing
        //It's better to set voice voice when we speak
        //This way we dont mess up with voices and players
        //engine->setSpeechVoice(voice);
        updateVoiceLabel();
        engine->setSpeechVoice(engineVoice);
        if (fliteSettingsDialog != NULL)
            fliteSettingsDialog->resetDialog();
    }
}

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

//The new code will mainly evolve this function
void MainWindow::addToPlaylist(QString filename, bool split, unsigned int begin, unsigned int end)
{
    QString currentVoice = engine->getSpeechVoice()->getName();
    //    QFileInfo fileInfo(filename);

    float rate = controls->getPlaybackRate();


    //if (rate == 1)
    //{
    //QFile::copy(filename, "/tmp/temp.wav");


    if (rate != 1)
    {
        QFile::copy(filename, "/tmp/temp.wav");
        QString command = "sox /tmp/temp.wav " + filename + " tempo " + QString::number(controls->getPlaybackRate());
        soxProcess.start(command);
        soxProcess.waitForFinished();
        QFile::remove("/tmp/temp.wav");
    }
    //}

    //    QFile::copy(filename, "/tmp/temp.wav");
    //    QString command = "sox /tmp/temp.wav " + filename + " equalizer 30 .71q +7.2 equalizer 85 1.10q +5.6 equalizer 155 1.80q 0 equalizer 300 .71q -7.2 equalizer 500 2.90q -4.8 equalizer 1500 2.9q +1.6 equalizer 3000 .51q +8 equalizer 6000 0.51q +11.2 equalizer 7000 0.51q +12 equalizer 8000 0.51q +12";
    //    soxProcess.start(command);
    //    soxProcess.waitForFinished();
    //    QFile::remove("/tmp/temp.wav");

    QFileInfo fileInfo(filename);
    if (fileInfo.exists())
    {
        //        if (split)
        //        {
        //            //SOS append before play
        //            //otherwise highlight selection will point to the previous text part
        //            beginQueue.append(begin);
        //            endQueue.append(end);
        //        }
        //        else
        //        {
        //            beginQueue.append(0);
        //            endQueue.append(0);
        //        }

        {
            QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());

            //This is an important change for gstreamer to handle wav files
            //player->setMedia(QUrl::fromLocalFile(filename));
            playlist->addMedia(url);
            if (player->state() == QMediaPlayer::StoppedState)
                player->play();
        }
    }
}

void MainWindow::durationChanged(qint64 duration)
{
    this->duration = duration/1000;
    slider->setMaximum(duration / 1000);
}

void MainWindow::positionChanged(qint64 progress)
{
    if (!slider->isSliderDown()) {
        slider->setValue(progress / 1000);
    }
    updateDurationInfo(progress / 1000);
}

void MainWindow::jump(const QModelIndex &index)
{
    if (index.isValid()) {
        playlist->setCurrentIndex(index.row());
        player->play();
    }
}

void MainWindow::playlistPositionChanged(int currentItem)
{
    //    playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
}

void MainWindow::seek(int seconds)
{
    player->setPosition(seconds * 1000);
}

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

void MainWindow::bufferingProgress(int progress)
{
    setStatusInfo(tr("Buffering %4%").arg(progress));
}

void MainWindow::setTrackInfo(const QString &info)
{
    trackInfo = info;
    Q_UNUSED(trackInfo);
}

void MainWindow::setStatusInfo(const QString &info)
{
    statusInfo = info;
    Q_UNUSED(statusInfo)
}

void MainWindow::displayErrorMessage()
{
    setStatusInfo(player->errorString());
}

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
void MainWindow::removeTempFiles()
{
    QStringList nameFilter("omilo-*");
    QDir directory("/tmp/");
    QStringList txtFilesAndDirectories = directory.entryList(nameFilter);
    for (int i = 0; i < txtFilesAndDirectories.size(); i++)
    {
        QString filename = txtFilesAndDirectories.at(i);
        QString command = "/tmp/" + filename;
        QFile::remove(command);
    }
    if(QFile::exists(textFile))
        QFile::remove(textFile);
    if(QFile::exists(wavFile))
        QFile::remove(wavFile);
    if(QFile::exists(voiceTextEditorFile))
        QFile::remove(voiceTextEditorFile);

    //Festival temporary files
    QStringList nameFilter2("est_*");
    QDir directory2("/tmp/");
    QStringList txtFilesAndDirectories2 = directory2.entryList(nameFilter2);
    for (int i = 0; i < txtFilesAndDirectories2.size(); i++)
    {
        QString filename = txtFilesAndDirectories2.at(i);
        QString command = "/tmp/" + filename;
        QFile::remove(command);
    }
}

void MainWindow::cancelButton_clicked()
{
    //if (engine->getIsProcessing())
    engine->cancel();
    player->stop();
    //    genericPlayer->stop();
    updateControlsWhenEngineIsIdle();
    //ui->cancelButton->setEnabled(false);
    cancelAction->setEnabled(false);
}

//ok
void MainWindow::displayHelp()
{
    QDesktopServices::openUrl(QUrl("file:///usr/share/omilo-qt5/README"));
}

//ok
void MainWindow::displayAboutMessage()
{
    QMessageBox::about(this, tr("About Omilo-Qt5"),
                       tr("Omilo - Text To Speech\n"
                          "Version 0.2\n"
                          "Developer : nstrikos@yahoo.gr\n"
                          "Webpage : http://anoikto.webs.com/omilo\n"
                          "Icons: Gartoon Redux 1.11 "));

}

void MainWindow::play()
{
    if ( (player->state() == QMediaPlayer::StoppedState) ||
         (player->state() == QMediaPlayer::PausedState) )
        player->play();
    else
        player->pause();
}

void MainWindow::stop()
{
    if (engine->getIsProcessing())
        engine->cancel();
    player->stop();
}

void MainWindow::rateUp()
{
    controls->increaseRate();
}

void MainWindow::rateDown()
{
    controls->decreaseRate();
}

void MainWindow::showFliteDialog()
{
    if (!fliteSettingsDialog)
        fliteSettingsDialog = new FliteSettingsDialog();
    fliteSettingsDialog->setModal(true);
    if (fliteSettingsDialog->exec())
    {
        engine->setDurationStretch(fliteSettingsDialog->getDuration());
        engine->setTargetMean(fliteSettingsDialog->getTarget());
    }
}

//ok
void MainWindow::splashTimerExpired()
{
    //Reaching here means mary startup time expired
    //and mary may not be running

    //Show the window and inform the user
    showMainWindow();

    QMessageBox::warning(this, tr("Warning"),
                         tr("Loading takes too long\n"
                            "Mary voices may not be available\n"));
}

void MainWindow::setDefaultVoice()
{
    engineVoice = KalFestival;
    voice = KalFestival;
    engine->setSpeechVoice(voice);
    updateVoiceLabel();
}

//ok
void MainWindow::checkInstalledVoice()
{
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
        setDefaultVoice();
    }
}

void MainWindow::hotKeyPlayPressed()
{
    ui->textEdit->clear();
    QString text = clipboard->text(QClipboard::Selection);
    ui->textEdit->append(text);
    speakText(text);
}

void MainWindow::hotKeyStopPressed()
{
    cancelButton_clicked();
    player->stop();
}

void MainWindow::speakSelectedText()
{
    QTextCursor cursor(ui->textEdit->textCursor());
    const QString text = cursor.selectedText();
    updateControlsWhenEngineIsProcessing();
    player->stop();
    playlist->clear();
    //beginQueue.clear();
    //endQueue.clear();
    engine->speak(text, false);
    //    speakText(text);
}

void MainWindow::fontChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    updateFont();
}

void MainWindow::spinBoxValueChanged(int arg1)
{
    Q_UNUSED(arg1);
    updateFont();
}

//ok
void MainWindow::showMainWindow()
{
    //we close splash screen
    //stop mary startup timer
    //delete startup thread we dont need it any more
    //show the window

    if (splashScreen != NULL)
    {
        splashScreen->finish(this);
        delete splashScreen;
        splashScreen = NULL;
    }


    if (maryStartupTimer != NULL)
    {
        maryStartupTimer->stop();
        delete maryStartupTimer;
        maryStartupTimer = NULL;
    }
    this->show();
}

void MainWindow::updateFont()
{
    QFont font = fontComboBox->currentFont(); //Get the current font
    //font.setPointSize(this->spinBox->value());
    font.setBold(boldAction->isChecked());
    ui->textEdit->setFont(font);
}

void MainWindow::invertPalette()
{
    if (invertColorsAction->isChecked())
        ui->textEdit->setPalette(invertedPalette);
    else
        ui->textEdit->setPalette(defaultPalette);
}

void MainWindow::increasePointSize()
{
    //spinBox->setValue(spinBox->value() + 1);
}

void MainWindow::decreasePointSize()
{
    //spinBox->setValue(spinBox->value() - 1);
}

void MainWindow::showFontList()
{
    fontComboBox->showPopup();
}

void MainWindow::highlightSelection()
{
    int currentIndex = playlist->currentIndex();
    if ( currentIndex >= 0 )
    {
        //        if (!beginQueue.isEmpty() && (!endQueue.isEmpty()))
        //        {
        //            beginBlock = this->beginQueue.dequeue();// + cursorPosition;
        //            endBlock = this->endQueue.dequeue();// + cursorPosition;
        //        }
        //        if (endBlock > 0)
        //        {
        //            beginBlock += cursorPosition;
        //            endBlock += cursorPosition;
        //            if (endBlock > ui->textEdit->document()->toPlainText().size())
        //                endBlock = ui->textEdit->document()->toPlainText().size();
        //            QTextCursor cursor = ui->textEdit->textCursor();
        //            cursor.setPosition(beginBlock, QTextCursor::MoveAnchor);
        //            cursor.setPosition(endBlock, QTextCursor::KeepAnchor);
        //            ui->textEdit->setTextCursor(cursor);
        //        }
        //        if (endBlock == 0)
        //        {
        //            beginBlock = 0;
        //            endBlock = 0;
        //            QTextCursor cursor = ui->textEdit->textCursor();
        //            cursor.setPosition(beginBlock, QTextCursor::MoveAnchor);
        //            cursor.setPosition(endBlock, QTextCursor::KeepAnchor);
        //            ui->textEdit->setTextCursor(cursor);
        //        }
        //ui->cancelButton->setEnabled(true);
        cancelAction->setEnabled(true);
    }
}

void MainWindow::enableSplitMode()
{
    splitMode = enableSplitModeAction->isChecked();
    engine->setSplitMode(splitMode);
}

void MainWindow::speakFromCurrentPosition()
{
    updateControlsWhenEngineIsProcessing();
    removeTempFiles();
    player->stop();
    playlist->clear();
    //beginQueue.clear();
    //endQueue.clear();
    QString text = ui->textEdit->document()->toPlainText();
    QTextCursor cursor = ui->textEdit->textCursor();
    cursorPosition = cursor.position();
    text = text.right(text.size() - cursorPosition);
    engine->setSpeechVoice(engineVoice);
    engine->speak(text, false);
}

void MainWindow::toggleUseTrayIcon()
{
    if (useTrayIcon == true)
        useTrayIcon = false;
    else
        useTrayIcon = true;
    toggleUseTrayIconAction->setChecked(useTrayIcon);
    showHideTrayIcon();
}

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
