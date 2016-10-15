#include "dialoguewindow.h"
#include "ui_dialoguewindow.h"

#include <QComboBox>
#include <QLineEdit>
#include <QFileDialog>
#include <QMessageBox>
#include <QDebug>

DialogueWindow::DialogueWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogueWindow)
{
    ui->setupUi(this);
    ui->removeButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
    pausesEnabled = true;
}

DialogueWindow::~DialogueWindow()
{
    delete ui;
}

void DialogueWindow::on_addButton_clicked()
{
    QComboBox *tmpCombo = new QComboBox();
    QLineEdit *tmpLine = new QLineEdit();

    QHBoxLayout *hBoxLayout = new QHBoxLayout();
    hBoxLayout->addWidget(tmpCombo);
    hBoxLayout->addWidget(tmpLine);

    ui->VLayout->insertLayout(ui->VLayout->count() - 1, hBoxLayout, 0);

    for (unsigned int i = 0; i < speechEngineInfo.installedVoices.size(); i++)
        tmpCombo->addItem(speechEngineInfo.installedVoices.at(i).name);

    checkTotalWidgets();
}

void DialogueWindow::on_removeButton_clicked()
{
    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    QList<QLineEdit *> allLinedits = ui->scrollArea->findChildren<QLineEdit *>();

    if (allCombos.length() > 0)
    {
        QComboBox *tmpCombo = allCombos.at(allCombos.length() - 1);
        ui->VLayout->removeWidget(tmpCombo);
        delete tmpCombo;
        QLineEdit *tmpLineEdit = allLinedits.at(allLinedits.length() - 1);
        ui->VLayout->removeWidget(tmpLineEdit);
        delete tmpLineEdit;
    }
    checkTotalWidgets();
}

void DialogueWindow::checkTotalWidgets()
{
    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    if (allCombos.length() == 0)
    {
        ui->removeButton->setEnabled(false);
        ui->saveButton->setEnabled(false);
    }
    else if (allCombos.length() > 0)
    {
        ui->removeButton->setEnabled(true);
        ui->saveButton->setEnabled(true);
    }
}

void DialogueWindow::on_okButton_clicked()
{
    clearLists();

    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    QList<QLineEdit *> allLineEdits = ui->scrollArea->findChildren<QLineEdit *>();

    for (int i = 0; i < allCombos.length(); i++)
    {
        QComboBox *tmpCombo =  allCombos.at(i);
        QLineEdit *tmpLineEdit =  allLineEdits.at(i);

        voices.append(tmpCombo->currentText());
        texts.append(tmpLineEdit->text().simplified());
        filenames.append(wavPrefix + QString::number(i) + ".wav");
    }
    speechEngine->setRate(1.0);
    speechEngine->makeDialogue(voices, texts, filenames, pausesEnabled);
    this->hide();
}

void DialogueWindow::setSpeechEngine(SpeechEngine *speechEngine)
{
    this->speechEngine = speechEngine;
}

void DialogueWindow::on_saveButton_clicked()
{
    //Set filename
    qDebug() << "Saving as...";
    QString filename = QFileDialog::getSaveFileName(this);
    qDebug() << "Filename is " << filename;
    if (filename.isEmpty())
        return;

    QFile file(filename);
    qDebug() << "Filename is " << filename;
    if (!file.open(QFile::WriteOnly | QFile::Text))
    {
        QMessageBox::warning(this, tr("Omilo"),
                             tr("Cannot write file %1:\n%2.")
                             .arg(filename)
                             .arg(file.errorString()));
        return;
    }

#ifndef QT_NO_CURSOR
    QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

    saveToFile(file);

#ifndef QT_NO_CURSOR
    QApplication::restoreOverrideCursor();
#endif
}

void DialogueWindow::saveToFile(QFile &file)
{
    //Write to file
    QTextStream out (&file);

    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    QList<QLineEdit *> allLineEdits = ui->scrollArea->findChildren<QLineEdit *>();


    for (int i = 0; i < allCombos.length(); i++)
    {
        QComboBox *tmpCombo =  allCombos.at(i);
        QLineEdit *tmpLineEdit =  allLineEdits.at(i);
        out << "(";
        out << "voice=\"" << tmpCombo->currentText() << "\",";
        out << "text=\"" << tmpLineEdit->text().simplified() << "\")\n";
    }
}

void DialogueWindow::on_loadButton_clicked()
{
    QString filename = QFileDialog::getOpenFileName(this);
    qDebug() << "Filename is " << filename;
    if (!filename.isEmpty())
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

#ifndef QT_NO_CURSOR
        QApplication::setOverrideCursor(Qt::WaitCursor);
#endif

        loadFromFile(file);

#ifndef QT_NO_CURSOR
        QApplication::restoreOverrideCursor();
#endif
        qDebug() << "Loaded file..." << filename;
    }
}

void DialogueWindow::loadFromFile(QFile &file)
{
    QTextStream in (&file);
    QString inText = in.readAll();
    makeDialogueFromInputText(inText);
}

void DialogueWindow::makeDialogueFromInputText(QString inText)
{
    int count = 0;
    int pos = 0;
    count = inText.count("voice");
    qDebug() << count;
    if (count > 0)
    {
        removeAllWidgets();
    }
    while (count > 0)
    {
        pos = inText.indexOf("(voice=\"");
        int pos2 = inText.indexOf("\"", 8);
        QString voice = inText.mid(8, pos2 - 8);
        pos = inText.indexOf("text=\"");
        pos2 = inText.indexOf("\"", pos + 5);
        int pos3 = inText.indexOf("\")");
        QString text = inText.mid(pos2 + 1, pos3 - pos2 - 1);
        text = text.simplified();
        int pos4 = inText.indexOf("\")\n");
        inText = inText.right(inText.size() - pos4 - 3);
        qDebug() << voice << text;

        QComboBox *tmpCombo = new QComboBox();
        for (unsigned int i = 0; i < speechEngineInfo.installedVoices.size(); i++)
            tmpCombo->addItem(speechEngineInfo.installedVoices.at(i).name);
        tmpCombo->setCurrentText(voice);
        QLineEdit *tmpLine = new QLineEdit();
        tmpLine->setText(text);

        QHBoxLayout *hBoxLayout = new QHBoxLayout();
        hBoxLayout->addWidget(tmpCombo);
        hBoxLayout->addWidget(tmpLine);

        ui->VLayout->insertLayout(ui->VLayout->count() - 1, hBoxLayout, 0);

        count--;
    }
    checkTotalWidgets();
}

void DialogueWindow::removeAllWidgets()
{
    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    QList<QLineEdit *> allLineEdits = ui->scrollArea->findChildren<QLineEdit *>();

    for (int i = 0; i < allCombos.length(); i++)
    {
        QComboBox *tmpCombo =  allCombos.at(i);
        QLineEdit *tmpLineEdit =  allLineEdits.at(i);

        ui->VLayout->removeWidget(tmpCombo);
        delete tmpCombo;
        ui->VLayout->removeWidget(tmpLineEdit);
        delete tmpLineEdit;
    }
}

void DialogueWindow::clearLists()
{
    voices.clear();
    texts.clear();
    filenames.clear();
}

void DialogueWindow::on_addPausesCheckBox_clicked()
{
    pausesEnabled = ui->addPausesCheckBox->isChecked();
}
