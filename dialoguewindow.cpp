#include "dialoguewindow.h"
#include "ui_dialoguewindow.h"

#include <QComboBox>
#include <QLineEdit>
#include <QDebug>

DialogueWindow::DialogueWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogueWindow)
{
    ui->setupUi(this);
    ui->removeButton->setEnabled(false);
    ui->saveButton->setEnabled(false);
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

void DialogueWindow::on_loadButton_clicked()
{
    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();

    for (int i = 0; i < allCombos.length(); i++)
    {
        QComboBox *tmpCombo =  allCombos.at(i);
        qDebug() << tmpCombo->currentText();
    }
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
    voices.clear();
    texts.clear();
    filenames.clear();

    QList<QComboBox *> allCombos = ui->scrollArea->findChildren<QComboBox *>();
    QList<QLineEdit *> allLineEdits = ui->scrollArea->findChildren<QLineEdit *>();

    for (int i = 0; i < allCombos.length(); i++)
    {
        QComboBox *tmpCombo =  allCombos.at(i);
        QLineEdit *tmpLineEdit =  allLineEdits.at(i);

        voices.append(tmpCombo->currentText());
        texts.append(tmpLineEdit->text());
        filenames.append(wavPrefix + QString::number(i) + ".wav");
        qDebug() << tmpLineEdit->text();
    }
    speechEngine->setRate(1.0);
    //emit startDialogue();
    speechEngine->makeDialogue(voices, texts, filenames);
    this->hide();
}

void DialogueWindow::setSpeechEngine(SpeechEngine *speechEngine)
{
    this->speechEngine = speechEngine;
}
