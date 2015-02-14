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
#include <QToolBar>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    initFunctions();
    connectSignalsToSlots();
    if (splashScreenIsDisabled)
        showWindowWithoutCloseButton();
}

MainWindow::~MainWindow()
{
    //hotKeyThread.stop();
    //hotKeyThread.wait();
//        hotKeyThread.terminate();
//        writeSettings();
//        if (editorVoiceOptionsDialog != NULL)
//            delete editorVoiceOptionsDialog;
//        if (fliteSettingsDialog != NULL)
//            delete fliteSettingsDialog;
//        if (installVoicesAction != NULL)
//            delete installVoicesDialog;
//        if (startUpThread != NULL)
//            delete startUpThread;

//        delete splashScreenTimer;
//        delete checkInstalledVoiceTimer;
//        delete engineInfo;
//        delete player;
//        delete playlist;
//        delete engine;
//        removeTempFiles();
//        delete controls;
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{

    //hotKeyThread.stop();
    //hotKeyThread.wait();
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

    delete maryStartupTimer;
    delete checkInstalledVoiceTimer;
    delete engineInfo;
    delete player;
    delete playlist;
    delete engine;
    delete fontComboBox;
    delete spinBox;
    removeTempFiles();
    event->accept();
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

void MainWindow::initFunctions()
{
    setCurrentFile("");
    createActions();
    createMenus();
    readSettings();
    initVariables();
    setupSplashScreen();
    setupMaryStartupTimer();
    createToolBars();
    setupPlayer();
    setupLayout();
}

void MainWindow::initVariables()
{
    editorVoiceOptionsDialog = NULL;
    installVoicesDialog = NULL;
    fliteSettingsDialog = NULL;
    splashScreen = NULL;
    maryStartupTimer = NULL;
    engine = new SpeechEngine(engineVoice);
    startUpThread = new StartupThread(engine);
    engineInfo = new SpeechEngineInfo();
    checkInstalledVoiceTimer = new QTimer();
    connect(checkInstalledVoiceTimer, SIGNAL(timeout()), this, SLOT(checkInstalledVoice()));
    checkInstalledVoiceTimer->start(5000);
    hotKeyThread.start();
    clipboard = QApplication::clipboard();
    fontComboBox = new QFontComboBox;
    spinBox = new QSpinBox;
    spinBox->setMinimum(8);
    spinBox->setMaximum(72);
    spinBox->setValue(qApp->font().pointSize());
}

void MainWindow::setupPlayer()
{
    player = new QMediaPlayer(this);
    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::CurrentItemOnce);
    player->setPlaylist(playlist);
    playlistModel = new PlaylistModel(this);
    playlistModel->setPlaylist(playlist);
    slider = new QSlider(Qt::Horizontal, this);
    slider->setRange(0, player->duration() / 1000);
    connect(slider, SIGNAL(sliderMoved(int)), this, SLOT(seek(int)));

    playlistView = new QListView(this);
    playlistView->setModel(playlistModel);

    controls = new PlayerControls(this);
    controls->setState(player->state());
    controls->setVolume(player->volume());
    controls->setMuted(controls->isMuted());

    connect(player, SIGNAL(durationChanged(qint64)), SLOT(durationChanged(qint64)));
    connect(player, SIGNAL(positionChanged(qint64)), SLOT(positionChanged(qint64)));
    connect(playlist, SIGNAL(currentIndexChanged(int)), SLOT(playlistPositionChanged(int)));
    connect(player, SIGNAL(mediaStatusChanged(QMediaPlayer::MediaStatus)),
            this, SLOT(statusChanged(QMediaPlayer::MediaStatus)));
    connect(player, SIGNAL(bufferStatusChanged(int)), this, SLOT(bufferingProgress(int)));
    connect(player, SIGNAL(error(QMediaPlayer::Error)), this, SLOT(displayErrorMessage()));
    connect(playlistView, SIGNAL(activated(QModelIndex)), this, SLOT(jump(QModelIndex)));
    connect(controls, SIGNAL(play()), player, SLOT(play()));
    connect(controls, SIGNAL(pause()), player, SLOT(pause()));
    connect(controls, SIGNAL(stop()), player, SLOT(stop()));
    connect(controls, SIGNAL(next()), this, SLOT(next()));
    connect(controls, SIGNAL(previous()), this, SLOT(previous()));
    connect(controls, SIGNAL(changeVolume(int)), player, SLOT(setVolume(int)));
    connect(controls, SIGNAL(changeMuting(bool)), player, SLOT(setMuted(bool)));
    //connect(controls, SIGNAL(changeRate(qreal)), player, SLOT(setPlaybackRate(qreal)));
    connect(player, SIGNAL(stateChanged(QMediaPlayer::State)),
            controls, SLOT(setState(QMediaPlayer::State)));
    connect(player, SIGNAL(volumeChanged(int)), controls, SLOT(setVolume(int)));
    connect(player, SIGNAL(mutedChanged(bool)), controls, SLOT(setMuted(bool)));
}

void MainWindow::setupLayout()
{
    labelDuration = new QLabel(this);
    historyLabel = new QLabel(this);
    historyLabel->setText(tr("History"));
    ui->verticalLayout->addWidget(historyLabel);
    ui->verticalLayout->addWidget(playlistView);
    QHBoxLayout *hLayout = new QHBoxLayout;
    hLayout->addWidget(slider);
    hLayout->addWidget(labelDuration);
    ui->verticalLayout->addLayout(hLayout);
    ui->verticalLayout->addWidget(controls);
    selectedVoiceLabel = new QLabel(this);
    ui->statusBar->addWidget(selectedVoiceLabel);
    updateVoiceLabel();
    engineStatusLabel = new QLabel(this);
    engineStatusLabel->setText(tr("Speech engine is idle"));
    ui->statusBar->addWidget(engineStatusLabel);
    ui->cancelButton->setEnabled(false);
}

void MainWindow::connectSignalsToSlots()
{
    connect(ui->textEdit->document(), SIGNAL(contentsChanged()), this, SLOT(documentWasModified()));
    connect(ui->speakButton, SIGNAL(clicked()), this, SLOT(speakButtonPressed()));
    connect(engine, SIGNAL(fileCreated(QString)), this, SLOT(addToPlaylist(QString)));
    connect(startUpThread, SIGNAL(maryServerIsUp()), this, SLOT(updateMaryStatus()));
    connect(&hotKeyThread, SIGNAL(playPressed()), this, SLOT(hotKeyPlayPressed()));
    connect(&hotKeyThread, SIGNAL(stopPressed()), this, SLOT(hotKeyStopPressed()));
    connect(&hotKeyThread, SIGNAL(showWindowPressed()), this, SLOT(hotKeyShowWindowPressed()));
    connect(fontComboBox, SIGNAL(currentIndexChanged(QString)), this, SLOT(fontChanged(QString)));
    connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(spinBoxValueChanged(int)));
}

//GUI Functionality
void MainWindow::createActions()
{
    newAction = new QAction(tr("&New"), this);
    newAction->setIcon(QIcon(":/images/new.png"));
    newAction->setShortcut(QKeySequence::New);
    connect(newAction, SIGNAL(triggered()), this, SLOT(newFile()));

    openAction = new QAction(tr("&Open..."), this);
    openAction->setIcon(QIcon(":/images/open.png"));
    openAction->setShortcut(QKeySequence::Open);
    connect(openAction, SIGNAL(triggered()), this, SLOT(openFile()));

    saveAction = new QAction(tr("&Save"), this);
    saveAction->setIcon(QIcon(":/images/save.png"));
    saveAction->setShortcut(QKeySequence::Save);
    connect(saveAction, SIGNAL(triggered()), this, SLOT(save()));

    saveAsAction = new QAction(tr("Save &As..."), this);
    saveAsAction->setIcon(QIcon(":/images/save-as.png"));
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
    voiceOptionAction->setIcon(QIcon(":/images/voice-options.png"));
    connect(voiceOptionAction, SIGNAL(triggered()), this, SLOT(selectVoice()));

    installVoicesAction = new QAction(tr("Instal&l voices..."), this);
    installVoicesAction->setShortcut(tr("F4"));
    installVoicesAction->setIcon(QIcon(":/images/install-voices.png"));
    connect(installVoicesAction, SIGNAL(triggered()), this, SLOT(installVoices()));

    exportToWavAction = new QAction(tr("&Export to wav..."), this);
    exportToWavAction->setShortcut(tr("Ctrl+E"));
    exportToWavAction->setIcon(QIcon(":/images/export.png"));
    connect(exportToWavAction, SIGNAL(triggered()), this, SLOT(exportToWav()));
    exportToWavAction->setEnabled(false);

    exitAction = new QAction(tr("E&xit"), this);
    exitAction->setShortcut(QKeySequence::Quit);
    exitAction->setIcon(QIcon(":/images/exit.png"));
    connect(exitAction, SIGNAL(triggered()), this, SLOT(close()));

    undoAction = new QAction(tr("&Undo"), this);
    undoAction->setIcon(QIcon(":/images/undo.png"));
    undoAction->setShortcut(QKeySequence::Undo);
    connect (undoAction, SIGNAL(triggered()), ui->textEdit, SLOT(undo()));

    redoAction = new QAction(tr("&Redo"), this);
    redoAction->setIcon(QIcon(":/images/redo.png"));
    redoAction->setShortcut(QKeySequence::Redo);
    connect (redoAction, SIGNAL(triggered()), ui->textEdit, SLOT(redo()));

    cutAction = new QAction(tr("Cu&t"), this);
    QIcon cutIcon = QIcon::fromTheme("editcut", QIcon(":/images/edit-cut.png"));
    cutAction->setIcon(cutIcon);
    cutAction->setShortcut(QKeySequence::Cut);
    connect (cutAction, SIGNAL(triggered()), ui->textEdit, SLOT(cut()));

    copyAction = new QAction(tr("&Copy"), this);
    copyAction->setIcon(QIcon(":/images/copy.png"));
    copyAction->setShortcut(QKeySequence::Copy);
    connect (copyAction, SIGNAL(triggered()), ui->textEdit, SLOT(copy()));

    pasteAction = new QAction(tr("&Paste"), this);
    pasteAction->setIcon(QIcon(":/images/paste.png"));
    pasteAction->setShortcut(QKeySequence::Paste);
    connect (pasteAction, SIGNAL(triggered()), ui->textEdit, SLOT(paste()));

    clearAction = new QAction(tr("C&lear"), this);
    clearAction->setIcon(QIcon(":/images/clear.png"));
    clearAction->setShortcut(tr("Alt+C"));
    connect (clearAction, SIGNAL(triggered()), ui->textEdit, SLOT(clear()));

    selectAllAction = new QAction(tr("Select &All"), this);
    selectAllAction->setIcon(QIcon(":/images/select-all.png"));
    selectAllAction->setShortcut(QKeySequence::SelectAll);
    connect(selectAllAction, SIGNAL(triggered()), ui->textEdit, SLOT(selectAll()));

    speakAction = new QAction(tr("&Speak"), this);
    QIcon speakIcon = QIcon(":/images/speak.png");
    speakAction->setIcon(speakIcon);
    speakAction->setShortcut(tr("F2"));
    connect(speakAction, SIGNAL(triggered()), this, SLOT(speakButtonPressed()));
    ui->speakButton->setIcon(speakIcon);

    speakSelectedTextAction = new QAction(tr("&Speak"), this);
    QIcon speakselectedTextIcon = QIcon(":/images/format-indent-more.png");
    speakSelectedTextAction->setIcon(speakselectedTextIcon);
    speakSelectedTextAction->setShortcut(tr("F7"));
    connect(speakSelectedTextAction, SIGNAL(triggered()), this, SLOT(speakSelectedText()));

    cancelAction = new QAction(tr("Cance&l"), this);
    cancelAction->setShortcut(tr("F12"));
    QIcon cancelIcon = QIcon(":/images/cancel.png");
    cancelAction->setIcon(cancelIcon);
    connect(cancelAction, SIGNAL(triggered()), this, SLOT(on_cancelButton_clicked()));
    cancelAction->setEnabled(false);
    ui->cancelButton->setIcon(cancelIcon);

    aboutAction = new QAction(tr("About"), this);
    aboutAction->setShortcut(tr("F1"));
    aboutAction->setIcon(QIcon(":/images/about.png"));
    connect(aboutAction, SIGNAL(triggered()), this, SLOT(displayAboutMessage()));

    stopAction = new QAction(tr("Stop"), this);
    stopAction->setShortcut(tr("Ctrl+1"));
    stopAction->setIcon(QIcon(":/images/stop.png"));
    connect(stopAction, SIGNAL(triggered()), this, SLOT(stop()));
    previousAction = new QAction(tr("Previous"), this);
    previousAction->setShortcut(tr("Ctrl+2"));
    previousAction->setIcon(QIcon(":/images/previous.png"));
    connect(previousAction, SIGNAL(triggered()), this, SLOT(previous()));
    playAction = new QAction(tr("Play/Pause"), this);
    playAction->setShortcut(tr("Ctrl+3"));
    playAction->setIcon(QIcon(":/images/play.png"));
    connect(playAction, SIGNAL(triggered()), this, SLOT(play()));
    nextAction = new QAction(tr("Next"), this);
    nextAction->setShortcut(tr("Ctrl+4"));
    nextAction->setIcon(QIcon(":/images/next.png"));
    connect(nextAction, SIGNAL(triggered()), this, SLOT(next()));
    volumeMuteAction = new QAction(tr("Mute"), this);
    volumeMuteAction->setShortcut(tr("Ctrl+5"));
    volumeMuteAction->setIcon(QIcon(":/images/volume-mute.png"));
    connect(volumeMuteAction, SIGNAL(triggered()), this, SLOT(volumeMute()));
    volumeDownAction = new QAction(tr("Volume down"), this);
    volumeDownAction->setShortcut(tr("Ctrl+6"));
    volumeDownAction->setIcon(QIcon(":/images/volume-down.png"));
    connect(volumeDownAction, SIGNAL(triggered()), this, SLOT(volumeDown()));
    volumeUpAction = new QAction(tr("Volume up"), this);
    volumeUpAction->setShortcut(tr("Ctrl+7"));
    volumeUpAction->setIcon(QIcon(":/images/volume-up.png"));
    connect(volumeUpAction, SIGNAL(triggered()), this, SLOT(volumeUp()));
    rateDownAction = new QAction(tr("Decrease rate"), this);
    rateDownAction->setShortcut(tr("Ctrl+8"));
    rateDownAction->setIcon(QIcon(":/images/rate-down.png"));
    connect(rateDownAction, SIGNAL(triggered()), this, SLOT(rateDown()));
    rateUpAction = new QAction(tr("Increase rate"), this);
    rateUpAction->setShortcut(tr("Ctrl+9"));
    rateUpAction->setIcon(QIcon(":/images/rate-up.png"));
    connect(rateUpAction, SIGNAL(triggered()), this, SLOT(rateUp()));
    showFliteSettingsAction = new QAction(tr("Flite settings..."), this);
    showFliteSettingsAction->setShortcut(tr("F5"));
    showFliteSettingsAction->setIcon(QIcon(":/images/voice-options.png"));
    connect(showFliteSettingsAction, SIGNAL(triggered()), this, SLOT(showFliteDialog()));
    toggleSplashAction = new QAction(tr("Disable splash screen"), this);
    connect(toggleSplashAction, SIGNAL(triggered()), this, SLOT(toggleSplash()));
    toggleSplashAction->setShortcut(tr("F6"));
    toggleSplashAction->setIcon(QIcon(":/images/toggle-splash.png"));
    hideWindowAction = new QAction(tr("Hide"), this);
    connect(hideWindowAction, SIGNAL(triggered()), this, SLOT(hotKeyShowWindowPressed()));
    hideWindowAction->setShortcut(tr("F12"));

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
    fileMenu->addAction(exitAction);

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
    speakMenu->addAction(previousAction);
    speakMenu->addAction(playAction);
    speakMenu->addAction(nextAction);
    speakMenu->addSeparator();
    speakMenu->addAction(volumeMuteAction);
    speakMenu->addAction(volumeDownAction);
    speakMenu->addAction(volumeUpAction);
    speakMenu->addAction(rateDownAction);
    speakMenu->addAction(rateUpAction);

    optionsMenu = menuBar()->addMenu(tr("&Options"));
    optionsMenu->addAction(installVoicesAction);
    optionsMenu->addAction(showFliteSettingsAction);

    viewMenu = menuBar()->addMenu(tr("&View"));
    viewMenu->addAction(toggleSplashAction);
    viewMenu->addAction(hideWindowAction);

    helpMenu = menuBar()->addMenu(tr("&Help"));
    helpMenu->addAction(aboutAction);
}

void MainWindow::createToolBars()
{
    fileToolBar = addToolBar(tr("&File"));
    fileToolBar->addAction(newAction);
    fileToolBar->addAction(openAction);
    fileToolBar->addAction(saveAction);
    fileToolBar->addAction(saveAsAction);
    fileToolBar->addAction(exportToWavAction);
    fileToolBar->addAction(exitAction);

    editToolBar = addToolBar(tr("&Edit"));
    editToolBar->addAction(undoAction);
    editToolBar->addAction(redoAction);
    editToolBar->addAction(cutAction);
    editToolBar->addAction(copyAction);
    editToolBar->addAction(pasteAction);
    editToolBar->addAction(clearAction);
    editToolBar->addAction(selectAllAction);

    speakToolBar = addToolBar(tr("&Speak"));
    speakToolBar->addAction(speakAction);
    speakToolBar->addAction(speakSelectedTextAction);
    speakToolBar->addAction(cancelAction);
    speakToolBar->addAction(voiceOptionAction);
    speakToolBar->addAction(stopAction);
    speakToolBar->addAction(previousAction);
    speakToolBar->addAction(playAction);
    speakToolBar->addAction(nextAction);
    speakToolBar->addAction(volumeMuteAction);
    speakToolBar->addAction(volumeDownAction);
    speakToolBar->addAction(volumeUpAction);
    speakToolBar->addAction(rateDownAction);
    speakToolBar->addAction(rateUpAction);

    optionsToolBar = addToolBar(tr("&Options"));
    optionsToolBar->addAction(installVoicesAction);
    optionsToolBar->addAction(showFliteSettingsAction);

    viewToolBar = addToolBar((tr("&View")));
    viewToolBar->addAction(toggleSplashAction);
    viewToolBar->addWidget(fontComboBox);
    viewToolBar->addWidget(spinBox);

    helpToolBar = addToolBar(tr("&Help"));
    helpToolBar->addAction(aboutAction);
}

void MainWindow::setupSplashScreen()
{
    if (!splashScreenIsDisabled)
    {
        splashScreen = new QSplashScreen();
        splashScreen->setPixmap(QPixmap(":images/omilo-splash.png"));
        splashScreen->show();
        Qt::Alignment topRight = Qt::AlignRight | Qt::AlignTop;
        splashScreen->showMessage(tr("Loading Mary voices..."), topRight, Qt::black);
        toggleSplashAction->setText(tr("Disable splash screen"));
    }
    else
    {
        toggleSplashAction->setText(tr("Enable splash screen"));
    }
}

void MainWindow::setupMaryStartupTimer()
{
    maryStartupTimer = new QTimer();
    connect(maryStartupTimer, SIGNAL(timeout()), this, SLOT(splashTimerExpired()));
    maryStartupTimer->start(20000);
}

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

void MainWindow::newFile()
{
    if (maybeSave())
    {
        ui->textEdit->clear();
        setCurrentFile("");
    }
}

void MainWindow::openFile()
{
    if (maybeSave())
    {
        QString filename = QFileDialog::getOpenFileName(this);
        if (!filename.isEmpty())
            loadFile(filename);
    }
}

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

bool MainWindow::save()
{
    if (curfile.isEmpty())
        return saveAs();
    else
        return saveFile(curfile);
}

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

bool MainWindow::saveAs()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (filename.isEmpty())
        return false;
    return saveFile(filename);
}

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

QString MainWindow::strippedName(const QString &fullFileName)
{
    return QFileInfo(fullFileName).fileName();
}

void MainWindow::openRecentFile()
{
    if (maybeSave())
    {
        QAction *action = qobject_cast<QAction *>(sender());
        if (action)
            loadFile(action->data().toString());
    }
}

void MainWindow::documentWasModified()
{
    setWindowModified(ui->textEdit->document()->isModified());
}

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
}

void MainWindow::readSettings()
{
    QSettings settings("Omilo", "Omilo");
    QPoint pos = settings.value("MainWindowPosition", QPoint(200, 200)).toPoint();
    QSize size = settings.value("MainWindowSize", QSize(800, 600)).toSize();
    resize(size);
    move(pos);
    recentFiles = settings.value("recentFiles").toStringList();
    updateRecentFileActions();
    engineVoice = settings.value("MainWindowVoice").toString();
    voice = engineVoice;
    writeVoiceToFile(voice);
    splashScreenIsDisabled = settings.value("SplashScreen").toBool();
}

void MainWindow::writeSettings()
{
    QSettings settings("Omilo", "Omilo");
    settings.setValue("MainWindowPosition", pos());
    settings.setValue("MainWindowSize", size());
    settings.setValue("recentFiles", recentFiles);
    settings.setValue("MainWindowVoice", engine->getSpeechVoice()->getName());
    settings.setValue("SplashScreen", splashScreenIsDisabled);
}

void MainWindow::exportToWav()
{
    QString filename = QFileDialog::getSaveFileName(this);
    if (!filename.isEmpty())
    {
        filename += ".wav";
        QString text = ui->textEdit->document()->toPlainText();
        engine->exportWav(filename, text);
        engineStatusLabel->setText(tr("Speech engine is processing..."));
        ui->speakButton->setEnabled(false);
        speakAction->setEnabled(false);
        exportToWavAction->setEnabled(false);
        installVoicesAction->setEnabled(false);
        ui->cancelButton->setEnabled(true);
        cancelAction->setEnabled(true);
    }
}

void MainWindow::speakButtonPressed()
{
    QString text = ui->textEdit->document()->toPlainText();
    engineStatusLabel->setText(tr("Speech engine is processing..."));
    ui->speakButton->setEnabled(false);
    speakAction->setEnabled(false);
    exportToWavAction->setEnabled(false);
    installVoicesAction->setEnabled(false);
    ui->cancelButton->setEnabled(true);
    cancelAction->setEnabled(true);
    speakText(text);
}

void MainWindow::speakText(QString text)
{
    player->stop();
    engine->speak(text);
}

void MainWindow::selectVoice()
{
    if (!editorVoiceOptionsDialog)
        editorVoiceOptionsDialog = new EditorVoiceOptionsDialog(&voice,this);
    editorVoiceOptionsDialog->selectVoice();
    if (editorVoiceOptionsDialog->exec())
    {
        engine->setSpeechVoice(voice);
        updateVoiceLabel();
        writeVoiceToFile(voice);
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
    installVoicesDialog->showWindow(engine->getSpeechVoice()->getName());
}

void MainWindow::addToPlaylist(QString filename)
{
    QFileInfo fileInfo(filename);

    float rate = controls->getPlaybackRate();

    if (rate != 1)
    {
        QFile::copy(filename, "/tmp/temp.wav");
        QString command = "sox /tmp/temp.wav " + filename + " tempo " + QString::number(controls->getPlaybackRate());
        soxProcess.start(command);
        soxProcess.waitForFinished();
        QFile::remove("/tmp/temp.wav");
    }
    if (fileInfo.exists())
    {
        QUrl url = QUrl::fromLocalFile(fileInfo.absoluteFilePath());
        playlist->addMedia(url);
    }
    playlist->setCurrentIndex(playlist->mediaCount() - 1);
    player->play();
    engineStatusLabel->setText(tr("Speech engine is idle"));
    ui->speakButton->setEnabled(true);
    speakAction->setEnabled(true);
    exportToWavAction->setEnabled(true);
    installVoicesAction->setEnabled(true);
    ui->cancelButton->setEnabled(false);
    cancelAction->setEnabled(false);
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

void MainWindow::previous()
{
    playlist->setCurrentIndex(playlist->currentIndex() - 1);
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
    playlistView->setCurrentIndex(playlistModel->index(currentItem, 0));
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
    selectedVoiceLabel->setText(tr("Voice: ") + engine->getSpeechVoice()->getName());
}

void MainWindow::updateMaryStatus()
{
    if (splashScreen != NULL)
    {
        splashScreen->finish(this);
        delete splashScreen;
        splashScreen = NULL;
    }

    if (maryStartupTimer != NULL)
        maryStartupTimer->stop();

    if (QMainWindow::isHidden())
        showWindowWithoutCloseButton();
}

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

void MainWindow::on_cancelButton_clicked()
{
    engine->cancel();
    engineStatusLabel->setText(tr("Speech engine is idle"));
    ui->speakButton->setEnabled(true);
    speakAction->setEnabled(true);
    exportToWavAction->setEnabled(true);
    installVoicesAction->setEnabled(true);
    ui->cancelButton->setEnabled(false);
    cancelAction->setEnabled(false);
}

void MainWindow::displayAboutMessage()
{
    QMessageBox::about(this, tr("About Omilo"),
                       tr("Omilo - Text To Speech\n"
                          "Version 0.10\n"
                          "Developer : nstrikos@yahoo.gr\n"
                          "Webpage : http://anoikto.webs.com/omilo"));

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
    player->stop();
}

void MainWindow::next()
{
    playlist->setCurrentIndex(playlist->currentIndex() + 1);
}


void MainWindow::volumeMute()
{
    player->setMuted(!player->isMuted());
    if (player->isMuted())
    {
        volumeMuteAction->setIcon(QIcon(":/images/volume-active.png"));
    }
    else
    {
        volumeMuteAction->setIcon(QIcon(":/images/volume-mute.png"));
    }
}

void MainWindow::volumeDown()
{
    player->setVolume(player->volume() - 1);
}

void MainWindow::volumeUp()
{
    player->setVolume(player->volume() + 1);
}


void MainWindow::rateUp()
{
    controls->increaseRate();
}

void MainWindow::rateDown()
{
    controls->decreaseRate();
}

void MainWindow::toggleSplash()
{
    if (splashScreenIsDisabled)
    {
        splashScreenIsDisabled = false;
        toggleSplashAction->setText(tr("Disable splash screen"));
    }
    else
    {
        splashScreenIsDisabled = true;
        toggleSplashAction->setText(tr("Enable splash screen"));
    }
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

void MainWindow::splashTimerExpired()
{
    if (splashScreen != NULL)
    {
        splashScreen->finish(this);
        delete splashScreen;
        splashScreen = NULL;
    }
    if (maryStartupTimer != NULL)
        maryStartupTimer->stop();

    if (QMainWindow::isHidden())
        showWindowWithoutCloseButton();

    QMessageBox::warning(this, tr("Warning"),
                         tr("Loading takes too long\n"
                            "Mary voices may not be available\n"));

}

void MainWindow::setDefaultVoice()
{
    engine->setSpeechVoice(KalFestival);
    updateVoiceLabel();
}

void MainWindow::checkInstalledVoice()
{
    engineInfo->update();
    bool found = false;
    int size = engineInfo->installedVoices.size();
    for (int i = 0; i < size; i++)
    {
        VoiceInfo tmp = engineInfo->installedVoices.at(i);
        if (tmp.name == engine->getSpeechVoice()->getName())
            found = true;
    }
    if (!found)
    {
        setDefaultVoice();
    }
}

void MainWindow::hotKeyPlayPressed()
{
    engineStatusLabel->setText(tr("Speech engine is processing..."));
    ui->speakButton->setEnabled(false);
    speakAction->setEnabled(false);
    exportToWavAction->setEnabled(false);
    installVoicesAction->setEnabled(false);
    ui->cancelButton->setEnabled(true);
    cancelAction->setEnabled(true);
    QString text = clipboard->text(QClipboard::Selection);
    player->stop();
    engine->speak(text);
}

void MainWindow::hotKeyStopPressed()
{
    player->stop();
}

void MainWindow::writeVoiceToFile(QString string)
{
    if (string == "")
        string = KalFestival;
    QFile file(voiceTextEditorFile);
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << string;
    // optional, as QFile destructor will already do it:
    file.close();
}

void MainWindow::speakSelectedText()
{
    engineStatusLabel->setText(tr("Speech engine is processing..."));
    ui->speakButton->setEnabled(false);
    speakAction->setEnabled(false);
    exportToWavAction->setEnabled(false);
    installVoicesAction->setEnabled(false);
    ui->cancelButton->setEnabled(true);
    cancelAction->setEnabled(true);
    player->stop();
    QTextCursor cursor(ui->textEdit->textCursor());
    const QString text = cursor.selectedText();
    player->stop();
    engine->speak(text);
}

void MainWindow::hotKeyShowWindowPressed()
{
    if (QMainWindow::isVisible() == true)
    {
        if (QMainWindow::isActiveWindow() == true)
            QMainWindow::setVisible(false);
        else
            QMainWindow::activateWindow();
    }
    else
    {
        QMainWindow::setVisible(true);
        QMainWindow::raise();
        QMainWindow::activateWindow();
    }
}

void MainWindow::fontChanged(const QString &arg1)
{
    Q_UNUSED(arg1);
    ui->textEdit->setFont(this->fontComboBox->currentFont());
}

void MainWindow::spinBoxValueChanged(int arg1)
{
    Q_UNUSED(arg1);
    QTextCursor cursor = ui->textEdit->textCursor();
    ui->textEdit->selectAll();
    ui->textEdit->setFontPointSize(this->spinBox->value());
    ui->textEdit->setTextCursor(cursor);

    //Without this we cannot insert character with the new size
    ui->textEdit->setFontPointSize(this->spinBox->value());
}

void MainWindow::showWindowWithoutCloseButton()
{
    setWindowFlags(Qt::WindowTitleHint | Qt::WindowMinimizeButtonHint);
    this->show();
}
